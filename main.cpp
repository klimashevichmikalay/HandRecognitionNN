#include <stdlib.h>
#include <vector>
#include <iostream>
#include "Matrix.h"
#include "Image.h"
#include "HopfieldNN.h"

using namespace cv;
using namespace std;

const int width = 40;
const int height = 30;
const int square = 16;

int main(int argc, char** argv)
{
	vector<Matrix> matrixs;

	Mat bgr1  =  imread ("imageForLearn/image1.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im1 =  Image(bgr1);
	matrixs.push_back(im1.getMatrix());

	Mat bgr2  =  imread ("imageForLearn/image2.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im2 =  Image(bgr2);
	matrixs.push_back(im2.getMatrix());

	Mat bgr3  =  imread ("imageForLearn/image3.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im3 =  Image(bgr3);
	matrixs.push_back(im3.getMatrix());
	
	HopfieldNN hopnn = HopfieldNN();
	hopnn.setStandarts(matrixs);
	hopnn.train();

	/////////////////////////////////
	////////////////////////////////////

	Mat bgr4  =  imread ("imageForLearn/recogn1.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im4 =  Image(bgr4);
	cout << "\nim for recognize:\n";
	im4.getMatrix().show();
	cout << "\nafter recognize:\n";
	hopnn.recognize(im4.getMatrix());
	

	Mat bgr5  =  imread ("imageForLearn/recogn2.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im5 =  Image(bgr5);
	cout << "\nim for recognize:\n";
	im5.getMatrix().show();
	cout << "\nafter recognize:\n";
	hopnn.recognize(im5.getMatrix());
	

	Mat bgr6  =  imread ("imageForLearn/recogn3.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im6 =  Image(bgr6);
	cout << "\nim for recognize:\n";
	im6.getMatrix().show();
	cout << "\nafter recognize:\n";
	hopnn.recognize(im6.getMatrix());

	cin.get();
	return 0;
}
