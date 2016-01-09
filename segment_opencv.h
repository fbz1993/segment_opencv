/**
 * @file  segment_opencv.h
 * @brief Wrapper of Felzenszwalb's graph-based segmentation for OpenCV
 */

#ifndef CV_SEGMENT_H
#define CV_SEGMENT_H

#include <opencv2/opencv.hpp>

/**
 * @brief Graph-based segmentation.
 * @param src_img Input image is CV_8UC3.
 * @param sigma,c,min_size Parameters of segmentation.
 * @param num_ncc The number of output segments.
 * @param dst_img Output image is CV_32SC1. dst_img.at<int>(i) is index of segment of pixel i.
 */
void segment_image(const cv::Mat& src_img, float sigma, float c, int min_size, int *num_ccs, cv::Mat& dst_img);

#endif