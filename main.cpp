#include <stdlib.h>
#include <vector>
#include <iostream>
#include "Matrix.h"
#include "Image.h"
#include "HopfieldNN.h"
#include "Controller.h"
#include <Windows.h>

using namespace cv;
using namespace std;

const int width = 40;
const int height = 30;
const int square = 16;

void test();
void getScreens(String nameScreen);
vector<string> get_all_files_names_within_folder(string folder);
String getName(String str);


int main(int argc, char** argv)
{
	Controller cntrl = Controller();
	Mat m1  =  imread ("imageForLearn/recogn1.JPG",  CV_LOAD_IMAGE_COLOR );
	cout << "\ngesture name:\n" << cntrl.recognize(m1);

	
	Mat m2  =  imread ("imageForLearn/recogn2.JPG",  CV_LOAD_IMAGE_COLOR );
	cout << "\ngesture name:\n" << cntrl.recognize(m2);

	Mat m3  =  imread ("imageForLearn/recogn3.JPG",  CV_LOAD_IMAGE_COLOR );
	cout << "\ngesture name:\n" << cntrl.recognize(m3);

	//test();
	cin.get();
	return 0;
}

String getName(String str)
 {
	 String name = "";
	 
	 for(int i = 0; i < str.length(); i++)
	 {
		 if(!strchr("0123456789",str[i]))
			 name += str[i];
	 }
	 return name;
 }

vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd; 
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd); 
	if(hFind != INVALID_HANDLE_VALUE) { 
		do
		{            
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
				names.push_back(fd.cFileName);
			}
		}
		while(::FindNextFile(hFind, &fd)); 
		::FindClose(hFind); 
	} 
	return names;
}


void getScreens(String nameScreen) {
	VideoCapture cap;    

	if(!cap.open(0))
		return;

	int count = 0;
	for(;;) {	
		Mat frame;
		cap >> frame;
		imshow("this is you, smile! :)", frame);
		if( waitKey(10) == 27 ) {
			count++;		
			stringstream ss;
			ss << count;
			string str = ss.str();
			String name = nameScreen + str;
			cv::imwrite(name +  ".JPG", frame);
			Image im =  Image(frame);
			(im.getMatrix()).show();	
		}					
	}
}   

void test()
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
	(hopnn.recognize(im4.getMatrix())).show();


	Mat bgr5  =  imread ("imageForLearn/recogn2.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im5 =  Image(bgr5);
	cout << "\nim for recognize:\n";
	im5.getMatrix().show();
	cout << "\nafter recognize:\n";
	(hopnn.recognize(im5.getMatrix())).show();


	Mat bgr6  =  imread ("imageForLearn/recogn3.JPG",  CV_LOAD_IMAGE_COLOR );
	Image im6 =  Image(bgr6);
	cout << "\nim for recognize:\n";
	im6.getMatrix().show();
	cout << "\nafter recognize:\n";
	(hopnn.recognize(im6.getMatrix())).show();

}

