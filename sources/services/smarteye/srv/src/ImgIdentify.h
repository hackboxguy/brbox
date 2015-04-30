#ifndef __IMGIDENTIFY_H_
#define __IMGIDENTIFY_H_
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "ADCommon.hpp"
using namespace std;
using namespace cv;
#define MIN_WIDTH_THRESHOLD 50 //ignore squares with too small width
/*---------------------------------------------------------------------------*/
class  ImgIdentify
{
	double angle(Point pt1, Point pt2, Point pt0);
	int find_relative_square(const vector<vector <Point> >& squares, vector<Point> biggest_square,vector<Point>& relative_square,int scale_factor,int tolerence);
	int find_largest_square(const vector<vector <Point> >& squares, vector<Point>& biggest_square);
	int apply_threshold(Mat& src_gray,int trsh,RNG &rng,Mat& drawing, vector<vector<Point> > &contours);
	int scale_image(Mat& image, const float imgScaleFactor);
	RPC_SRV_RESULT capture_camera_image(int video_device,int frmPixels,int frmLines,Mat& image);

public:
	 ImgIdentify();
	~ ImgIdentify();
	RPC_SRV_RESULT identify_image();
};
/*---------------------------------------------------------------------------*/
#endif
