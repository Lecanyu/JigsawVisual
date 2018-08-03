//////////////////////////////////////////////////////////////////////////
// Visualize assembled images according to pose
#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Utils.h"

namespace JigsawVisualNS
{
	class JigsawVisual
	{
	public:
		Alignments2d pose_;
		Alignments2d cropTranslation_;
		std::vector<cv::Mat> originalImgArr_;
		std::vector<cv::Mat> croppedImgArr_;
		cv::Mat assembledImg_;

		JigsawVisual(std::string root_dir, std::string pose_file, bool isLine);

		// Crop out bounding box in original images 
		void CropImages(uchar r, uchar g, uchar b);
		// Assemble all image fragments and ignore background color 
		void AssembleAllImages(uchar r, uchar g, uchar b);

	private:
		// according to background color to find bounding box
		void FindBoundingBox(const cv::Mat& img, uchar r, uchar g, uchar b, UV& out_min_uv, UV& out_max_uv);

		void FillHole(cv::Mat& hole_img, int ring);
		// k-ring avg
		void KRingAvg(const cv::Mat& hole_img, int u, int v, int k, uchar& out_r, uchar& out_g, uchar& out_b);
	};
}
