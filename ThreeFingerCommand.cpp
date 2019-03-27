#include <map> 
#include "ThreeFingerCommand.h"

ThreeFingerCommand::ThreeFingerCommand()
{
	standartsPath = "threeFinger";
	matrixsPath = "threeFingerMatrixs";	
	hnn = HopfieldNN();	

	vector<string> matrixs = get_all_files_names_within_folder(matrixsPath);
	if(std::find(matrixs.begin(), matrixs.end(), "W.txt") != matrixs.end()) 
		hnn.setW(Matrix(matrixsPath + "/" + "W.txt"));
	else ICommand::retrain(standartsPath, matrixsPath, hnn);
}

void ThreeFingerCommand::addGesture(Image im, String name)
{
	ICommand::addNewGesture(im, name, standartsPath, matrixsPath, hnn);	
}

String ThreeFingerCommand::recognize(Image im)
{ 
	return ICommand::recognizeImage(im,  matrixsPath,  hnn);	
}

vector<string> ThreeFingerCommand::get_all_files_names_within_folder(string folder)
{
	return ICommand::get_all_files_names_within_folder(folder);
}