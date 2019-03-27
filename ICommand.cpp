#include "ICommand.h"

vector<string> ICommand::get_all_files_names_within_folder(const string &folder)
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

void ICommand::addNewGesture(Image im,const String &name,const String &standartsPath,const String &matrixsPath, HopfieldNN &hnn)
{
	im.saveImage(standartsPath + "/" + name + ".JPG");
	retrain(standartsPath, matrixsPath, hnn);
}

String ICommand::recognizeImage(Image im,const String &matrixsPath, HopfieldNN &hnn)
{ 
	map <String, Matrix> images;	
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

void ICommand::retrain(const String &standartsPath,const String &matrixsPath, HopfieldNN &hnn)
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
		(recognize((itr->second), hnn)).save(matrixsPath + "/" + (itr->first) + ".txt");
}

String ICommand::getName(const String &str)
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

Matrix ICommand::recognize(const Matrix &Y, HopfieldNN &hnn)
{	
	return hnn.recognize(Y);	
}