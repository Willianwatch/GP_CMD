#pragma once
#include "FilterNode.h"

class BinaryTree
{
public:
	//宏
	using preFunction = void(*)(shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> node);
	using postFunction = void(*)(BinaryTree *const &pointer, vector<shared_ptr<FilterNode>> &vec);

	//属性
	vector<Mat> input;
	vector<Mat> original;
	vector<Mat> output;
	vector<double> ratioVec;
	shared_ptr<FilterNode> root = make_shared<FilterNode>();
	vector<shared_ptr<FilterNode>> nodes{ root };
	vector<shared_ptr<FilterNode>> noChildNodes{ root };
	vector<shared_ptr<FilterNode>> oneChildNodes;
	vector<shared_ptr<FilterNode>> twoChildNodes;
	vector<shared_ptr<FilterNode>> oneInputNodes{ root };
	double fitness = 0.0;
	double overallFitness = 0.0;

	/*************************构造函数************************/
	BinaryTree() = default;
	BinaryTree(vector<Mat> noiseImageList, vector<Mat> originalImageList);

	BinaryTree(unsigned num);
	static void free(BinaryTree* thisTree);
	void addNodes();
	void insert(shared_ptr<FilterNode> node, bool noOrOne, bool leftOrRight);
	void setNodeFunctions();

	BinaryTree(const BinaryTree &orig);
	void copyInsert(shared_ptr<FilterNode> node);
	void copyInsert(shared_ptr<FilterNode> node, bool leftOrRight);
	/********************************************************/

	/**********************析构函数***************************/
	~BinaryTree();
	static void destruct(BinaryTree *const &pointer, vector<shared_ptr<FilterNode>> &vec);
	static void getResult(BinaryTree *const &pointer, vector<shared_ptr<FilterNode>> &vec);
	static void transfer(shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> node);
	void preOrderTraverse(preFunction function, shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> choosenNode);
	void postOrderTraverse(postFunction function, shared_ptr<FilterNode> choosenNode);
	/********************************************************/

	//交叉
	void crossover(shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> thisNode, shared_ptr<FilterNode> thatNode);
	vector<shared_ptr<BinaryTree>> sizeFairCrossover(shared_ptr<BinaryTree> anotherTree);

	//变异
	void mutate(std::size_t sz);
	void mutateReplace();
	void mutateDelete();
	void mutateInsert();
	void mutateChange();

	void setTreeInput(vector<Mat> &noiseImages, vector<Mat> &grayImages);
	void getTreeOutput();
	void calculateFitness();

	//序列化和反序列化
	BinaryTree(String sourceFile);
	void serialization(string destinationFile);

	//辅助函数
	void remove(vector<shared_ptr<FilterNode>> &vec, shared_ptr<FilterNode> node);
	std::size_t getRandomInteger(std::size_t first, std::size_t last);
	bool getRandomBool();
	float getRandomReal();
};

