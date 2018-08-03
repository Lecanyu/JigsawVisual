#include "JigsawVisualPair.h"


namespace JigsawVisualNS
{
	JigsawVisualPair::JigsawVisualPair(int frame_num, std::string root_dir, Alignments2d& alignment)
	{
		relative_ = alignment;
		frameNum_ = frame_num;
		originalImgArr_.resize(frameNum_);
		croppedImgArr_.resize(frameNum_);

		for (int i = 0; i < relative_.data_.size(); ++i)
		{
			int v[2];
			v[0] = relative_.data_[i].frame1_;	v[1] = relative_.data_[i].frame2_;
			for (int k = 0; k < 2; ++k)
			{
				int fragmentId = v[k];
				if (originalImgArr_[fragmentId].empty())
				{
					std::cout << "Load image " << fragmentId << "...";
					std::stringstream ss;
					char image_name[128];
					sprintf(image_name, "fragment_%04d.png", fragmentId + 1);				//use a special filename pattern
					ss << root_dir << image_name;
					cv::Mat img = cv::imread(ss.str());
					originalImgArr_[fragmentId] = img;
					std::cout << "Done!\n";
				}
			}
		}

		for (int i = 0; i < frameNum_; ++i)
			cropTranslation_.data_.push_back(FramedTransformation2d(i, i, 0.0, Eigen::Matrix3d::Identity()));

		for (int i = 0; i < relative_.data_.size(); ++i)
		{
			auto frame1 = relative_.data_[i].frame1_;
			auto frame2 = relative_.data_[i].frame2_;
			IdPair idpair(frame1, frame2);
			mapIdPair2EdgeIds_[idpair].edgeIds.push_back(i);
			mapIdPair2EdgeIds_[idpair].selectId = -1;
		}
	}

	JigsawVisualPair::JigsawVisualPair(int frame_num, std::string root_dir, std::string relative_file, bool isLine)
		:frameNum_(frame_num)
	{
		if (isLine)
			relative_.LoadFromFileLine(relative_file);
		else
			relative_.LoadFromFile(relative_file);

		originalImgArr_.resize(frameNum_);
		croppedImgArr_.resize(frameNum_);

		for (int i = 0; i < relative_.data_.size(); ++i)
		{
			int v[2];
			v[0] = relative_.data_[i].frame1_;	v[1] = relative_.data_[i].frame2_;
			for (int k = 0; k < 2; ++k)
			{
				int fragmentId = v[k];
				if (originalImgArr_[fragmentId].empty())
				{
					std::cout << "Load image " << fragmentId << "...";
					std::stringstream ss;
					char image_name[128];
					sprintf(image_name, "fragment_%04d.png", fragmentId + 1);				//use a special filename pattern
					ss << root_dir << image_name;
					cv::Mat img = cv::imread(ss.str());
					originalImgArr_[fragmentId] = img;
					std::cout << "Done!\n";
				}
			}
		}

		for (int i = 0; i < frameNum_; ++i)
			cropTranslation_.data_.push_back(FramedTransformation2d(i, i, 0.0, Eigen::Matrix3d::Identity()));

		for (int i = 0; i < relative_.data_.size(); ++i)
		{
			auto frame1 = relative_.data_[i].frame1_;
			auto frame2 = relative_.data_[i].frame2_;
			IdPair idpair(frame1, frame2);
			mapIdPair2EdgeIds_[idpair].edgeIds.push_back(i);
			mapIdPair2EdgeIds_[idpair].selectId = -1;
		}
	}


	void JigsawVisualPair::CropImages(uchar r, uchar g, uchar b)
	{
		for (int i = 0; i < originalImgArr_.size(); ++i)
		{
			if (originalImgArr_[i].empty())
				continue;
			std::cout << "Crop image " << i << "...";
			UV min_uv, max_uv;
			FindBoundingBox(originalImgArr_[i], r, g, b, min_uv, max_uv);
			cv::Rect bb(min_uv.v, min_uv.u, max_uv.v - min_uv.v + 1, max_uv.u - min_uv.u + 1);
			cv::Mat croppedRef(originalImgArr_[i], bb);
			cv::Mat croppedImage;
			croppedRef.copyTo(croppedImage);
			croppedImgArr_[i] = croppedImage;

			// we should modify cropTranslation_, so that get correct visual result
			cropTranslation_.data_[i].transformation_(0, 2) += min_uv.u;
			cropTranslation_.data_[i].transformation_(1, 2) += min_uv.v;

			std::cout << "Done!\n";
		}
	}

	void JigsawVisualPair::FindBoundingBox(const cv::Mat& img, uchar r, uchar g, uchar b, UV& out_min_uv, UV& out_max_uv)
	{
		int min_u = 99999, min_v = 99999;
		int max_u = -99999, max_v = -99999;
		for (int u = 0; u < img.rows; ++u)
		{
			for (int v = 0; v < img.cols; ++v)
			{
				cv::Vec3b intensity = img.at<cv::Vec3b>(u, v);
				uchar blue = intensity.val[0];
				uchar green = intensity.val[1];
				uchar red = intensity.val[2];
				if (r == red && g == green && b == blue)
					continue;
				if (min_u > u) min_u = u;
				if (min_v > v) min_v = v;
				if (max_u < u) max_u = u;
				if (max_v < v) max_v = v;
			}
		}
		out_min_uv.u = min_u;	out_min_uv.v = min_v;
		out_max_uv.u = max_u;	out_max_uv.v = max_v;
	}

