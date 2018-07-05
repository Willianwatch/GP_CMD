#include "stdafx.h"
#include "Gene.h"

void mean(vector<Mat> &image_list, Mat &output)
{
	cv::blur(image_list[0], output, cv::Size(3, 3), cv::Point(0, 0));
}

void equalizeHist(vector<Mat>& imageList, Mat & output)
{
	cv::equalizeHist(imageList[0], output);
}

void min(vector<Mat>& imageList, Mat & output)
{
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	cv::erode(imageList[0], output, element);
}

void max(vector<Mat>& imageList, Mat & output)
{
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	cv::dilate(imageList[0], output, element);
}

void sobel(vector<Mat>& imageList, Mat & output)
{
	Mat sobel_x, sobel_y;
	Sobel(imageList[0], sobel_x, CV_16S, 1, 0);
	convertScaleAbs(sobel_x, sobel_x);
	Sobel(imageList[0], sobel_y, CV_16S, 0, 1);
	convertScaleAbs(sobel_y, sobel_y);
	addWeighted(sobel_x, 0.5, sobel_y, 0.5, 0, output);
}

void lightEdge(vector<Mat>& imageList, Mat & output)
{
	cv::Laplacian(imageList[0], output, CV_16S);
	output.convertTo(output, CV_8U);
}

void darkEdge(vector<Mat>& imageList, Mat & output)
{
	cv::Laplacian(imageList[0], output, CV_16S);
	output += 255;
	output.convertTo(output, CV_8U);
}

void lightPixel(vector<Mat>& imageList, Mat & output)
{
	Scalar meanScalar = cv::mean(imageList[0]);
	uchar meanValue = static_cast<uchar>(meanScalar.val[0]);
	multiply(imageList[0], (imageList[0] >= meanValue) / 255, output);
}

void darkPixel(vector<Mat>& imageList, Mat & output)
{
	Scalar meanScalar = cv::mean(imageList[0]);
	uchar meanValue = static_cast<uchar>(meanScalar.val[0]);
	output = imageList[0] + (imageList[0] >= meanValue);
}

void largeArea(vector<Mat>& imageList, Mat & output)
{
	output = imageList[0];
	Mat bwImage;
	grayToBw(output, bwImage);
	map<ushort, list<pair<int, int>>> labeledPixels;
	double meanOfRegionPixels = labelArea(bwImage, labeledPixels);
	for (auto &i : labeledPixels)
	{
		if (i.second.size() < meanOfRegionPixels)
		{
			for (auto &j : i.second)
				output.at<uchar>(j.first - 1, j.second - 1) = 255;
		}
	}
}

void smallArea(vector<Mat>& imageList, Mat & output)
{
	output = imageList[0];
	Mat bwImage;
	grayToBw(output, bwImage);
	map<ushort, list<pair<int, int>>> labeledPixels;
	double meanOfRegionPixels = labelArea(bwImage, labeledPixels);
	for (auto &i : labeledPixels)
	{
		if (i.second.size() >= meanOfRegionPixels)
		{
			for (auto &j : i.second)
				output.at<uchar>(j.first - 1, j.second - 1) = 255;
		}
	}
}

void inversion(vector<Mat>& imageList, Mat & output)
{
	output = 255 - imageList[0];
}

void logicalSum(vector<Mat>& imageList, Mat & output)
{
	output = cv::max(imageList[0], imageList[1]);
}

void logicalProd(vector<Mat>& imageList, Mat & output)
{
	output = cv::min(imageList[0], imageList[1]);
}

void algebraicSum(vector<Mat>& imageList, Mat & output)
{
	imageList[0].convertTo(imageList[0], CV_32F);
	imageList[1].convertTo(imageList[1], CV_32F);
	output = imageList[0] + imageList[1] - imageList[0].mul(imageList[1]) / 255;
	output.convertTo(output, CV_8UC1);
}

void algebraicProd(vector<Mat>& imageList, Mat & output)
{
	imageList[0].convertTo(imageList[0], CV_32F);
	imageList[1].convertTo(imageList[1], CV_32F);
	output = imageList[0].mul(imageList[1]) / 255;
	output.convertTo(output, CV_8UC1);
}

void boundedSum(vector<Mat>& imageList, Mat & output)
{
	output = imageList[0] + imageList[1];
}

void boundedProd(vector<Mat>& imageList, Mat & output)
{
	output = imageList[0] - (255 - imageList[1]);
}

void opening(vector<Mat>& imageList, Mat & output)
{
	morphologyEx(imageList[0], output, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(3, 3)));
}

void closing(vector<Mat>& imageList, Mat & output)
{
	morphologyEx(imageList[0], output, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3)));
}

