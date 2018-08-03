//////////////////////////////////////////////////////////////////////////
// Visualize a batch of images according to relative transformation
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Core>
#include <Eigen/Eigen>

#include "Utils.h"

namespace JigsawVisualNS
{
	class JigsawVisualPair
	{
	public:
		int frameNum_;
		Alignments2d relative_;
		Alignments2d cropTranslation_;
		std::vector<cv::Mat> originalImgArr_;
		std::vector<cv::Mat> croppedImgArr_;
		cv::Mat assembledImg_;
		std::unordered_map<IdPair, MultiEdgeId, hash_func_IdPair, key_equal_IdPair> mapIdPair2EdgeIds_;

		JigsawVisualPair(int frame_num, std::string root_dir, std::string relative_file, bool isLine);
		JigsawVisualPair(int frame_num, std::string root_dir, Alignments2d& alignment);

		// Crop out bounding box in original images 
		void CropImages(uchar r, uchar g, uchar b);
		// Assemble one pair fragments
		void AssemblePair(int v1, int v2, uchar r, uchar g, uchar b, int multi_id = 0);
		// Assemble loop
		void AssembleLoop(const std::vector<int>& loop, uchar r, uchar g, uchar b);

	private:
		// according to background color to find bounding box
		void FindBoundingBox(const cv::Mat& img, uchar r, uchar g, uchar b, UV& out_min_uv, UV& out_max_uv);
	};
}