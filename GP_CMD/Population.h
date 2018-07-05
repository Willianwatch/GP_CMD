#pragma once
#include "BinaryTree.h"

class Population
{
private:
	string trainingImageFolder;
	string targetImageFolder;
	vector<string> TrainingImageNames;
	vector<string> TargetImageNames;
	vector<Mat> trainingImages{};
	vector<Mat> inputImages{};
	vector<Mat> targetImages{};

	string testImageFolder;
	string testTargetFolder;
	string outputImageFolder;
	vector<string> testImageNames;
	vector<string> testTargetNames;
	vector<Mat> testImages{};
	vector<Mat> testInputImages{};
	vector<Mat> testTargetImages{};
	vector<Mat> outputImages{};

	vector<shared_ptr<BinaryTree>> population{};

	std::size_t generationGap = 60;
	double crossoverRatio = 1;
	double mutateRatio = 0.9;
	std::size_t populationSize = 500;
	unsigned maxIterationTime = 1000;
	unsigned recordInterval = 1000;

	pair<double, unsigned> parsimonyFactor{0.0, 0};

	size_t choice(size_t i, size_t j);
	void getInputImage(bool needPreProcess);
	void mgg(unsigned generation);
	void competition(unsigned generation);
	std::size_t findBest();
	std::size_t findOverallBest();
	static bool cmp(shared_ptr<BinaryTree> a, shared_ptr<BinaryTree> b);

	void log(unsigned generation, size_t bestPos);
	void changesRecord(unsigned generation, size_t bestPos);
	vector<Mat> filtersVote();
	void voteChangesRecord(unsigned generation);

	double getRandomReal();
	std::size_t getRandom(std::size_t first, std::size_t last);

	void penaltyFunction();
public:
	vector<tuple<unsigned, double, unsigned>> records;	
	size_t overallBestPos = 0;

	Population(unsigned maxGeneration, std::size_t sz);
	Population() = default;
	~Population();
	
	size_t size();
	void push_back(shared_ptr<BinaryTree> treePointer);
	shared_ptr<BinaryTree>& operator[](size_t n);
	const shared_ptr<BinaryTree>& operator[](size_t n) const;

	void training();
	void supressBloat();
};

