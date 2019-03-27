#pragma once
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <map> 
#include <windows.h>
#include "Matrix.h"
#include "Image.h"

using namespace cv;
using namespace std;

enum fingersNumber {ONE = 1, TWO, THREE, FOUR};

class ICommand
{
public:		
	virtual void addGesture(Image im, String name) = 0;
	virtual	String recognize(Image im) = 0;
	virtual	void retrain() = 0;
	virtual	vector<string> get_all_files_names_within_folder(string folder);
	virtual	String getName(String str) = 0;
	virtual	Matrix recognize(Matrix Y) = 0;
};