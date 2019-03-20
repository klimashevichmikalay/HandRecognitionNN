#include "Matrix.h" 

Matrix::Matrix(int rows, int columns) {	
	this->rows = rows;
	this->columns = columns;
	matrix = new float*[rows];
	for (int i = 0; i < rows; i++)
		matrix[i] = new float[columns];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			matrix[i][j] = 0;
}
std::ostream& operator<<(std::ostream& os, const Matrix& mt) {

	os << std::endl << std::endl << std::endl;
	for (int i = 0; i < mt.getRows(); i++, os << std::endl)
		for (int j = 0; j < mt.getColumns(); j++)
			if(mt[i][j] == 1){
				os << '#';}
			else {
				os << ' ';
			}
	return os;
}
bool Matrix::operator==(const Matrix& mt) {
	if (rows != mt.getRows() || columns != mt.getColumns())
		return false;
	for (int i = 0; i < mt.getRows(); i++)
		for (int j = 0; j < mt.getColumns(); j++)
			if (abs(matrix[i][j] - mt[i][j]) > eps)
				return false;
	return true;
}
Matrix& Matrix::operator=(const Matrix& mt) {
	if (this == &mt)
		return *this;
	delete[]matrix;
	matrix = new float*[mt.getRows()];
	for (int i = 0; i < mt.getRows(); i++)
		matrix[i] = new float[mt.getColumns()];
	rows = mt.getRows();
	columns = mt.getColumns();
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			matrix[i][j] = mt[i][j];
	return *this;
}


Matrix Matrix::T() {
	Matrix res(columns, rows);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			res[j][i] = matrix[i][j];
	return res;
}
Matrix Matrix::operator-(const Matrix& mt) {
	Matrix res(mt.rows, mt.columns);
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->columns; j++)
			res[i][j] = this->matrix[i][j] - mt[i][j];
	return res;
}
Matrix Matrix::operator+(const Matrix& mt) {
	Matrix res(mt.rows, mt.columns);
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->columns; j++)
			res[i][j] = this->matrix[i][j] + mt[i][j];
	return res;
}
Matrix Matrix::operator/(const Matrix& mt) {
	Matrix res(mt.rows, mt.columns);
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->columns; j++)
			res[i][j] = this->matrix[i][j] / mt[i][j];
	return res;
}
Matrix::Matrix(const Matrix& mt) {
	rows = mt.getRows();
	columns = mt.getColumns();
	matrix = new float*[rows];
	for (int i = 0; i < rows; i++)
		matrix[i] = new float[columns];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			matrix[i][j] = mt[i][j];
}

Matrix Matrix::transpose(const Matrix &mt) {
	Matrix res = Matrix(mt.getColumns(), mt.getRows());
	for(int i = 0; i < mt.getRows(); ++i)
      for(int j=0; j<mt.getColumns(); ++j) {
         res[j][i] = mt[i][j];
      }
	  return res;
}

Matrix Matrix::reshape(int rows, int columns) const { // (5x4) -> (2x10)
	Matrix vec(rows, columns);
	int r = 0, c = 0;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->columns; j++) {
			vec[r][c++] = matrix[i][j];
			if (c == columns) {
				c = 0;
				r++;
			}
		}
	return vec;
}
Matrix operator*(Matrix& m1, Matrix& m2) {
	Matrix result(m1.getRows(), m2.getColumns());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m2.getColumns(); j++)
			for (int k = 0; k < m1.getColumns(); k++)
				result[i][j] += m1[i][k] * m2[k][j];
	return result;
}
Matrix signMatrix(Matrix& mt) { 
	Matrix result(mt.getRows(), mt.getColumns());
	for (int i = 0; i < mt.getRows(); i++)
		for (int j = 0; j < mt.getColumns(); j++)
			result[i][j] = (mt[i][j] < 0) ? -1 : 1;
	return result;
}
Matrix operator/(Matrix& m1, float d) {
	Matrix result(m1.getRows(), m1.getColumns());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m1.getColumns(); j++)
			for (int k = 0; k < m1.getColumns(); k++)
				result[i][j] = m1[i][k] / d;
	return result;
}
Matrix absMatrix(Matrix& m1) {
	Matrix result(m1.getRows(), m1.getColumns());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m1.getColumns(); j++)
			for (int k = 0; k < m1.getColumns(); k++)
				result[i][j] = abs(m1[i][k]);
	return result;
}
Matrix operator*(float d, Matrix& m1) {
	Matrix result(m1.getRows(), m1.getColumns());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m1.getColumns(); j++)			
				result[i][j] = m1[i][j] * d;
	return result;
}

 Matrix sum(const Matrix& mt1, const Matrix& mt2)
 {	
	Matrix res(mt1.rows, mt1.columns);
	for (int i = 0; i < mt1.rows; i++)
		for (int j = 0; j < mt1.columns; j++)
			(*(res[i]+j)) = (*(mt1[i]+j)) + (*(mt2[i]+j));
	return res;
} 

Matrix multipleNumber(float d, const Matrix& m1) {
	Matrix result(m1.getRows(), m1.getColumns());
	for (int i = 0; i < m1.getRows(); i++)
		for (int j = 0; j < m1.getColumns(); j++)			
				*(result[i] + j) = ((*(m1[i]+j)) * d);
	return result;
}

void Matrix::operator+=(Matrix& mat) {
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->columns; j++)
			this->matrix[i][j] += mat[i][j];
}
void showMt(std::string name, const Matrix& mt) {
	using namespace std;
	cout << setw(name.size() + 1) << name << endl << mt;
	_getch();
	system("cls");
}
bool isDiff(Matrix& mt1, Matrix& mt2, float eps)
{
	if (mt1.getRows() != mt2.getRows() || mt1.getColumns() != mt2.getColumns())
		return false;
	for (int i = 0; i < mt1.getRows(); i++)
		for (int j = 0; j < mt1.getColumns(); j++)
			if (abs(mt1[i][j] - mt2[i][j]) > eps) {
				std::cout << std::endl << "diff: " << abs(mt1[i][j] - mt2[i][j]) << std::endl;
				return false;
			}
	return true;
}