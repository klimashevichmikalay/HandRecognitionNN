#include <stdlib.h>
#include <vector>
#include <iostream>
#include "Matrix.h"
#include "Image.h"
#include "HopfieldNN.h"
#include "Controller.h"
#include <Windows.h>
#include <tchar.h>

using namespace cv;
using namespace std;

const int width = 40;
const int height = 30;
const int square = 16;

void test();
void getScreens(String nameScreen);
std::wstring s2ws(const std::string& s);
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
		if( waitKey(10) == 32 ) {
				
			Image im =  Image(frame);
			(im.getMatrix()).show();	
			String res = cntrl.recognize(frame);
			cout << "\nResult: " + res;
		    int result = MessageBox(NULL, res.c_str(), "Continue?",  MB_YESNO);
			switch (result)
			{
			case IDYES:				
				break;
			case IDNO:
				exit(0);
				break;	
			}
		}	
		if( waitKey(10) == 9 )  
		{
			cout << "\nEnter a name for new gesture:\n";
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

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
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