#ifndef CV_SEGMENT_H
#define CV_SEGMENT_H

#include <image.h>
#include <misc.h>
#include <opencv2/opencv.hpp>

// Mat -> image<rgb>*
image<rgb>* convertfromMat(const cv::Mat& src_img);

void segment_image(const cv::Mat& src_img, float sigma, float c, int min_size, int *num_ccs, cv::Mat& dst_img);

#endif