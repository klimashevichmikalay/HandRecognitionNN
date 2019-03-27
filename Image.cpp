#include "Image.h"

Image::Image(Mat _mt) 
{
	mt = _mt;
	width = 40;
	height = 30;
	square = 16;
	isCorrected = false;
	this->matrix = createImageMatrix(this->mt);
}

Image::Image(String path)
{
	mt = imread(path, CV_LOAD_IMAGE_COLOR );
	width = 40;
	height = 30;
	square = 16;
	isCorrected = false;
	this->matrix = createImageMatrix(this->mt);
}

Matrix Image::getMatrix()
{
	return this->matrix;
}

void Image::saveImage(String path)
{
	cv::imwrite(path, mt);
}
int Image::countFingers()
{
	bool skin = false;
	int count = 0;

	for (int j = 0; j < matrix.getCols(); j++) 
	{
		if(matrix[8][j] == 1)			
			skin = true;

		if(matrix[8][j] == -1 && skin)			
		{
			count++;
			skin = false;
		}
	}
	return count;
}

Mat Image::matToBgra(Mat _mt)
{
	Mat bgr = _mt;
	Mat res;
	pyrMeanShiftFiltering( bgr, res, 12, 12, 3);
	cv::Mat bgra;

	cv::cvtColor(res, bgra, cv::COLOR_BGR2BGRA);
	cv::Mat abgr(bgra.size(), bgra.type());
	int from_to[] = { 0,3, 1,1, 2,2, 3,0 };
	cv::mixChannels(&bgra,1,&abgr,1,from_to,4);
	return abgr;	
}

bool Image::isSkinSquare(int startRow, int startCol, Mat im)
{
	int skinPixels = 0;
	for(int i = startRow; i < startRow + square; i++)
		for(int j = startCol; j < startCol + square; j++)		
			if(isSkinPixel(i, j, im))
				skinPixels++;		

	if(skinPixels >= square*square*0.65)
		return true;

	return false;
}

HSV Image::bgrToHsv(BGRA in)
{
	HSV out;
	float min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min  : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max  : in.b;

	out.v = max;                             
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0;
		return out;
	}
	if( max > 0.0 ) { 
		out.s = (delta / max);             
	} else {		
		out.s = 0.0;
		out.h = NULL;                  
		return out;
	}
	if( in.r >= max )                          
		out.h = ( in.g - in.b ) / delta;        
	else
		if( in.g >= max )
			out.h = 2.0 + ( in.b - in.r ) / delta; 
		else
			out.h = 4.0 + ( in.r - in.g ) / delta; 

	out.h *= 60.0;                           

	if( out.h < 0.0 )
		out.h += 360.0;
	return out;
}

CrCbY Image::bgraToCrCbY(BGRA bgra)
{   //Y = 0.299 * R + 0.287 * G + 0.11  * B;
	CrCbY ccy;   
	ccy.y = 0.375* bgra.r + 0.5* bgra.g + 0.125* bgra.b;
	ccy.cr = bgra.r - ccy.y;
	ccy.cb = bgra.b - ccy.y;

	return ccy;
}

bool  Image::isSkinPixel(int x, int y, Mat im)
{
	BGRA bgra;
	bgra.a = float(im.at<cv::Vec4b>(x,y)[0]);
	bgra.g =  float(im.at<cv::Vec4b>(x,y)[1]);
	bgra.r = float(im.at<cv::Vec4b>(x,y)[2]);
	bgra.b = float(im.at<cv::Vec4b>(x,y)[3]);

	HSV hsv = bgrToHsv(bgra);
	CrCbY ccy = 	bgraToCrCbY(bgra);   	

	if((bgra.r > 95 && bgra.g > 40 && bgra.b > 20 && bgra.r > bgra.g && bgra.r > bgra.b
		&& (fabs(bgra.r - bgra.g) > 15) && bgra.a > 15 && ccy.cr > 70 && ccy.cb > 40 && ccy.y > 80
		&&( ccy.cr <= (1.5862*ccy.cb) + 20) && (ccy.cr >= (0.3448*ccy.cb) + 46.2069) &&
		(ccy.cr >= (-4.5652*ccy.cb) + 234.5652) && (ccy.cr <= (-1.15*ccy.cb) + 301.75)
		&& (ccy.cr <= (-2.2857*ccy.cb) + 432.85))|| 
		(hsv.h >= 0.0  && hsv.s >= 0.05 && hsv.s <= 0.68
		&& bgra.r > 95.0 && bgra.g > 40.0 && bgra.b > 20.0 && bgra.r > bgra.g && bgra.r > bgra.b
		&& (fabs(bgra.r - bgra.g) > 15) && bgra.a > 15 ))
		return true;

	return false;
}

Mat Image::correctImage(Matrix m)
{
	isCorrected = true;
	shiftInfo si =  m.getShiftInfo();
	float scale =  sqrt(float(si.x1 - si.x) * float(si.x1 - si.x)  + float(si.y1 - si.y) * float(si.y1 - si.y));

	if(scale == 0)
		return  mt;

	if(scale < 28.5 || scale > 29.5)
	{
		Mat outImg;
		cv::resize(mt, outImg, cv::Size(), 29.0 / scale, 29.0 / scale);
		Rect myROI((si.x) * square * 29.0 / scale, ((si.y) * square * 29.0 / scale), ((si.x1+2)* square * 29.0 / scale) - ((si.x) * square * 29.0 / scale), ((si.y1)* square * 29.0 / scale) - ((si.y) * square * 29.0 / scale));

		Mat croppedImage;	

		try
		{
			croppedImage = outImg(myROI);
		}
		catch ( cv::Exception & e )
		{
			return  mt;
		}		
		
		Mat correctedMt(480, 640, CV_8UC3, Scalar(0, 0, 0));	

		try
		{
			croppedImage.copyTo(correctedMt.rowRange(0, croppedImage.rows).colRange(0,  croppedImage.cols));
		}
		catch ( cv::Exception & e )
		{
			return  mt;
		}

		return correctedMt;
	}
	return  mt;
}

Matrix Image::createImageMatrix(Mat input)
{
	Matrix imMatrix = Matrix(height, width);	
	Mat im =  matToBgra(input);
	for(int i = 0; i < im.rows; i+=square)
		for(int j = 0; j < im.cols; j+=square)
		{
			if(isSkinSquare(i, j, im) == true)
			{
				imMatrix[i/square][j/square] = 1;
			}
			else {
				imMatrix[i/square][j/square] = -1;
			}
		}

		if(!isCorrected)				
			return createImageMatrix(correctImage(imMatrix));
		else
		{
			isCorrected = false;
			this->matrix = imMatrix;
			return imMatrix;
		}
}


