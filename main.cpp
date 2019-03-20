﻿#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "Matrix.h"

using namespace cv;
using namespace std;

int width = 48;
int height = 48;
Matrix processImage(String);
void getScreens(String);
void recognitionIm(Matrix Y, Matrix W );
bool isSkinPixel(Mat im, int x, int y);
Matrix train(Matrix* matrixs, int count);

void testNN(){
	Matrix matrixs[2];
	Matrix m1 = Matrix(4,4);
	Matrix m2 = Matrix(4,4);
	for(int i = 0; i < m1.getColumns(); i++)
		for(int j = 0; j < m1.getColumns(); j++)
		{
			m1[i][j] = -1;
			m2[i][j] = -1;
		}

		m1[0][3] = 1;
		m1[0][2] = 1;
		m1[0][1] = 1;
		m1[0][0] = 1;

		m2[3][3] = 1;
		m2[2][2] = 1;
		m2[1][1] = 1;
		m2[0][0] = 1;

		matrixs[0] = m1;
		matrixs[1] = m2;

		Matrix m11 = Matrix(4,4);
	Matrix m21 = Matrix(4,4);
	for(int i = 0; i < m11.getColumns(); i++)
		for(int j = 0; j < m11.getColumns(); j++)
		{
			m11[i][j] = -1;
			m21[i][j] = -1;
		}

		m11[0][3] = 1;
		m11[1][2] = 1;
		m11[0][1] = 1;
		m11[0][0] = 1;

		m21[3][3] = 1;
		m21[2][3] = 1;
		m21[2][0] = 1;
		m21[0][0] = 1;
		m21[1][0] = 1;
		m21[3][0] = 1;
	
//
	Matrix W = train(matrixs, 2);
	cout << "\nm1: \n" << m1;
	cout <<"\nbadm1: \n" << m11;
	recognitionIm(m11, W);
	cout << "\nm2: \n" << m2;
	cout <<"\nbadm2: \n" << m21;
	recognitionIm(m21, W);
}


int main(int argc, char** argv)
{
   // testNN();	
	//int i =0;	
	//getScreens("qwerty");
   // cout <<  processImage("qwerty0.JPG"); 
	 //Matrix matrixs[2];
    // matrixs[0] = processImage("ok0.JPG");  
   //  matrixs[1] = processImage("ok1.JPG");  
	/*matrixs[i++] = processImage("ok2.JPG");
	matrixs[i++] = processImage("ok3.JPG");
	matrixs[i++] = processImage("ok4.JPG"); 
	matrixs[i++] = processImage("ok5.JPG"); 
	matrixs[i++] = processImage("ok6.JPG");  
	matrixs[i++] = processImage("victory0.JPG");  
	matrixs[i++] = processImage("victory1.JPG");
	matrixs[i++] = processImage("victory2.JPG");
	matrixs[i++] = processImage("victory3.JPG");
	matrixs[i++] = processImage("victory4.JPG");*/
	//getScreens("newtest");
	cout << processImage("newtest0.JPG");
	//Matrix W = train(matrixs, 2);
  //  recognitionIm(processImage("ok9.JPG"), W);
	/*recognitionIm(processImage("ok8.JPG"), W);
	recognitionIm(processImage("ok9.JPG"), W);
	recognitionIm(processImage("victory5.JPG"), W);
	recognitionIm(processImage("victory6.JPG"), W);
	recognitionIm(processImage("victory7.JPG"), W);*/

	cin.get();
	cin.get();
	cin.get();
	cin.get();
	return 0;
}

Matrix train(Matrix* matrixs, int count) {
	float h = 0.8; // 0.7..0.9
	int N = height*width;
    Matrix  W = Matrix(N, N);
    
	Matrix prevW;
	int cnt = 0;
	do {
		cout << "\ntrain stage: " << cnt;
		cnt++;
		for (int i = 0; i < 2; i++) {
		  {	
			Matrix Xi = matrixs[i].reshape(height*width, 1);		
			W = W + ((h / N) * (Xi - W * Xi) * Xi.T()); 
		  }
		}
	   {	if (isDiff(W, prevW, 0.0001)) {			
			std::cout << cnt << std::endl;
			_getch();
			break;
		}
	   }
		prevW = W;
	} while (true);
	for (int i = 0; i < W.getRows(); i++)
		W[i][i] = 0;

	return W;
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

void recognitionIm(Matrix Y, Matrix W ){
	cout << "\nim to recognize:\n" << Y;
	int iteration = 1000;
	Matrix Q;	
	int count = 0;
	Matrix prevQ;
	Y = Y.reshape(height*width, 1);
	do {
		std::cout << "Iteration #" << count << std::endl;
		Q = F(W*Y); 
		if (Y == Q || prevQ == Q) {
			cout << "\nNN worked good, answear: \n";
			cout << signMatrix(Q.reshape(height, width));
			return;
		}
		else {
			prevQ = Y;
			Y = Q;
		}
		count++;
	} while (count < iteration);
	cout << "\nNN worked bad, answear: \n";
	cout << signMatrix(Q.reshape(height, width));
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
	float B = 0;//abgr
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

void getSkinColors(){
	Mat im   =  imread ( "test11.JPG",  CV_LOAD_IMAGE_COLOR );
	float sumG = 0;
	float sumR = 0;
	std::vector<float> gVect;
	std::vector<float> rVect;

	for(int i = 0; i < im.rows; i++)
		for(int j = 0; j < im.cols; j++)
		{
			float sum = float(im.at<cv::Vec3b>(i,j)[0]) + float(im.at<cv::Vec3b>(i,j)[1])  + float(im.at<cv::Vec3b>(i,j)[2]);
			sumG += (float(im.at<cv::Vec3b>(i,j)[1]) / sum);
			gVect.push_back(float(im.at<cv::Vec3b>(i,j)[1]) / sum);

			sumR += (float(im.at<cv::Vec3b>(i,j)[2]) / sum);
			rVect.push_back(float(im.at<cv::Vec3b>(i,j)[2]) / sum);
		}
		cout << endl << "averG: " << (sumG/(((float)im.rows) *( (float)im.cols))) << " averR: " << (sumR/(((float)im.rows) *( (float)im.cols)));
		cout << endl << "gVectSize and rVectSize: " << gVect.size() << " and " << rVect.size();

		cout << endl << "minG: " << *(std::min_element(std::begin(gVect), std::end(gVect)));
		cout << endl << "maxG: " << *(std::max_element(std::begin(gVect), std::end(gVect)));


		cout << endl << "minR: " << *(std::min_element(std::begin(rVect), std::end(rVect)));		
		cout << endl << "maxR: " << *(std::max_element(std::begin(rVect), std::end(rVect)));

		cin.get();
		cin.get();
}
