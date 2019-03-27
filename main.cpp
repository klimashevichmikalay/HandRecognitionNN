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
	VideoCapture cap;    

	if(!cap.open(0))
		return 0;

	Controller cntrl = Controller();

	int count = 0;
	for(;;) 
	{	
		Mat frame;
		cap >> frame;
		imshow("this is you, smile! :)", frame);
		if( waitKey(10) == 27 ) {
			count++;		
			stringstream ss;
			ss << count;
			string str = ss.str();			
			Image im =  Image(frame);
			(im.getMatrix()).show();
			cout << "\nIt is:\n" << cntrl.recognize(frame);
		}	
	    if( waitKey(10) == 32 )  
		{
			cout << "Enter a name for new gesture:\n";
			String name;
			cin >> name;
			cntrl.addGesture(frame, name);
		}
	}
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
	Controller cntrl = Controller();	
	cout << "\ngesture name(ok):\n" << cntrl.recognize(imread ("imageForLearn/recogn1.JPG", CV_LOAD_IMAGE_COLOR));
	cout << "\ngesture name(vikt):\n" << cntrl.recognize(imread("imageForLearn/recogn2.JPG", CV_LOAD_IMAGE_COLOR));	
	cout << "\ngesture name:\(ok)n" << cntrl.recognize(imread("imageForLearn/recogn3.JPG", CV_LOAD_IMAGE_COLOR));
	cout << "\n\nafter add new gesture:\n";
	cntrl.addGesture(imread("imageForLearn/rock.JPG", CV_LOAD_IMAGE_COLOR), "weWillRockYou");

	cout << "\ngesture name(ok):\n" << cntrl.recognize(imread ("imageForLearn/recogn1.JPG", CV_LOAD_IMAGE_COLOR));
	cout << "\ngesture name(v):\n" << cntrl.recognize(imread("imageForLearn/recogn2.JPG", CV_LOAD_IMAGE_COLOR));	
	cout << "\ngesture name(rock):\n" << cntrl.recognize(imread("imageForLearn/recogn3.JPG", CV_LOAD_IMAGE_COLOR)); 

}

