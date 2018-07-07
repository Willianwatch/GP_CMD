#include "stdafx.h"
#include "Population.h"

void Population::penaltyFunction()
{
	for (auto i : population)
		i->overallFitness = 0.1 * parsimonyFactor.first *(1 / (1 + static_cast<double>(parsimonyFactor.second) / i->root->family)) + i->fitness;
}

Population::Population(unsigned maxGeneration, std::size_t sz)
{
	trainingImageFolder = String("TrainingDataSet/TrainingImages/");
	targetImageFolder = String("TrainingDataSet/TargetImages/");
	TrainingImageNames = vector<string>({  "3.bmp", "34.bmp", "35.bmp", "50.bmp" });
	TargetImageNames = vector<string>({  "3.tif", "34.tif", "35.tif", "50.tif" });

	testImageFolder = String("TestDataSet/TestImages/");
	testTargetFolder = String("TestDataSet/OriginalImages/");
	outputImageFolder = String("TestDataSet/OutputImages/");
	testImageNames = vector<string>();
	for (int i = 0; i != 51; ++i)
	{
		testImageNames.push_back(to_string(i) + ".bmp");
	}
	testTargetNames = vector<string>({  "3.jpg", "4.jpg", "8.jpg", "7.jpg", "13.jpg", "25.jpg", "31.jpg", "34.jpg", "35.jpg", "38.jpg", "39.jpg", "50.jpg" });

	maxIterationTime = maxGeneration;
	populationSize = sz;

	for (std::size_t i = 0; i != TrainingImageNames.size(); ++i)
	{
		Mat trainingImage = imread(trainingImageFolder + TrainingImageNames[i], 0);
		trainingImages.push_back(trainingImage);

		Mat targetImage = imread(targetImageFolder + TargetImageNames[i], 0);
		targetImages.push_back(targetImage);
	}
	getInputImage(false);

	for (size_t i = 0; i != testImageNames.size(); ++i)
	{
		Mat testImage = imread(testImageFolder + testImageNames[i], 0);
		testImages.push_back(testImage);

		//Mat testTargetImage = imread(testTargetFolder + testTargetNames[i], 0);
		//testTargetImages.push_back(testTargetImage);
	}
	getInputImage(false);


	for (std::size_t i = 0; i != sz; ++i)
	{
		shared_ptr<BinaryTree> tree = make_shared<BinaryTree>(2);
		tree->setTreeInput(inputImages, targetImages);
		tree->getTreeOutput();
		tree->calculateFitness();
		population.push_back(tree);
	}

	supressBloat();
	records.push_back(make_tuple(0, population[overallBestPos]->fitness, population[overallBestPos]->root->family));

	log(0, overallBestPos);
}

Population::~Population()
{
}

size_t Population::size()
{
	return population.size();
}

void Population::push_back(shared_ptr<BinaryTree> treePointer)
{
	population.push_back(treePointer);
}

shared_ptr<BinaryTree>& Population::operator[](size_t n)
{
	return population[n];
}

const shared_ptr<BinaryTree>& Population::operator[](size_t n) const
{
	return population[n];
}

void Population::training()
{
	for (unsigned generation = 0; generation != maxIterationTime; ++generation)
	{
		mgg(generation + 1);
	}
}

void Population::supressBloat()
{
	size_t bestPos = findBest();
	parsimonyFactor = make_pair(population[bestPos]->fitness, population[bestPos]->root->family);
	penaltyFunction();
	overallBestPos = findOverallBest();
}

size_t Population::choice(size_t i, size_t j)
{
	if (population[i]->overallFitness <= population[j]->overallFitness)
		return i;
	else
		return j;
}

void Population::getInputImage(bool needPreProcess)
{
	if (needPreProcess)
	{
		double choosenMean = 0.0;
		double choosenStd = 0.1;
		vector<double> meanVec;
		vector<double> stdVec;
		for (auto &i : trainingImages)
		{
			Mat tempMean, tempStd;
			meanStdDev(i, tempMean, tempStd);
			double meanValue = tempMean.at<double>(0, 0);
			double stdValue = tempStd.at<double>(0, 0);
			meanVec.push_back(meanValue);
			stdVec.push_back(stdValue);
			if ((meanValue / stdValue) > (choosenMean / choosenStd))
			{
				choosenMean = meanValue;
				choosenStd = stdValue;
			}
		}

		for (std::size_t i = 0; i != trainingImages.size(); ++i)
		{
			Mat trainingImage;
			trainingImages[i].convertTo(trainingImage, CV_32FC1);
			Mat inputImage = (choosenStd / stdVec[i]) * (trainingImage - meanVec[i]) + choosenMean;
			inputImage.convertTo(inputImage, CV_8UC1);
			inputImages.push_back(inputImage);
		}
	}
	else
	{
		inputImages = trainingImages;
	}
}

