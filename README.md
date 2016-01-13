# segment_opencv
OpenCV wrapper of graph-based segmentation (Felzenszwalb)

# Requirement
OpenCV 3.x

# Compile
% make -f Makefile.txt

# Usage
cv::Mat src_img = cv::imread(imgname, 1);
cv::Mat dst_img;
segment_image(src_img, 0.5, 500, 50, dst_img); // dst_img is CV_32SC. Each element is the index of segment.

