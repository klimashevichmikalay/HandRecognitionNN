#pragma once
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Matrix.h"

using namespace cv;
using namespace std;


typedef struct BGRA
{    
	float b;
	float g; 
	float r; 
	float a; 
};

typedef struct HSV 
{
	float h;      
	float s;       
	float v;      
};

typedef struct CrCbY 
{
	float y;
	float cr;
	float cb;  
};

class Image 
{
public:
	Image(Mat _mt);
	Matrix getMatrix();
private:
	int width;
	int height;
	int square;
	bool isCorrected;
	Mat mt;	
	bool isSkinSquare(int startRow, int startCol, Mat im);
	HSV bgrToHsv(BGRA in);	
	CrCbY bgraToCrCbY(BGRA bgra);
	bool  isSkinPixel(int x, int y, Mat im);
	Mat Image::correctImage(Matrix m);
	Mat matToBgra(Mat _mt);
	Matrix getImageMatrix(Mat im);
};