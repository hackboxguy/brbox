#ifndef __IMGIDENTIFY_H_
#define __IMGIDENTIFY_H_
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "ADCommon.hpp"
#include "SmarteyeJsonDef.h"
using namespace std;
using namespace cv;
#define MIN_WIDTH_THRESHOLD 50 //ignore squares with too small width
#define MAX_SQARES_FILTER   20 //when too many contours are found, then increase the threshold and scan again.
#define MIN_SCAN_THRESHOLD  100 //start with 100, can go upto 200
/*---------------------------------------------------------------------------*/
class  ImgIdentify
{
	double angle(Point pt1, Point pt2, Point pt0);
	int find_relative_square(const vector<vector <Point> >& squares, vector<Point> biggest_square,vector<Point>& relative_square,int scale_factor,int tolerence);
	int find_largest_square(const vector<vector <Point> >& squares, vector<Point>& biggest_square);
	int apply_threshold(Mat& src_gray,int trsh,RNG &rng,Mat& drawing, vector<vector<Point> > &contours);
	int scale_image(Mat& image, const float imgScaleFactor);
	RPC_SRV_RESULT capture_camera_image(int video_device,int frmPixels,int frmLines,Mat& image);
	RPC_SRV_RESULT identify_image_box(/*bool logImage=false,*/std::string imgPath="");//check identify-pattern-box exists
public:
	bool logflag;
	 ImgIdentify();
	~ ImgIdentify();
	RPC_SRV_RESULT IdentifyPattern(EJSON_SMARTEYE_IFACE_TYPE pattern,std::string imgPath);

};
/*---------------------------------------------------------------------------*/
#endif
