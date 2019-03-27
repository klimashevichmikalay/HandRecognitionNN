#pragma once
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <map> 
#include "Matrix.h"
#include "OneFingerCommand.h"
#include "TwoFingerCommand.h"
#include "ThreeFingerCommand.h"
#include "FourFingerCommand.h"

using namespace cv;
using namespace std;

class Controller
{
public:	
	enum fingersNumber {ONE = 1, TWO, THREE, FOUR};
	Controller();
	String recognize(Mat mat);
	String addGesture(Mat mat, String name);
private:	
	OneFingerCommand ofc;
	TwoFingerCommand tfc;
	ThreeFingerCommand thfc;
	FourFingerCommand ffc;
	map <fingersNumber, ICommand*> commands;
};