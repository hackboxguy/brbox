#ifndef __IMGIDENTIFY_H_
#define __IMGIDENTIFY_H_
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "ADCommon.hpp"
#include "SmarteyeJsonDef.h"
using namespace std;
using namespace cv;
#define MIN_WIDTH_THRESHOLD 100 //ignore squares with too small width
#define MAX_SQARES_FILTER   10 //when too many contours are found, then increase the threshold and scan again.
#define MIN_SCAN_THRESHOLD  100 //start with 100, can go upto 200
/*---------------------------------------------------------------------------*/
class  ImgIdentify
{
	double angle(Point pt1, Point pt2, Point pt0);
	int find_relative_square(const vector<vector <Point> >& squares, vector<Point> biggest_square,vector<Point>& relative_square,int scale_factor,int tolerence);
	int find_relative_square_new(const vector<vector <Point> >& squares, vector<Point> biggest_square,vector<Point>& relative_square,float scale_W,float scale_H,float tolerence);

	int find_largest_square(const vector<vector <Point> >& squares, vector<Point>& biggest_square,float ratioLow,float ratioHigh);
	int apply_threshold(Mat& src_gray,int trsh,RNG &rng,Mat& drawing, vector<vector<Point> > &contours);
	int scale_image(Mat& image, const float imgScaleFactor);
	RPC_SRV_RESULT capture_camera_image(int video_device,int frmPixels,int frmLines,Mat& image);
	RPC_SRV_RESULT identify_image_box(/*bool logImage=false,*/std::string imgPath="");//check identify-pattern-box exists
	int identifyBlackPanel(std::string &baseImagePath, string &testImagePath, int thresholdPixelCount, int minThresholdForBaseImage, int maxThresholdForBaseImage, int minThresFactorForTestImage, bool debug = false);

public:
	bool logflag;
	 ImgIdentify();
	~ ImgIdentify();
	RPC_SRV_RESULT IdentifyPattern(EJSON_SMARTEYE_IFACE_TYPE pattern,std::string imgPath,std::string chkWallFile="",std::string chkWallBaseFile="");
	RPC_SRV_RESULT identify_color_box(std::string imgPath,EJSON_SMARTEYE_IFACE_TYPE pattern);
	RPC_SRV_RESULT capture_jpg_image(std::string imgPath,int resPix=1920,int resLines=1080);
	RPC_SRV_RESULT identify_if_all_tiles_on(std::string inputFile,std::string baseMaskFile);
	RPC_SRV_RESULT scan_qr_string(std::string filepath,std::string &qrstring);
	RPC_SRV_RESULT compare_images(std::string filepath1,std::string filepath2,std::string &result);
};
/*---------------------------------------------------------------------------*/
#endif
