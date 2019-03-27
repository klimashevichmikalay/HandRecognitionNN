#pragma once
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "Matrix.h"
#include "ICommand.h"
#include "HopfieldNN.h"

using namespace std;

class TwoFingerCommand: public ICommand
{
public:
	TwoFingerCommand();
	void addGesture(Image im, String name);
	String recognize(Image im);

private:
	HopfieldNN hnn;
	String standartsPath;
	String matrixsPath;
	vector<string> get_all_files_names_within_folder(string folder);
};