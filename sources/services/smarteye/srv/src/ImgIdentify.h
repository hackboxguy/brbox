//a program to automatically detect if display is displaying identify-pattern, if not, this could be black image or backlight off problem
//use this utility in automated testing of display power cycle using logitech C920 camera along with raspi or any other linux machine
//original code:
//http://stackoverflow.com/questions/8667818/opencv-c-obj-c-detecting-a-sheet-of-paper-square-detection
//http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
//compiled by: albert.david@barco.com
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#define MAX_SQARES_FILTER   20 //when too many contours are found, then increase the threshold and scan again.
#define MIN_SCAN_THRESHOLD  100
#define MIN_WIDTH_THRESHOLD 50 //ignore squares with too small width
/*---------------------------------------------------------------------------*/
double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
/*---------------------------------------------------------------------------*/
//finds the smaller square-box with reference to biggest_square, the relation is given by the multifying factor "scale_factor"
//in ovd kvd: outer rectangle in identify pattern is 3 times bigger than inner rectangle, hence scale factor to be used is 3
int find_relative_square(const vector<vector <Point> >& squares, vector<Point> biggest_square,vector<Point>& relative_square,int scale_factor,int tolerence) 
{
	if (!squares.size()) 
		return -1;
	int max_width = 0;
	int max_height = 0;
	int max_square_idx = 0;
	const int n_points = 4;

	Rect relative_rect = boundingRect(Mat(biggest_square));
	relative_rect.width/=scale_factor;
	relative_rect.height/=scale_factor;
	for (size_t i = 0; i < squares.size(); i++) 
	{
		Rect rectangle = boundingRect(Mat(squares[i]));
		if(rectangle.width<MIN_WIDTH_THRESHOLD)//ignore too small squares
			continue;

		//check if small rect fits within the tolerence window
		if( (rectangle.width >= relative_rect.width-tolerence) && (rectangle.width <= relative_rect.width+tolerence) )
		{
			float tmp=tolerence*0.5;//fullHD H to W aspect ratio is 9/16=0.576(hence reduce the height-tolerance to half)
			int tolH=(int)tmp;
			if( (rectangle.height >= relative_rect.height-tolH) && (rectangle.height <= relative_rect.height+tolH) )
			{
				relative_square = squares[i];
				//cout<<"found smaller rect"<<endl;
				return 0;
			}
		}
	}
	return -1;
}
/*---------------------------------------------------------------------------*/
//finds the larges square from squares-list
int find_largest_square(const vector<vector <Point> >& squares, vector<Point>& biggest_square) 
{
	if (!squares.size()) 
		return -1;

	int max_width = 0;
	int max_height = 0;
	int max_square_idx = 0;
	const int n_points = 4;

	for (size_t i = 0; i < squares.size(); i++) 
	{
		Rect rectangle = boundingRect(Mat(squares[i]));
		if ((rectangle.width >= max_width) && (rectangle.height >= max_height)) 
		{
			max_width = rectangle.width;
			max_height = rectangle.height;
			max_square_idx = i;
		}
	}
	biggest_square = squares[max_square_idx];
	return 0;
}
/*---------------------------------------------------------------------------*/
int apply_threshold(Mat& src_gray,int trsh,RNG &rng,Mat& drawing, vector<vector<Point> > &contours)
{
	Mat threshold_output;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold( src_gray, threshold_output, trsh, 255, THRESH_BINARY );
	/// Find contours
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	for( int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
		minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
	}

	/// Draw polygonal contour + bonding rects + circles
	drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
		//circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
int main(int argc, char** argv )
{
	int result=-1;
	bool debugFlag=false;
	if(argc>1)
	{
		if(strcmp(argv[1],"debug")==0)
			debugFlag=true;
	}
	vector<vector<Point> > contours;
	Mat image, src_gray;

	//capture image frame from /dev/video0 v4l2 device
	VideoCapture capture(0);//0 indicates /dev/video0
	capture.set(CV_CAP_PROP_FRAME_WIDTH,1920);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
	if(!capture.isOpened())
		cout << "Failed to connect to the camera." << endl;
	//Mat frame, edges;
	capture >> image;
	if(image.empty())
	{
		cout << "Failed to capture an image" << endl;
		return -1;
	}

	//reduce size of the file if it is too big
	//const float imgScaleFactor =0.25;
	//cv::resize(image, image, cvSize(0, 0),imgScaleFactor,imgScaleFactor);


	/// Convert image to gray and blur it
	cvtColor( image, src_gray, CV_BGR2GRAY );
	blur( src_gray, src_gray, Size(3,3) );

	RNG rng(12345);
	Mat contour_image;
	apply_threshold(src_gray,MIN_SCAN_THRESHOLD,rng,contour_image,contours);//filter-out unwanted non-square stuff
	if(contours.size()>MAX_SQARES_FILTER)
	{
		apply_threshold(src_gray,MIN_SCAN_THRESHOLD+20,rng,contour_image,contours);//filter-out unwanted non-square stuff
		if(contours.size()>MAX_SQARES_FILTER)
		{
			apply_threshold(src_gray,MIN_SCAN_THRESHOLD+40,rng,contour_image,contours);//filter-out unwanted non-square stuff
			if(contours.size()>MAX_SQARES_FILTER)
			{
				apply_threshold(src_gray,MIN_SCAN_THRESHOLD+60,rng,contour_image,contours);//filter-out unwanted non-square stuff
				if(contours.size()>MAX_SQARES_FILTER)
					apply_threshold(src_gray,MIN_SCAN_THRESHOLD+80,rng,contour_image,contours);//filter-out unwanted non-square stuff
			}
			
		}
		
	}

	vector<Point> biggest_square;
	vector<Point> smaller_square;

	//find largest square
	find_largest_square(contours/*squares*/,biggest_square);//find outer white box of ovd/kvd identify pattern
	//find a smaller square which is 3times smaller than largest
	int res=find_relative_square(contours/*squares*/,biggest_square,smaller_square,3,20); //+/- 20pixel tolerence
	if(res==0)
		result = 0;
	else
		result=-1;

	if(debugFlag)
	{
		cout << "Total Sqr="<<contours.size()<<endl;
		if(contours.size()>0)
		{
			Rect Myrect = boundingRect(Mat(biggest_square));
			cout << "SqrBigW="<<Myrect.width<<" SqrBigH="<<Myrect.height<<endl;
		}
		if(res==0)
		{
			Rect Smallrect = boundingRect(Mat(smaller_square));
			cout << "SqrSmlW="<<Smallrect.width<<" SqrSmlH="<<Smallrect.height<<endl;
			cout << "Result: Success"<<endl;
		}
		else
			cout << "Result: Fail !!!!!!"<<endl;
		//show detected contours for manual inspection
		namedWindow("Display Image", WINDOW_AUTOSIZE );
		imshow("Display Image", contour_image);
		waitKey(0);
	}
	if(res==0)
		cout << "Result: Success"<<endl;
	else
		cout << "Result: Fail !!!!!!"<<endl;
	return result;
}
/*---------------------------------------------------------------------------*/

