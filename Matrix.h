#pragma once
#include <ostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cmath>
#include <string>
class Matrix {
	float** matrix;
	int rows;
	int columns;
	float eps;
public:
	Matrix(int, int);
	Matrix()
	{ 
		matrix = nullptr; 
		eps = 0.000001;
	}
	Matrix(const Matrix&);

	~Matrix() {
		if (matrix != nullptr){		
	for (int i = 0; i < rows; i++)
		delete[]matrix[i];
		delete[]matrix;
	}
}
	Matrix Matrix::transpose(const Matrix &mt);
	int getColumns() const { return columns; }
	int getRows() const { return rows; }
	float* operator[](int p) { return matrix[p]; }
	const float* operator[](int p) const { return matrix[p]; }
	Matrix& operator=(const Matrix&);
	Matrix reshape(int rows, int columns) const;
	friend Matrix operator*(Matrix&, Matrix&);
	friend Matrix operator/(Matrix&, float);
	friend Matrix operator*(float, Matrix&);
	friend Matrix absMatrix(Matrix&);
	friend std::ostream& operator<<(std::ostream& os, const Matrix& mt);
	void operator+=(Matrix&);
	bool operator==(const Matrix&);
	Matrix T();
	Matrix operator-(const Matrix& mt);
	Matrix operator+(const Matrix& mt);
	Matrix operator/(const Matrix& mt);
	friend Matrix signMatrix(Matrix&);
	friend void showMt(std::string name, const Matrix& mt);
	friend bool isDiff(Matrix& mt1, Matrix& mt2, float eps);
};