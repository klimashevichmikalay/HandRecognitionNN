#pragma once
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <map> 
#include <windows.h>
#include "Matrix.h"
#include "Image.h"
#include "HopfieldNN.h"

using namespace cv;
using namespace std;

enum fingersNumber {ONE = 1, TWO, THREE, FOUR};

class ICommand
{
public:	
	virtual void addGesture(Image im, String name) = 0;
	virtual String recognize(Image im) = 0;
	void addNewGesture(Image im, const String &name,const String &standartsPath,const String &matrixsPath, HopfieldNN &hnn);	
	void retrain(const String &standartsPath,const String &matrixsPath, HopfieldNN &hnn);
	vector<string> get_all_files_names_within_folder(const string &folder);
	String getName(const String &str);
	Matrix recognize(const Matrix &Y, HopfieldNN &hnn);
	String recognizeImage(Image im,const String &matrixsPath, HopfieldNN &hnn);
};