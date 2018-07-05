#pragma once
#include "FUH.h"
#include "Gene.h"

static vector<ImageFunction> oneInputFunction{ mean, min, max, sobel, lightPixel, darkPixel, largeArea, smallArea, inversion};
static vector<ImageFunction> twoInputFunction{ logicalSum, logicalProd, algebraicProd, algebraicSum, boundedProd, boundedSum };

static vector<ImageFunction>::size_type optionalRange1 = oneInputFunction.size();
static vector<ImageFunction>::size_type optionalRange2 = twoInputFunction.size();

class FilterNode
{
public:
	//����
	shared_ptr<FilterNode>  parents = nullptr;
	shared_ptr<FilterNode> leftChild = nullptr;
	shared_ptr<FilterNode> rightChild = nullptr;
	unsigned family = 1;
	vector<Mat> input;
	pair<bool, size_t> functionMessage;
	Mat output;

	//���캯��
	FilterNode() = default;
	FilterNode(const FilterNode &orig);
	FilterNode(const pair<bool, size_t> message);

	//��������
	~FilterNode();

	//��ֵ���������
	FilterNode &operator=(const FilterNode &orig);

	void getInput();
	void getOutput();
};

