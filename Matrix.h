#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <sstream>
#include <numeric>
#include <functional>
using namespace std;

class Matrix {
private:
	int rows;
	int cols;	
	vector<vector<float>> matrix;
	static bool cmp(float x, float y);
public:
	inline int getRows() const;
	inline int getCols() const;
	Matrix();
	Matrix(std::string file);
	Matrix( const int& _rows, const int& _cols);
	friend std::ostream& operator<<(std::ostream& os, const Matrix &mt);
	void save(std::string file);	
    bool Matrix::operator==(const Matrix& mt);	
	friend	Matrix operator*(Matrix& m1, Matrix& m2);
	friend Matrix operator*(float d, Matrix& m);
	friend  Matrix operator+(Matrix& m1, Matrix& m2);
	Matrix operator-(const Matrix& mt);
	Matrix T();	
	Matrix reshape(int rows, int columns) const;
	inline vector<float> & operator[](int x) { return matrix.at(x); }
	inline const vector<float> operator[](int p) const { return matrix[p]; }
	void show();
};