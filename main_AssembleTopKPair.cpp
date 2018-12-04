/***
 * Visualize top-K highest score alignment 
 */
#include "Utils.h"
#include "JigsawVisualPair.h"
#include "cmdline.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iterator>

int main(int argc, char**argv)
{
	int K;
	int frame_num;
	std::string image_dir, relative_file, background_color;
	std::string save_dir;
	int pose_file_type;

	/*cmdline::parser arg_parser;
	arg_parser.add<int>("frame_num", 'n', "total frame number", true);
	arg_parser.add<std::string>("image_dir", 'd', "image directory", true, "");
	arg_parser.add<std::string>("relative_file", 'p', "pose file", true, "");
	arg_parser.add<int>("pose_file_type", 't', "pose file type (0: matrix, 1: line)", true, 0);
	arg_parser.add<std::string>("background", 'b', "background color (RGB 0-255)", false, "0 0 0");

	arg_parser.parse_check(argc, argv);

	frame_num = arg_parser.get<int>("frame_num");
	image_dir = arg_parser.get<std::string>("image_dir");
	relative_file = arg_parser.get<std::string>("relative_file");
	pose_file_type = arg_parser.get<int>("pose_file_type");
	background_color = arg_parser.get<std::string>("background");*/


	image_dir = "C:/Users/range/Dropbox/JigsawGame/Chirstmas_Tree_Light/";
	save_dir = "C:/Users/range/Desktop/topK/";
	background_color = "168 8 248";
	relative_file = "C:/Users/range/Dropbox/JigsawGame/Chirstmas_Tree_Light/matching_global2.tr";
	pose_file_type = 1;
	K = 50;
	frame_num = 30;


	JigsawVisualNS::Alignments2d alignment;
	if (pose_file_type == 1)
		alignment.LoadFromFileLine(relative_file);
	else
		alignment.LoadFromFile(relative_file);

	std::sort(alignment.data_.begin(), alignment.data_.end(), JigsawVisualNS::SortScore());
	alignment.data_.resize(K);

	std::istringstream iss(background_color);
	std::vector<std::string> results(std::istream_iterator< std::string > {iss},
		std::istream_iterator<std::string>());

	uchar r = atoi(results[0].c_str());
	uchar g = atoi(results[1].c_str());
	uchar b = atoi(results[2].c_str());
	JigsawVisualNS::JigsawVisualPair visual(frame_num, image_dir, alignment);
	visual.CropImages(r, g, b);

	for (int i=0;i<K;++i)
	{
		int v1 = alignment.data_[i].frame1_;
		int v2 = alignment.data_[i].frame2_;
		visual.AssemblePair(v1, v2, r, g, b);
		
		std::stringstream ss;
		ss << save_dir << v1 << "-" << v2 << ".png";
		cv::imwrite(ss.str(), visual.assembledImg_);
		std::cout << "Save to "<< ss.str() <<"\n";
	}

	return 0;
}