	void JigsawVisualPair::AssemblePair(int v1, int v2, uchar r, uchar g, uchar b, int multi_id /*=0*/)
	{
		std::cout << "Assembling...";
		// init
		std::vector<DummyImage> dummy_images;
		dummy_images.push_back(DummyImage(croppedImgArr_[v1], r, g, b));
		dummy_images.push_back(DummyImage(croppedImgArr_[v2], r, g, b));

		int total_pixel = 0;
		std::vector<std::unordered_map<UV, int, hash_big, key_equal>> mapUV2IdArray(dummy_images.size());
		for (int ix = 0; ix < dummy_images.size(); ++ix)
		{
			for (int i = 0; i < dummy_images[ix].pixels_.size(); ++i)
			{
				UV uv = dummy_images[ix].pixels_[i].uv;
				mapUV2IdArray[ix][uv] = i;
			}
			total_pixel += dummy_images[ix].pixels_.size();
		}

		// transform
		dummy_images[0].Transform_f(cropTranslation_.data_[v1].transformation_);	// first move to original position to eliminate cropped influence
		dummy_images[1].Transform_f(cropTranslation_.data_[v2].transformation_);

		IdPair idpair(v1, v2);
		int edgeId = mapIdPair2EdgeIds_[idpair].edgeIds[multi_id];
		dummy_images[0].Transform_f(Eigen::Matrix3d::Identity());			// then transform
		dummy_images[1].Transform_f(relative_.data_[edgeId].transformation_);

		// assemble
		DummyImage big_img;
		big_img.pixels_.reserve(total_pixel);
		for (int ix = 0; ix < dummy_images.size(); ++ix)
		{
			auto& transformed_img = dummy_images[ix];

			UV min_uv, max_uv;
			transformed_img.BoundingBox_f(min_uv, max_uv);

			for (int u = min_uv.u; u < max_uv.u; ++u)
			{
				for (int v = min_uv.v; v < max_uv.v; ++v)
				{
					UV query_uv(u, v);
					Eigen::Matrix3d t;
					if (ix == 0)
						t = cropTranslation_.data_[v1].transformation_.inverse();
					else
						t = cropTranslation_.data_[v2].transformation_.inverse()*relative_.data_[edgeId].transformation_.inverse();
					query_uv.Transform(t);
					if (mapUV2IdArray[ix].find(query_uv) == mapUV2IdArray[ix].end())
						continue;
					auto pixel_index = mapUV2IdArray[ix][query_uv];
					Pixel new_pixel = dummy_images[ix].pixels_[pixel_index];
					new_pixel.uv.u = u;	new_pixel.uv.v = v;
					big_img.pixels_.push_back(new_pixel);
				}
			}
		}

		UV min_uv, max_uv;
		big_img.BoundingBox_i(min_uv, max_uv);
		int offset_u = -min_uv.u;
		int offset_v = -min_uv.v;
		Eigen::Matrix3d offset;
		offset <<
			1, 0, offset_u,
			0, 1, offset_v,
			0, 0, 1;
		big_img.Transform_i(offset);
		big_img.BoundingBox_i(min_uv, max_uv);
		// copy to opencv
		cv::Mat assembled_img(max_uv.u + 1, max_uv.v + 1, CV_8UC3);
		for (int u = 0; u < max_uv.u + 1; ++u)
			for (int v = 0; v < max_uv.v + 1; ++v)
			{
				cv::Vec3b intensity;
				intensity.val[0] = 0;
				intensity.val[1] = 0;
				intensity.val[2] = 0;
				assembled_img.at<cv::Vec3b>(u, v) = intensity;
			}

		for (int i = 0; i < big_img.pixels_.size(); ++i)
		{
			int u = big_img.pixels_[i].uv.u;
			int v = big_img.pixels_[i].uv.v;
			cv::Vec3b intensity;
			intensity.val[0] = big_img.pixels_[i].b;
			intensity.val[1] = big_img.pixels_[i].g;
			intensity.val[2] = big_img.pixels_[i].r;
			assembled_img.at<cv::Vec3b>(u, v) = intensity;
		}
		assembledImg_ = assembled_img;

		std::cout << "Done!\n";
	}


