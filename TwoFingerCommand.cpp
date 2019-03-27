#include <map> 
#include "TwoFingerCommand.h"

TwoFingerCommand::TwoFingerCommand()
{
	standartsPath = "twoFinger";
	matrixsPath = "twoFingerMatrixs";	
	hnn = HopfieldNN();	

	vector<string> matrixs = get_all_files_names_within_folder(matrixsPath);
	if(std::find(matrixs.begin(), matrixs.end(), "W.JPG") != matrixs.end()) 
		hnn.setW(Matrix(matrixsPath + "/" + "W.JPG"));
	else retrain();
}

void TwoFingerCommand::addGesture(Image im, String name)
{
	im.saveImage(standartsPath + "/" + name + ".JPG");
	retrain();
}


String TwoFingerCommand::recognize(Image im)
{ 
	map <String, Matrix> images;
	// vector <Matrix> mtrxs;
	vector<string> matrixs = get_all_files_names_within_folder(matrixsPath);

	for(int i = 0; i < matrixs.size(); i++)
	{
		images.insert(pair<String, Matrix>(matrixs[i], Matrix(matrixsPath + "/" + matrixs[i])));
	}

	Matrix Y = im.getMatrix();
	Matrix result = hnn.recognize(Y);

	map<String, Matrix>::iterator itr;    
	for (itr = images.begin(); itr != images.end(); ++itr)
		if( itr->second == result)
	return getName(itr->first);

	return "not recognized";
}


void TwoFingerCommand::retrain()
{
	vector<string> names = get_all_files_names_within_folder(standartsPath);	 
	if(names.empty())
		return;

	map <String, Matrix> matrix;
	vector<Matrix> vect;
	for(int i = 0; i < names.size(); i++)
	{
		Image im = Image(standartsPath + "/" + names[i]);
		matrix.insert(pair<String, Matrix>(names[i], im.getMatrix()));
		vect.push_back(im.getMatrix());
	}	
	hnn.setStandarts(vect);
	hnn.train();
	(hnn.getW()).save(matrixsPath + "/" + "W.txt");

	map<String, Matrix>::iterator itr;    
	for (itr = matrix.begin(); itr != matrix.end(); ++itr)           
		(recognize((itr->second))).save(matrixsPath + "/" + (itr->first) + ".txt");
}


vector<string> TwoFingerCommand::get_all_files_names_within_folder(string folder)
{
	return ICommand::get_all_files_names_within_folder(folder);
}


String TwoFingerCommand::getName(String str)
{
	String name = "";

	for(int i = 0; i < str.length(); i++)
	{
		if(strchr("0123456789.",str[i]))
			return name;

		name += str[i];
	}
	return name;
}


Matrix TwoFingerCommand::recognize(Matrix Y)
{	
	return hnn.recognize(Y);	
}