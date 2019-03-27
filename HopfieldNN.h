#pragma once
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Matrix.h"

using namespace std;
 
class HopfieldNN 
{
public:	
	HopfieldNN();
	Matrix train();
	Matrix recognize(Matrix Y);
	Matrix F(const Matrix& matrix);
	void setW(Matrix _W);
	void setStandarts(vector<Matrix> _standards);
	Matrix getW(){return W;}
private:
	int width;
	int height;
	int square;
	float eps;
	Matrix W;
	vector<Matrix> standards;
		
};