	void JigsawVisualPair::AssembleLoop(const std::vector<int>& loop, uchar r, uchar g, uchar b)
	{
		std::cout << "Assembling...";
		// init
		std::vector<DummyImage> dummy_images;
		for (int i = 0; i < loop.size(); ++i)
			dummy_images.push_back(DummyImage(croppedImgArr_[loop[i]], r, g, b));

		int total_pixel = 0;
		std::vector<std::unordered_map<UV, int, hash_big, key_equal>> mapUV2IdArray(dummy_images.size());
		for (int ix = 0; ix < dummy_images.size(); ++ix)
		{
			for (int i = 0; i < dummy_images[ix].pixels_.size(); ++i)
			{
				UV uv = dummy_images[ix].pixels_[i].uv;
				mapUV2IdArray[ix][uv] = i;
			}
			total_pixel += dummy_images[ix].pixels_.size();
		}

		// transform
		std::vector<Eigen::Matrix3d, Eigen::aligned_allocator<Eigen::Matrix3d>> transform_array;

		for (int i = 0; i < loop.size(); ++i)		//	loop.size() == dummy_images.size()
		{
			dummy_images[i].Transform_f(cropTranslation_.data_[loop[i]].transformation_);	// first move to original position to eliminate cropped influence
			transform_array.push_back(cropTranslation_.data_[loop[i]].transformation_);
		}

		Alignments2d dummy_pose;
		for (int i = 0; i < frameNum_; ++i)
			dummy_pose.data_.push_back(FramedTransformation2d(i, i, 0.0, Eigen::Matrix3d::Identity()));

		for (int i = 0; i < loop.size() - 1; ++i)					// then transform
		{
			IdPair idpair(loop[i], loop[i + 1]);
			Eigen::Matrix3d tran;
			if (mapIdPair2EdgeIds_.find(idpair) == mapIdPair2EdgeIds_.end())
			{
				IdPair idpair_reverse(loop[i + 1], loop[i]);
				if (mapIdPair2EdgeIds_.find(idpair_reverse) == mapIdPair2EdgeIds_.end())
				{
					std::cout << "cannot find loop id\n";
					exit(-1);
				}
				tran = relative_.data_[mapIdPair2EdgeIds_[idpair_reverse].edgeIds[0]].transformation_.inverse();
			}
			else
				tran = relative_.data_[mapIdPair2EdgeIds_[idpair].edgeIds[0]].transformation_;
			if (i == 0)
			{
				dummy_images[0].Transform_f(Eigen::Matrix3d::Identity());
				dummy_images[1].Transform_f(tran);
				dummy_pose.data_[0].transformation_ = Eigen::Matrix3d::Identity();
				dummy_pose.data_[1].transformation_ = tran;

				transform_array[0] = Eigen::Matrix3d::Identity()*transform_array[0];
				transform_array[1] = tran*transform_array[1];
			}
			else {
				dummy_images[i + 1].Transform_f(dummy_pose.data_[i].transformation_*tran);
				dummy_pose.data_[i + 1].transformation_ = dummy_pose.data_[i].transformation_*tran;

				transform_array[i + 1] = dummy_pose.data_[i].transformation_*tran * transform_array[i + 1];
			}
		}

		// assemble
		DummyImage big_img;
		big_img.pixels_.reserve(total_pixel);
		for (int ix = 0; ix < dummy_images.size(); ++ix)
		{
			auto& transformed_img = dummy_images[ix];

			UV min_uv, max_uv;
			transformed_img.BoundingBox_f(min_uv, max_uv);

			for (int u = min_uv.u; u < max_uv.u; ++u)
			{
				for (int v = min_uv.v; v < max_uv.v; ++v)
				{
					UV query_uv(u, v);
					Eigen::Matrix3d t = transform_array[ix].inverse();
					query_uv.Transform(t);
					if (mapUV2IdArray[ix].find(query_uv) == mapUV2IdArray[ix].end())
						continue;
					auto pixel_index = mapUV2IdArray[ix][query_uv];
					Pixel new_pixel = dummy_images[ix].pixels_[pixel_index];
					new_pixel.uv.u = u;	new_pixel.uv.v = v;
					big_img.pixels_.push_back(new_pixel);
				}
			}
		}


		UV min_uv, max_uv;
		big_img.BoundingBox_i(min_uv, max_uv);
		int offset_u = -min_uv.u;
		int offset_v = -min_uv.v;
		Eigen::Matrix3d offset;
		offset <<
			1, 0, offset_u,
			0, 1, offset_v,
			0, 0, 1;
		big_img.Transform_i(offset);
		big_img.BoundingBox_i(min_uv, max_uv);
		// copy to opencv
		cv::Mat assembled_img(max_uv.u + 1, max_uv.v + 1, CV_8UC3);
		for (int u = 0; u < max_uv.u + 1; ++u)
			for (int v = 0; v < max_uv.v + 1; ++v)
			{
				cv::Vec3b intensity;
				intensity.val[0] = 0;
				intensity.val[1] = 0;
				intensity.val[2] = 0;
				assembled_img.at<cv::Vec3b>(u, v) = intensity;
			}

		for (int i = 0; i < big_img.pixels_.size(); ++i)
		{
			int u = big_img.pixels_[i].uv.u;
			int v = big_img.pixels_[i].uv.v;
			cv::Vec3b intensity;
			intensity.val[0] = big_img.pixels_[i].b;
			intensity.val[1] = big_img.pixels_[i].g;
			intensity.val[2] = big_img.pixels_[i].r;
			assembled_img.at<cv::Vec3b>(u, v) = intensity;
		}
		assembledImg_ = assembled_img;

		std::cout << "Done!\n";
	}
}