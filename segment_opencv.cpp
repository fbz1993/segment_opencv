#include <cstdio>
#include <cstdlib>
#include <image.h>
#include <misc.h>
#include <pnmfile.h>
#include <segment-image.h>

#include <opencv2/opencv.hpp>
#include "segment_opencv.h"


// Mat -> image<rgb>*
image<rgb>* convertfromMat(const cv::Mat& src_img){
	image<rgb>* im = new image<rgb>(src_img.cols, src_img.rows);
	for(int y=0; y<src_img.rows; ++y){
		for(int x=0; x<src_img.cols; ++x){
			const cv::Vec3b& vec = src_img.at<cv::Vec3b>(y,x);
			rgb* ptr = imPtr(im,x,y);
			ptr->r = vec[2];
			ptr->g = vec[1];
			ptr->b = vec[0];
		}
	}
	return im;
}

void segment_image(const cv::Mat& src_img, float sigma, float c, int min_size, int *num_ccs, cv::Mat& dst_img)
{
	image<rgb>* im = convertfromMat(src_img);
	int width = im->width();
	int height = im->height();
	
	image<float> *r = new image<float>(width, height);
	image<float> *g = new image<float>(width, height);
	image<float> *b = new image<float>(width, height);
	
	// smooth each color channel  
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			imRef(r, x, y) = imRef(im, x, y).r;
			imRef(g, x, y) = imRef(im, x, y).g;
			imRef(b, x, y) = imRef(im, x, y).b;
		}
	}
	image<float> *smooth_r = smooth(r, sigma);
	image<float> *smooth_g = smooth(g, sigma);
	image<float> *smooth_b = smooth(b, sigma);
	delete r;
	delete g;
	delete b;
	
	// build graph
	edge *edges = new edge[width*height*4];
	int num = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x < width-1) {
				edges[num].a = y * width + x;
				edges[num].b = y * width + (x+1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
				num++;
			}
			
			if (y < height-1) {
				edges[num].a = y * width + x;
				edges[num].b = (y+1) * width + x;
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
				num++;
			}
			
			if ((x < width-1) && (y < height-1)) {
				edges[num].a = y * width + x;
				edges[num].b = (y+1) * width + (x+1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
				num++;
			}
			
			if ((x < width-1) && (y > 0)) {
				edges[num].a = y * width + x;
				edges[num].b = (y-1) * width + (x+1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
				num++;
			}
		}
	}
	delete smooth_r;
	delete smooth_g;
	delete smooth_b;
	
	// segment
	universe *u = segment_graph(width*height, num, edges, c);
	
	// post process small components
	for (int i = 0; i < num; i++) {
		int a = u->find(edges[i].a);
		int b = u->find(edges[i].b);
		if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
			u->join(a, b);
	}
	delete [] edges;
	*num_ccs = u->num_sets();
		
	dst_img = cv::Mat(src_img.size(), CV_32SC1);
	
//	std::map<int,int> key_values;
	int idx = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dst_img.at<int>(y,x) = u->find(y * width + x);
		}
	}
	delete u;
}