void Population::mgg(unsigned generation)
{
	std::size_t firstPos = getRandom(0, populationSize - 1);
	std::size_t secondPos = getRandom(0, populationSize - 1);
	while (firstPos == secondPos)
		secondPos = getRandom(0, populationSize - 1);

	Population newPopulation;
	newPopulation.push_back(population[firstPos]);
	newPopulation.push_back(population[secondPos]);

	size_t numOfChildren = 0;
	while (numOfChildren != generationGap)
	{
		double crossoverRandom = getRandomReal();
		if (crossoverRandom <= crossoverRatio)
		{
			numOfChildren += 2;
			vector<shared_ptr<BinaryTree>> offsprings(population[firstPos]->sizeFairCrossover(population[secondPos]));
			double mutateRandom = 0.0;
			for (auto i : offsprings)
			{
				mutateRandom = getRandomReal();
				if (mutateRandom <= mutateRatio)
					i->mutate(getRandom(0, 3));
				i->getTreeOutput();
				i->calculateFitness();
				newPopulation.push_back(i);
			}
		}
	}

	newPopulation.supressBloat();
	vector<double> fitnessList{};
	for (size_t it = 0; it != newPopulation.size(); ++it)
	{
		if (it == newPopulation.overallBestPos)
			continue;
		else
			fitnessList.push_back(newPopulation[it]->overallFitness);
	}

	vector<double> rouletteRatio{};
	double sum = accumulate(fitnessList.begin(), fitnessList.end(), 0.0);
	double currentSum = 0.0;
	for (auto vec_it = fitnessList.cbegin(); vec_it != fitnessList.cend(); ++vec_it)
	{
		currentSum += *vec_it;
		rouletteRatio.push_back(currentSum / sum);
	}
	double selectRatio = getRandomReal();
	std::size_t it = 0;
	for (; it != rouletteRatio.size(); ++it)
	{
		if (selectRatio <= rouletteRatio[it])
			break;
	}
	std::size_t rouletteOne = 0;
	if (it < newPopulation.overallBestPos)
		rouletteOne = it;
	else
		rouletteOne = it + 1;
	population[firstPos] = newPopulation[newPopulation.overallBestPos];
	population[secondPos] = newPopulation[rouletteOne];

	supressBloat();
	records.push_back(make_tuple(generation, population[overallBestPos]->fitness, population[overallBestPos]->root->family));
	log(generation, overallBestPos);
	voteChangesRecord(generation);
	//changesRecord(generation, overallBestPos);

	mutateRatio -= 0.8 / maxIterationTime;
}

void Population::competition(unsigned generation)
{
	vector<shared_ptr<BinaryTree>> newPopulation;
	size_t i = 0, j = 0;
	vector<bool> label(populationSize, false);
	vector<size_t> temp;
	for (size_t index = 0; index != populationSize / 2; ++index)
	{
		size_t chooseni = getRandom(0, populationSize - 1 - 2 * index);

		for (size_t counti = 0; counti != chooseni; )
		{
			if (label[i])
				++i;
			else
				++counti;
		}
		size_t choosenj = getRandom(0, populationSize - 2 - 2 * index);
		for (size_t countj = 0; countj != choosenj; )
		{
			if (label[j])
				++j;
			else
				++countj;
		}
		
		i = choice(j, i);
		temp.push_back(i);
		label[i] = true;
	}
	while (newPopulation.size() < populationSize)
	{
		if (getRandomReal() <= crossoverRatio)
		{
			i = getRandom(0, populationSize / 2 - 1);
			do {
				j = getRandom(0, populationSize / 2 - 1);
			} while (i == j);
			vector<shared_ptr<BinaryTree>> offsprings = population[i]->sizeFairCrossover(population[j]);
			for (auto child : offsprings)
			{
				if (getRandomReal() <= mutateRatio)
					child->mutate(getRandom(0, 3));
				child->getTreeOutput();
				child->calculateFitness();

				newPopulation.push_back(child);
			}
		}
	}

	population.insert(population.end(), newPopulation.begin(), newPopulation.end());
	supressBloat();
	sort(population.begin(), population.end(), [](shared_ptr<BinaryTree> a, const shared_ptr<BinaryTree> b) 
	{
		return a->overallFitness < b->overallFitness;
	});

	population.erase(population.begin() + populationSize);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(population.begin(), population.end(), std::default_random_engine(seed));

	supressBloat();
	records.push_back(make_tuple(generation, population[overallBestPos]->fitness, population[overallBestPos]->root->family));
	log(generation, overallBestPos);
	voteChangesRecord(generation);
	//changesRecord(generation, overallBestPos);

	mutateRatio -= 0.8 / maxIterationTime;
}

