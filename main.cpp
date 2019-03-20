#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "Matrix.h"

using namespace cv;
using namespace std;

int width = 48;
int height = 48;
void recognize(Matrix Y, Matrix W );
Matrix processImage(String);
void getScreens(String);
bool isSkinPixel(Mat im, int x, int y);
Matrix newTrain(Matrix* matrixs, int count);
Matrix F(const Matrix& matrix);

Matrix multiple(const Matrix &m1, const Matrix &m2)
{
	Matrix result(m1.getRows(), m2.getColumns());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m2.getColumns(); j++)
			for (int k = 0; k < m1.getColumns(); k++)
				(*((result[i]) + j)) = (*(m1[i] + k)) * (*(m2[k] + j));
	return result;
}

int main(int argc, char** argv)
{ 	
	//int i =0;	
	//getScreens("qwerty");
   // cout <<  processImage("qwerty0.JPG"); 
	 Matrix matrixs[2];
     matrixs[0] = processImage("ok0.JPG");  
     //matrixs[1] = processImage("ok1.JPG"); 
	// matrixs[2] = processImage("ok2.JPG");
	// matrixs[3] = processImage("ok3.JPG");
	 matrixs[1] = processImage("victory0.JPG");
		//getScreens("newtest");
	
	Matrix W = newTrain(matrixs, 2);
    recognize(processImage("ok9.JPG"), W);
	 recognize(processImage("test3.JPG"), W);
	  recognize(processImage("victory7.JPG"), W);	


	cin.get();	
	return 0;
}

Matrix newTrain(Matrix* matrixs, int count) {
	int N = height*width;
	Matrix  W = Matrix(N, N);	
	for (int i = 0; i < count; i++)   
		W = sum(W,  multipleNumber(((float)(1.0 / (float)N)), multiple((matrixs[i].reshape(N, 1)), ((matrixs[i].reshape(N, 1)).T()))));	
	return W;
}

void recognize(Matrix Y, Matrix W ){	
	cout << "\n image before recognize:\n" << Y.reshape(height, width);	
	Y = Y.reshape(height*width, 1);//cout << signMatrix(Q.reshape(height, width));
	cout << "\nrecognized image:\n" << F((W*Y).reshape(height, width));	
}

Matrix F(const Matrix& matrix) { 
	Matrix result(matrix.getRows(), matrix.getColumns());
	for (int i = 0; i < matrix.getRows(); i++)
		for (int j = 0; j < matrix.getColumns(); j++) {
			if (matrix[i][j] > 0)
				result[i][j] = 1;
			else if (matrix[i][j] < 0)
				result[i][j] = -1;
		}
		return result;
}

bool isSkinSquare(int startRow, int startCol, Mat im){
	int skinPixels = 0;
	for(int i = startRow; i < startRow + 10; i++){
		for(int j = startCol; j < startCol + 10; j++)
		{
			if(isSkinPixel(im, i, j))
				skinPixels++;
		}
	}
	if(skinPixels > 70)
		return true;

	return false;
}

typedef struct RGB {
	float r;       // a fraction between 0 and 1
	float g;       // a fraction between 0 and 1
	float b;       // a fraction between 0 and 1
} rgb;

typedef struct HSV {
	float h;       // angle in degrees
	float s;       // a fraction between 0 and 1
	float v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
	hsv         out;
	float      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min  : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max  : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	} else {
		// if max is 0, then r = g = b = 0              
		// s = 0, h is undefined
		out.s = 0.0;
		out.h = NULL;                            // its now undefined
		return out;
	}
	if( in.r >= max )                           // > is bogus, just keeps compilor happy
		out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
	else
		if( in.g >= max )
			out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + ( in.r - in.g ) / delta; 

	out.h *= 60.0;                           

	if( out.h < 0.0 )
		out.h += 360.0;
	return out;
}


rgb hsv2rgb(hsv in)
{
	float      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;     
}



bool  isSkinPixel(Mat im, int x, int y)
{
	float B = 0;
	float G = 0;
	float R = 0;
	float A = 0;
	float H = 0;
	float S = 0;
	float V = 0;
	float Y = 0;
	float Cr = 0;
	float Cb = 0;
	G = float(im.at<cv::Vec4b>(x,y)[1]);
	R = float(im.at<cv::Vec4b>(x,y)[2]);
	B = float(im.at<cv::Vec4b>(x,y)[3]);
	A = float(im.at<cv::Vec4b>(x,y)[0]);
	//Y = 0.299 * R + 0.287 * G + 0.11  * B;
	Y = 0.375* R + 0.5* G + 0.125* B;
	Cr = R - Y;
	Cb = B - Y;
	RGB rgb;
	rgb.r = R;
	rgb.g = G;
	rgb.b = B;
	HSV hsv = rgb2hsv(rgb);
	H = hsv.h;
	S = hsv.s;
	V = hsv.v;

	if((R > 95 && G > 40 && B > 20 && R > G && R > B
		&& (fabs(R - G) > 15) && A > 15 && Cr > 70 && Cb > 40 && Y > 80
		&&( Cr <= (1.5862*Cb) + 20) && (Cr >= (0.3448*Cb) + 46.2069) &&
		(Cr >= (-4.5652*Cb) + 234.5652) && (Cr <= (-1.15*Cb) + 301.75)
		&& (Cr <= (-2.2857*Cb) + 432.85))|| 
		(H >= 0.0  && S >= 0.05 && S <= 0.68
		&& R > 95.0 && G > 40.0 && B > 20.0 && R > G && R > B && (fabs(R - G) > 15) && A > 15 ))
		return true;
	else
		return false;
}

void getScreens(String nameScreen) {
	VideoCapture cap;    
	if(!cap.open(0))
		return;

	cap.set(CV_CAP_PROP_FRAME_WIDTH,480);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	int count = 0;
	for(;;) {	
		Mat frame;
		cap >> frame;
		if( frame.empty() ) break; 
		imshow("this is you, smile! :)", frame);
		if( waitKey(10) == 27 ) {	
			stringstream ss;
			ss << count;
			count++;
			string str = ss.str();
			String name = nameScreen + str;
			cv::imwrite(name +  ".JPG", frame); 			
		}
	}   
}

Matrix processImage(String fileP)
{
	Matrix result = Matrix(height, width);
	Mat bgr  =  imread ( fileP,  CV_LOAD_IMAGE_COLOR );
	Mat res;
	pyrMeanShiftFiltering( bgr, res, 12, 12, 3);
	cv::Mat bgra;

	cv::cvtColor(res, bgra, cv::COLOR_BGR2BGRA);
	cv::Mat abgr(bgra.size(), bgra.type());
	int from_to[] = { 0,3, 1,1, 2,2, 3,0 };
	cv::mixChannels(&bgra,1,&abgr,1,from_to,4);
	Mat im = abgr;
	Mat hsv;
	cvtColor(imread ( fileP,  CV_LOAD_IMAGE_COLOR ), hsv, CV_BGR2HSV);

	char imMatrix[48][48];
	for(int i = 0; i < im.rows; i+=10)
		for(int j = 0; j < im.cols; j+=10)
		{	

			if(isSkinSquare(i, j, im) == true)
			{
				result[i/10][j/10] = 1;
			}
			else {
				result[i/10][j/10] = -1;
			}
		}
		return result;
}