void grayToBw(Mat & grayImage, Mat & bwImage)
{
	int NumberOfRows = grayImage.rows;
	int NumberOfCols = grayImage.cols;
	bwImage = Mat::zeros(NumberOfRows, NumberOfCols, CV_8UC1);

	Mat rowDiff, colDiff, rowEqual, colEqual;
	absdiff(grayImage(Range(1, NumberOfRows), Range(0, NumberOfCols)), grayImage(Range(0, NumberOfRows - 1), Range(0, NumberOfCols)), rowDiff);
	absdiff(grayImage(Range(0, NumberOfRows), Range(1, NumberOfCols)), grayImage(Range(0, NumberOfRows), Range(0, NumberOfCols - 1)), colDiff);

	rowEqual = (rowDiff == 0);
	colEqual = (colDiff == 0);
	bwImage(Range(1, NumberOfRows), Range(0, NumberOfCols)) += rowEqual;
	bwImage(Range(0, NumberOfRows - 1), Range(0, NumberOfCols)) += rowEqual;
	bwImage(Range(0, NumberOfRows), Range(1, NumberOfCols)) += colEqual;
	bwImage(Range(0, NumberOfRows), Range(0, NumberOfCols - 1)) += colEqual;
}

double labelArea(Mat& bwImage, map<ushort, list<pair<int, int>>>& labeledPixels)
{
	int NumberOfRows = bwImage.rows;
	int NumberOfCols = bwImage.cols;

	ushort labelCount = 0;
	size_t regionPixelCount = 0;
	Mat containerImage = Mat::zeros(NumberOfRows + 2, NumberOfCols + 2, CV_8UC1);
	bwImage.copyTo(containerImage(Range(1, NumberOfRows + 1), Range(1, NumberOfCols + 1)));
	Mat labeledImage = Mat::zeros(NumberOfRows + 2, NumberOfCols + 2, CV_16UC1);

	for (int i = 1; i != NumberOfRows + 1; ++i)
	{
		const uchar* rowData = containerImage.ptr<uchar>(i);
		for (int j = 1; j != NumberOfCols + 1; ++j)
		{
			if (labeledImage.at<ushort>(i, j) != 0);
			else if (rowData[j] == 0);
			else
			{
				++labelCount;
				list<pair<int, int>> &neighborPixels = labeledPixels[labelCount];
				set<pair<int, int>> neighborPixelsSet;
				neighborPixels.push_back(make_pair(i, j));
				neighborPixelsSet.insert(make_pair(i, j));
				auto li_it = neighborPixels.begin();

				while (li_it != neighborPixels.end())
				{
					pair<int, int> &currentPixel = *li_it;
					int currentX = currentPixel.first;
					int currentY = currentPixel.second;
					labeledImage.at<ushort>(currentX, currentY) = labelCount;

					if (labeledImage.at<ushort>(currentX - 1, currentY) != 0);
					else if (containerImage.at<uchar>(currentX - 1, currentY) == 0);
					else if (neighborPixelsSet.find(make_pair(currentX - 1, currentY)) != neighborPixelsSet.end());
					else
					{
						neighborPixels.push_back(make_pair(currentX - 1, currentY));
						neighborPixelsSet.insert(make_pair(currentX - 1, currentY));
					}

					if (labeledImage.at<ushort>(currentX + 1, currentY) != 0);
					else if (containerImage.at<uchar>(currentX + 1, currentY) == 0);
					else if (neighborPixelsSet.find(make_pair(currentX + 1, currentY)) != neighborPixelsSet.end());
					else
					{
						neighborPixels.push_back(make_pair(currentX + 1, currentY));
						neighborPixelsSet.insert(make_pair(currentX + 1, currentY));
					}

					if (labeledImage.at<ushort>(currentX, currentY - 1) != 0);
					else if (containerImage.at<uchar>(currentX, currentY - 1) == 0);
					else if (neighborPixelsSet.find(make_pair(currentX, currentY - 1)) != neighborPixelsSet.end());
					else
					{
						neighborPixels.push_back(make_pair(currentX, currentY - 1));
						neighborPixelsSet.insert(make_pair(currentX, currentY - 1));
					}

					if (labeledImage.at<ushort>(currentX, currentY + 1) != 0);
					else if (containerImage.at<uchar>(currentX, currentY + 1) == 0);
					else if (neighborPixelsSet.find(make_pair(currentX, currentY + 1)) != neighborPixelsSet.end());
					else
					{
						neighborPixels.push_back(make_pair(currentX, currentY + 1));
						neighborPixelsSet.insert(make_pair(currentX, currentY + 1));
					}

					++li_it;
				}
				regionPixelCount += neighborPixels.size();
			}
		}
	}
	double meanOfRegionPixels = static_cast<double>(regionPixelCount) / labelCount;
	return meanOfRegionPixels;
}