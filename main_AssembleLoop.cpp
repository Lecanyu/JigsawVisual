//#include "Utils.h"
//#include "JigsawVisualPair.h"
//#include "cmdline.h"
//
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iterator>
//
//int main(int argc, char**argv)
//{
//	int frame_num;
//	std::string image_dir, relative_file, background_color;
//	int pose_file_type = 0;
//
//	/*cmdline::parser arg_parser;
//	arg_parser.add<int>("frame_num", 'n', "total frame number", true);
//	arg_parser.add<std::string>("image_dir", 'd', "image directory", true, "");
//	arg_parser.add<std::string>("pose_file", 'p', "pose file", true, "");
//	arg_parser.add<std::string>("background", 'b', "background color (RGB 0-255)", false, "0 0 0");
//
//	arg_parser.parse_check(argc, argv);
//
//	frame_num = arg_parser.get<int>("frame_num");
//	image_dir = arg_parser.get<std::string>("image_dir");
//	relative_file = arg_parser.get<std::string>("pose_file");
//	background_color = arg_parser.get<std::string>("background");*/
//
//	frame_num = 30;
//	image_dir = "C:/Users/range/Dropbox/JigsawGame/England/";
//	background_color = "8 8 248";
//	relative_file = "E:/Jigsaw/Tools/selected.txt";
//
//	std::cout << "Input loop length\n";
//	int length;
//	std::cin >> length;
//	std::cout << "Input loop vertex Id\n";
//	std::vector<int> loop(length, 0);
//	for (int i = 0; i < length; ++i)
//		std::cin >> loop[i];
//	std::istringstream iss(background_color);
//	std::vector<std::string> results(std::istream_iterator< std::string > {iss},
//		std::istream_iterator<std::string>());
//
//	uchar r = atoi(results[0].c_str());
//	uchar g = atoi(results[1].c_str());
//	uchar b = atoi(results[2].c_str());
//	JigsawVisualPair visual(frame_num, image_dir, relative_file, pose_file_type);
//	visual.CropImages(r, g, b);
//	visual.AssembleLoop(loop, r, g, b);
//
//	cv::imshow("image", visual.assembledImg_);
//	cv::waitKey();
//	cv::imwrite("Assembled.png", visual.assembledImg_);
//	std::cout << "Save to Assembled.png\n";
//
//	return 0;
//}
