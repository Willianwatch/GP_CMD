#pragma once
#include "FUH.h"

void mean(vector<Mat> &imageList, Mat &output);
void equalizeHist(vector<Mat> &imageList, Mat &output);
void min(vector<Mat> &imageList, Mat &output);
void max(vector<Mat> &imageList, Mat &output);
void sobel(vector<Mat> &imageList, Mat &output);
void lightEdge(vector<Mat> &imageList, Mat &output);
void darkEdge(vector<Mat> &imageList, Mat &output);
void lightPixel(vector<Mat> &imageList, Mat &output);
void darkPixel(vector<Mat> &imageList, Mat &output);
void largeArea(vector<Mat> &imageList, Mat &output);
void smallArea(vector<Mat> &imageList, Mat &output);
void inversion(vector<Mat> &imageList, Mat &output);
void logicalSum(vector<Mat> &imageList, Mat &output);
void logicalProd(vector<Mat> &imageList, Mat &output);
void algebraicSum(vector<Mat> &imageList, Mat &output);
void algebraicProd(vector<Mat> &imageList, Mat &output);
void boundedSum(vector<Mat> &imageList, Mat &output);
void boundedProd(vector<Mat> &imageList, Mat &output);
void opening(vector<Mat> &imageList, Mat &output);
void closing(vector<Mat> &imageList, Mat &output);
void grayToBw(Mat & grayImage, Mat & bwImage);
double labelArea(Mat& bwImage, map<ushort, list<pair<int, int>>>& labeledPixels);