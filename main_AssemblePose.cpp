#include "Utils.h"
#include "JigsawVisual.h"
#include "cmdline.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iterator>

int main(int argc, char**argv)
{
	std::string image_dir, pose_file, background_color, savefile;
	int pose_file_type;

	//original cmd
	/*if(argc!=6)
	{
		std::cout << "Parameters error!\n";
		return -1;
	}
	image_dir = argv[1];
	background_color = argv[4];
	pose_file = argv[2];
	pose_file_type = atoi(argv[3]);
	savefile = argv[5];*/

	/*cmdline::parser arg_parser;
	arg_parser.add<std::string>("image_dir", 'd', "image directory", true, "");
	arg_parser.add<std::string>("pose_file", 'p', "pose file", true, "");
	arg_parser.add<int>("pose_file_type", 't', "pose file type (0: matrix, 1: line)", true, 0);
	arg_parser.add<std::string>("background", 'b', "background color (RGB 0-255)", false, "0 0 0");

	arg_parser.parse_check(argc, argv);

	image_dir = arg_parser.get<std::string>("image_dir");
	pose_file = arg_parser.get<std::string>("pose_file");
	pose_file_type = arg_parser.get<int>("pose_file_type");
	background_color = arg_parser.get<std::string>("background");*/

	image_dir = "../../example/";
	background_color = "8 8 248";
	/*pose_file = "C:/Users/range/Desktop/jigsaw_pose.txt";
	pose_file_type = 0;*/
	pose_file = "../../example/matrix_pose.txt";
	pose_file_type = 0;
	savefile = "../../example/assemble.png";

	std::istringstream iss(background_color);
	std::vector<std::string> results(std::istream_iterator < std::string > {iss},
		std::istream_iterator<std::string>());

	uchar r = atoi(results[0].c_str());
	uchar g = atoi(results[1].c_str());
	uchar b = atoi(results[2].c_str());
	JigsawVisualNS::JigsawVisual visual(image_dir, pose_file, pose_file_type);
	visual.CropImages(r, g, b);
	visual.AssembleAllImages(r, g, b);

	cv::imshow("image", visual.assembledImg_);
	cv::waitKey();
	cv::imwrite(savefile, visual.assembledImg_);
	std::cout << "Save to Assembled.png\n";

	return 0;
}
