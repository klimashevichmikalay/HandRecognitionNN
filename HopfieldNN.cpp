#include "HopfieldNN.h"

HopfieldNN::HopfieldNN()
{
	this->width = 40;
	this->height = 30;
	this->square = 16;
	this->eps = 0.001;	
	this->W = Matrix(height*width, height*width);
}

void HopfieldNN::setW(Matrix _W)
{
	this->W = _W;
}

void HopfieldNN::setStandarts(vector<Matrix> _standards)
{
	this->standards = _standards;
}

Matrix HopfieldNN::train() 
{
	float h = 0.8;
	int N = height*width;
	Matrix prevW(N, N);	
	do 
	{		
		for (int i = 0; i < this->standards.size(); i++)
		{		  
			Matrix Xi = standards[i].reshape(height*width, 1);		
			W = W + ((h / N) * (Xi - W * Xi) * Xi.T()); 		  
		}
		if (W == prevW)		
			return W;

		prevW = W;
	} 
	while (true);
}

Matrix HopfieldNN::recognize(Matrix Y)
{
	int N = height*width;
	Matrix Q(N, 1);	
	Matrix prevQ(N, 1);
	Y = Y.reshape(N, 1);
	int iterations = 100;
	int count = 0;

	do 
	{				
		Q = F(W*Y);
		if (Y == Q || prevQ == Q)
		{		
			return (F(Q.reshape(height, width)));			
		}
		else
		{
			prevQ = Y;
			Y = Q;
		}
		count++;
	}
	while (count < iterations);

	return	(F(Q.reshape(height, width)));
}

Matrix HopfieldNN::F(const Matrix& matrix)
{ 
	Matrix result(matrix.getRows(), matrix.getCols());
	for (int i = 0; i < matrix.getRows(); i++)
		for (int j = 0; j < matrix.getCols(); j++)		
			if (matrix[i][j] > 0)
				result[i][j] = 1;
			else if (matrix[i][j] < 0)
				result[i][j] = -1;

	return result;
}