std::size_t Population::findBest()
{
	std::size_t sz = population.size();
	std::size_t pos = 0;
	double biggestFitness = population[pos]->fitness;
	for (std::size_t i = 1; i != sz; ++i)
	{
		if (population[i]->fitness < biggestFitness)
		{
			biggestFitness = population[i]->fitness;
			pos = i;
		}
	}
	return pos;
}

std::size_t Population::findOverallBest()
{
	std::size_t sz = population.size();
	std::size_t pos = 0;
	double biggestOverallFitness = population[pos]->overallFitness;
	for (std::size_t i = 1; i != sz; ++i)
	{
		if (population[i]->overallFitness < biggestOverallFitness)
		{
			biggestOverallFitness = population[i]->overallFitness;
			pos = i;
		}
	}
	return pos;
}

bool Population::cmp(shared_ptr<BinaryTree> a, shared_ptr<BinaryTree> b)
{
	return a->overallFitness<b->overallFitness;
}

void Population::log(unsigned generation, size_t bestPos)
{
	cout.precision(12);
	cout << setfill('0') << right << "Generation:" << setw(5) << generation << " "
		<< "Fitness:" << population[bestPos]->fitness << " "
		<< "Nodes:" << setw(3) << population[bestPos]->root->family << setfill(' ') << endl;
}

void Population::changesRecord(unsigned generation, size_t bestPos)
{
	if (generation % recordInterval != 0);
	else
	{


		string generationString = to_string(generation);
		BinaryTree optimal = *(population[bestPos]);
		optimal.setTreeInput(testImages, testTargetImages);
		optimal.getTreeOutput();
		optimal.calculateFitness();

		for (size_t i = 0; i != optimal.output.size(); ++i)
		{
			cout << optimal.fitness << endl;
			imwrite(outputImageFolder + generationString + "/" + testImageNames[i], optimal.output[i]);
		}
	}
}

vector<Mat> Population::filtersVote()
{
	vector<shared_ptr<BinaryTree>> tempPopulation(population);
	sort(tempPopulation.begin(), tempPopulation.end(), cmp);
	for (auto i : tempPopulation)
		cout << i->overallFitness << " ";
	cout << endl;

	vector<Mat> voteResults;
	for (auto &image : testImages)
	{
		voteResults.push_back(Mat::ones(image.size(), CV_8UC1)*255);
	}

	//vector<double> filtersWeight;
	//for (size_t i = 0; i != 20; ++i)
	//{
	//	filtersWeight.push_back(1 / tempPopulation[i]->overallFitness);
	//}
	//double weightsSum = std::accumulate(filtersWeight.cbegin(), filtersWeight.cend(), 0.0);
	//for (size_t i = 0; i != 20; ++i)
	//{
	//	filtersWeight[i] /= weightsSum;
	//}

	for (size_t i = 0; i != 5; ++i)
	{
		BinaryTree goodTree = *(tempPopulation[i]);
		goodTree.setTreeInput(testImages, testTargetImages);
		goodTree.getTreeOutput();
		for (size_t j = 0; j != voteResults.size(); ++j)
		{
			Mat image = (goodTree.output[j] <= 32);
			cv::bitwise_and(voteResults[j], image, voteResults[j]);
		}
	}

	//for (auto &image : voteResults)
	//{
	//	image.convertTo(image, CV_8UC1);
	//	cv::threshold(image, image, 30, 255, THRESH_BINARY);
	//}

	return voteResults;
}

void Population::voteChangesRecord(unsigned generation)
{
	if (generation % recordInterval != 0);
	else
	{
		string fileName = to_string(generation) + ".txt";
		population[overallBestPos]->serialization(fileName);

		string generationString = to_string(generation);
		vector<Mat> voteResults = filtersVote();
		for (size_t i = 0; i != voteResults.size(); ++i)
		{
			imwrite(outputImageFolder + generationString + "/" + testImageNames[i], voteResults[i]);
		}
	}
}

double Population::getRandomReal()
{
	static default_random_engine e;
	static uniform_real_distribution<double> u(0, 1);
	return u(e);
}

std::size_t Population::getRandom(std::size_t first, std::size_t last)
{
	static uniform_int_distribution<std::size_t> u(0, 999);
	static default_random_engine e;
	u = uniform_int_distribution<std::size_t>(first, last);
	return u(e);
}
