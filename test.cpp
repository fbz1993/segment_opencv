#include <iostream>
#include <opencv2/opencv.hpp>
#include "segment_opencv.h"
#include <utility/utility_for_opencv.h>

int main(int argc, char** argv)
{
	if(argc < 2){
		std::cerr << "arg1: imgname" << std::endl;
		return 1;
	}
	cv::Mat src_img = cv::imread(argv[1], 1);
	cv::Mat dst_img;
	int n;
	segment_image(src_img, 0.5, 500, 50, &n, dst_img);
	n = quantize<int>(dst_img, dst_img);
	
	cv::RNG rng(1);
	std::vector<cv::Vec3b> colors(n);
	for(auto& color : colors){
		color[0] = rng.uniform(0,255);
		color[1] = rng.uniform(0,255);
		color[2] = rng.uniform(0,255);
	}
	cv::Mat dsp_img(dst_img.size(), CV_8UC3);
	dst_img.forEach<int>([&](int idx, const int* p){
		dsp_img.at<cv::Vec3b>(p) = colors[idx];
	});
	
	cv::imshow("result", dsp_img);
	cv::waitKey(0);
	return 0;
}