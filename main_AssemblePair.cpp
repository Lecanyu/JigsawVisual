//#include "Utils.h"
//#include "JigsawVisualPair.h"
//#include "cmdline.h"
//
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iterator>
//#include <ctime>
//
//int main(int argc, char**argv)
//{
//	int frame_num;
//	std::string image_dir, relative_file, background_color;
//	int pose_file_type;
//
//	/*cmdline::parser arg_parser;
//	arg_parser.add<int>("frame_num", 'n', "total frame number", true);
//	arg_parser.add<std::string>("image_dir", 'd', "image directory", true, "");
//	arg_parser.add<std::string>("pose_file", 'p', "pose file", true, "");
//	arg_parser.add<int>("pose_file_type", 't', "pose file type (0: matrix, 1: line)", true, 0);
//	arg_parser.add<std::string>("background", 'b', "background color (RGB 0-255)", false, "0 0 0");
//
//	arg_parser.parse_check(argc, argv);
//
//	frame_num = arg_parser.get<int>("frame_num");
//	image_dir = arg_parser.get<std::string>("image_dir");
//	relative_file = arg_parser.get<std::string>("pose_file");
//	pose_file_type = arg_parser.get<int>("pose_file_type");
//	background_color = arg_parser.get<std::string>("background");*/
//
//	frame_num = 30;
//	image_dir = "C:/Users/range/Dropbox/JigsawGame/Chirstmas_Tree_Light/";
//	background_color = "0 0 0";
//	relative_file = "C:/Users/range/Dropbox/JigsawGame/Chirstmas_Tree_Light/matching_global2.tr";
//	pose_file_type = 1;
//	/*relative_file = "C:/Users/range/Dropbox/JigsawGame/Chirstmas_Tree_Light/matching_global2.tr";
//	pose_file_type = 1;*/
//
//	int v1, v2;
//	std::cout << "Input two id for visualization\n";
//	std::cin >> v1 >> v2;
//	int No = 0;
//	if (pose_file_type){
//		std::cout << "look gt pair please input No between " << v1 << "---" << v2 << "\n";
//		std::cin >> No;
//	}
//
//	std::istringstream iss(background_color);
//	std::vector<std::string> results(std::istream_iterator< std::string > {iss},
//		std::istream_iterator<std::string>());
//
//	uchar r = atoi(results[0].c_str());
//	uchar g = atoi(results[1].c_str());
//	uchar b = atoi(results[2].c_str());
//	JigsawVisualPair visual(frame_num, image_dir, relative_file, pose_file_type);
//	int start_s = clock();
//	visual.CropImages(r, g, b);
//	visual.AssemblePair(v1, v2, r, g, b, No);
//	int stop_s = clock();
//	std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << " ms" << std::endl;
//
//	cv::imshow("image", visual.assembledImg_);
//	cv::waitKey();
//	cv::imwrite("Assembled.png", visual.assembledImg_);
//	std::cout << "Save to Assembled.png\n";
//
//	return 0;
//}
