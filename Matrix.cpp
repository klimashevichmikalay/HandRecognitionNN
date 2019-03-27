#include "Matrix.h"

Matrix::Matrix(std::string file) 
{		
	string line;
	string num;
	std::vector<float> row;
	ifstream myfile(file);			
	while ( getline (myfile, line))
	{  
		std::stringstream ssLine(line);
		while (getline(ssLine, num, ' ')) 
			row.push_back(atof(num.c_str()));					

		matrix.push_back(row);	
		row.clear();
	}
	myfile.close();
	rows = matrix.size();
	cols = (matrix.at(0)).size();	
}

Matrix::Matrix( const int& _rows, const int& _cols) 
{
	this->rows = _rows;
	this->cols = _cols;
	matrix = vector<vector<float>>(_rows,(vector<float>(_cols, 0.0)));	
}

Matrix::Matrix() 
{
	matrix = vector<vector<float>>(0,(vector<float>(0, 0.0)));
}

int Matrix::getRows() const 
{
	return rows;
}

int  Matrix::getCols() const
{
	return cols;
}


ostream& operator<<(std::ostream& os, const Matrix &m)
{	        
	ostream_iterator<float> out_it (os," ");
	os.precision(5);
	for (int i = 0; i < m.getRows(); i++, os << std::endl)									
		std::copy(m.matrix.at(i).begin(), m.matrix.at(i).end(), out_it );			      
	return os;
}

void Matrix::save(std::string file)
{
	std::ofstream fout(file);
	fout << (*this);
	fout.close();
}

bool Matrix::cmp(float x, float y)
{
	return ((abs(x -y)) < 0.0005);
}

bool Matrix::operator==(const Matrix& mt)
{
	if (rows != mt.getRows() || cols != mt.getCols())
		return false;

	for (int i = 0; i < mt.getRows(); i++)							
		if(!std::equal(mt.matrix.at(i).begin(), mt.matrix.at(i).end(), matrix.at(i).begin(), &cmp))
			return false;	

	return true;
}

Matrix Matrix::operator-(const Matrix& mt) {
	Matrix res(mt.rows, mt.cols);
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++)
			res[i][j] = this->matrix[i][j] - mt[i][j];
	return res;
}

Matrix operator*(Matrix& m1, Matrix& m2) 
{
	Matrix result(m1.getRows(), m2.getCols());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m2.getCols(); j++)
			for (int k = 0; k < m1.getCols(); k++)
				result[i][j] += m1[i][k] * m2[k][j];

	return result;
}

Matrix operator*(float d, Matrix& m) 
{
	Matrix result = m;
	for(int i =0 ; i < result.getRows(); i++)
		std::transform(result[i].begin(), result[i].end(), result[i].begin(),
		std::bind(std::multiplies<float>(), std::placeholders::_1, d));
	return result;
}

Matrix operator+(Matrix& m1, Matrix& m2)
{
	Matrix res(m1.getRows(), m1.getCols());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m1.getCols(); j++)
			res[i][j] = m1[i][j] + m2[i][j];

	return res;
}

Matrix Matrix::T() 
{
	Matrix res(cols, rows);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			res[j][i] = (*this)[i][j];

	return res;
}

Matrix Matrix::reshape(int rows, int columns) const
{ 
	Matrix vec(rows, columns);
	int r = 0, c = 0;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++) {
			vec[r][c++] = (*this)[i][j];
			if (c == columns) {
				c = 0;
				r++;
			}
		}
		return vec;
}

void Matrix::shiftUnits()
{
	shiftInfo si = getShiftInfo() ;
	int toLeft = si.x;
	int toUp = si.y;

	for (int i = 0; i < rows; i++)	
		for (int j = 0; j < cols; j++) 
		{
			if((*this)[i][j] == 1)
			{
				(*this)[i][j] = 0;
				(*this)[i - toUp][j - toLeft] = 1;
			}
		}
}
shiftInfo Matrix::getShiftInfo() 
{
	int _x = 1000;
	int _y = 1000;
	int _x1 = 0;
	int _y1 = 0;


	for (int i = 0; i < rows; i++)	
		for (int j = 0; j < cols; j++) 
		{
			if((*this)[i][j] == 1)
			{
				if(i < _y)
					_y = i;

				if(j < _x)
					_x = j;

				if(i > _y1)
					_y1 = i;

				if(j > _x1)
					_x1 = j;
			}
		}

		shiftInfo si;
		si.x = _x;
		si.y = _y;
		si.x1 = _x1;
		si.y1 = _y1;

		return si;
}

void Matrix::show() 
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++) 
			if((*this)[i][j] == 1)			
			{
				cout<< '#';	}		
			else		
			{
				cout<<' ';
			}	
			cout << endl;
	}
}