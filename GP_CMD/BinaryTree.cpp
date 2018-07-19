#include "stdafx.h"
#include "BinaryTree.h"
#include "Gene.h"



BinaryTree::BinaryTree(vector<Mat> noiseImageList, vector<Mat> originalImageList):
	BinaryTree()
{
	addNodes();
	setNodeFunctions();
	setTreeInput(noiseImageList, originalImageList);
	getTreeOutput();
	calculateFitness();
}

BinaryTree::BinaryTree(unsigned num):
	BinaryTree()
{
	for (unsigned i = num - 1; i != 0; --i)
		addNodes();
	setNodeFunctions();
}

void BinaryTree::free(BinaryTree * thisTree)
{

}

void BinaryTree::addNodes()
{
	std::size_t noChildNodesNum = 2 * noChildNodes.size();
	std::size_t oneChildNodesNum = oneChildNodes.size();
	std::size_t addPos = getRandomInteger(0, noChildNodesNum + oneChildNodesNum - 1);
	if (addPos < noChildNodesNum)
	{
		shared_ptr<FilterNode> node(noChildNodes[(std::size_t)(addPos / 2)]);
		insert(node, false, static_cast<bool>(addPos % 2));
	}
	else
	{
		shared_ptr<FilterNode> node(oneChildNodes[addPos - noChildNodesNum]);
		(node->leftChild) ? insert(node, true, true) : insert(node, true, false);
	}
}

void BinaryTree::insert(shared_ptr<FilterNode> node, bool noOrOne, bool leftOrRight)
{
	shared_ptr<FilterNode> t = make_shared<FilterNode>();

	t->parents = node;
	if (leftOrRight)
		node->rightChild = t;
	else
		node->leftChild = t;

	shared_ptr<FilterNode> it(t);
	while (it->parents)
	{
		++(it->parents->family);
		it = it->parents;
	}

	nodes.push_back(t);
	noChildNodes.push_back(t);
	oneInputNodes.push_back(t);

	if (noOrOne)
	{
		remove(oneChildNodes, node);
		remove(oneInputNodes, node);
		twoChildNodes.push_back(node);
	}
	else
	{
		remove(noChildNodes, node);
		oneChildNodes.push_back(node);
	}
}

void BinaryTree::setNodeFunctions()
{
	for (auto i : oneInputNodes)
		i->functionMessage = make_pair(false, getRandomInteger(0, optionalRange1 - 1));
	for (auto i : twoChildNodes)
		i->functionMessage = make_pair(false, getRandomInteger(0, optionalRange2 - 1));
}

BinaryTree::BinaryTree(const BinaryTree & orig):
	input(orig.input),
	original(orig.original),
	ratioVec(orig.ratioVec),
	fitness(orig.fitness)
{
	*(root) = *(orig.root);
	stack<shared_ptr<FilterNode>> origStack{};
	stack<shared_ptr<FilterNode>> copyStack{};
	shared_ptr<FilterNode> node(root);
	shared_ptr<FilterNode> origNode(orig.root);
	while (origNode || (!origStack.empty()))
	{
		while (origNode)
		{
			if (!(origNode->leftChild) && !(origNode->rightChild));
			else if (origNode->leftChild && origNode->rightChild)
			{
				copyInsert(node);
				*(node->leftChild) = *(origNode->leftChild);
				*(node->rightChild) = *(origNode->rightChild);
			}
			else if (origNode->leftChild)
			{
				copyInsert(node, false);
				*(node->leftChild) = *(origNode->leftChild);
			}
			else
			{
				copyInsert(node, true);
				*(node->rightChild) = *(origNode->rightChild);
			}
			origStack.push(origNode);
			copyStack.push(node);
			origNode = origNode->leftChild;
			node = node->leftChild;
		}
		if (!(origStack.empty()))
		{
			origNode = origStack.top();
			node = copyStack.top();
			origStack.pop();
			copyStack.pop();
			origNode = origNode->rightChild;
			node = node->rightChild;
		}
	}
}

void BinaryTree::copyInsert(shared_ptr<FilterNode> node)
{
	shared_ptr<FilterNode> t1 = make_shared<FilterNode>();
	shared_ptr<FilterNode> t2 = make_shared<FilterNode>();
	t1->parents = node;
	node->leftChild = t1;
	t2->parents = node;
	node->rightChild = t2;

	noChildNodes.push_back(t1);
	noChildNodes.push_back(t2);

	oneInputNodes.push_back(t1);
	oneInputNodes.push_back(t2);

	nodes.push_back(t1);
	nodes.push_back(t2);

	remove(noChildNodes, node);
	remove(oneInputNodes, node);

	twoChildNodes.push_back(node);
}

void BinaryTree::copyInsert(shared_ptr<FilterNode> node, bool leftOrRight)
{
	shared_ptr<FilterNode> t = make_shared<FilterNode>();
	t->parents = node;
	if (leftOrRight)
		node->rightChild = t;
	else
		node->leftChild = t;

	nodes.push_back(t);

	noChildNodes.push_back(t);

	oneInputNodes.push_back(t);

	remove(noChildNodes, node);

	oneChildNodes.push_back(node);
}

BinaryTree::~BinaryTree()
{
	postOrderTraverse(destruct, root);
}

void BinaryTree::destruct(BinaryTree * const & pointer, vector<shared_ptr<FilterNode>>& vec)
{
	for (auto i = vec.rbegin(); i != vec.rend(); ++i)
	{
		(*i)->parents.reset();
		(*i)->leftChild.reset();
		(*i)->rightChild.reset();
	}
}

void BinaryTree::getResult(BinaryTree * const & pointer, vector<shared_ptr<FilterNode>>& vec)
{
	pointer->output.clear();
	for (auto i : pointer->input)
	{
		for (auto j = pointer->noChildNodes.begin(); j != pointer->noChildNodes.end(); ++j)
			(*j)->input = { i };
		for (auto k = vec.rbegin(); k != vec.rend(); ++k)
			(*k)->getOutput();
		pointer->output.push_back(pointer->root->output);
	}
}

void BinaryTree::transfer(shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> node)
{
	thisTree->remove(thisTree->nodes, node);
	thatTree->nodes.push_back(node);
	if (!(node->leftChild) && !(node->rightChild))
	{
		thisTree->remove(thisTree->noChildNodes, node);
		thatTree->noChildNodes.push_back(node);

		thisTree->remove(thisTree->oneInputNodes, node);
		thatTree->oneInputNodes.push_back(node);
	}
	else if (node->leftChild && node->rightChild)
	{
		thisTree->remove(thisTree->twoChildNodes, node);
		thatTree->twoChildNodes.push_back(node);
	}
	else
	{
		thisTree->remove(thisTree->oneChildNodes, node);
		thatTree->oneChildNodes.push_back(node);

		thisTree->remove(thisTree->oneInputNodes, node);
		thatTree->oneInputNodes.push_back(node);
	}
}

void BinaryTree::preOrderTraverse(preFunction function, shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> choosenNode)
{
	shared_ptr<FilterNode> node(choosenNode);
	stack<shared_ptr<FilterNode>> thisStack{};
	while (node || !(thisStack.empty()))
	{
		while (node)
		{
			function(thisTree, thatTree, node);
			thisStack.push(node);
			node = node->leftChild;
		}
		if (!(thisStack.empty()))
		{
			node = thisStack.top();
			thisStack.pop();
			node = node->rightChild;
		}
	}
}

void BinaryTree::postOrderTraverse(postFunction function, shared_ptr<FilterNode> choosenNode)
{
	shared_ptr<FilterNode> node(choosenNode);
	stack<shared_ptr<FilterNode>> firstStack{};
	vector<shared_ptr<FilterNode>> secondStack{};
	firstStack.push(node);
	while (!(firstStack.empty()))
	{
		node = firstStack.top();
		firstStack.pop();
		if (node->leftChild)
			firstStack.push(node->leftChild);
		if (node->rightChild)
			firstStack.push(node->rightChild);
		secondStack.push_back(node);
	}
	function(this, secondStack);
}

void BinaryTree::crossover(shared_ptr<BinaryTree> thisTree, shared_ptr<BinaryTree> thatTree, shared_ptr<FilterNode> thisNode, shared_ptr<FilterNode> thatNode)
{
	//更改每个父辈节点的family信息
	shared_ptr<FilterNode> node;
	if (thisNode->family == thatNode->family);
	else if (thisNode->family > thatNode->family)
	{
		unsigned sizeDis = thisNode->family - thatNode->family;
		node = thisNode;
		while (node->parents)
		{
			node->parents->family -= sizeDis;
			node = node->parents;
		}
		node = thatNode;
		while (node->parents)
		{
			node->parents->family += sizeDis;
			node = node->parents;
		}
	}
	else
	{
		unsigned sizeDis = thatNode->family - thisNode->family;
		node = thisNode;
		while (node->parents)
		{
			node->parents->family += sizeDis;
			node = node->parents;
		}
		node = thatNode;
		while (node->parents)
		{
			node->parents->family -= sizeDis;
			node = node->parents;
		}
	}

	//更改记录树的状态的vector的信息
	node = thisNode;
	preOrderTraverse(transfer, thisTree, thatTree, node);

	node = thatNode;
	preOrderTraverse(transfer, thatTree, thisTree, node);

	//交换子树
	shared_ptr<FilterNode> thisNodeParents(thisNode->parents);
	shared_ptr<FilterNode> thatNodeParents(thatNode->parents);
	if (thisNodeParents)
	{
		if (!(thisNodeParents->leftChild))
			thisNodeParents->rightChild = thatNode;
		else if (thisNodeParents->leftChild == thisNode)
			thisNodeParents->leftChild = thatNode;
		else
			thisNodeParents->rightChild = thatNode;
		thatNode->parents = thisNodeParents;
	}
	else
	{
		thatNode->parents.reset();
		thisTree->root = thatNode;
	}
	if (thatNodeParents)
	{
		if (!(thatNodeParents->leftChild))
			thatNodeParents->rightChild = thisNode;
		else if (thatNodeParents->leftChild == thatNode)
			thatNodeParents->leftChild = thisNode;
		else
			thatNodeParents->rightChild = thisNode;
		thisNode->parents = thatNodeParents;
	}
	else
	{
		thisNode->parents.reset();
		thatTree->root = thisNode;
	}
}

vector<shared_ptr<BinaryTree>> BinaryTree::sizeFairCrossover(shared_ptr<BinaryTree> anotherTree)
{
	//将规模较小的树作为第一棵树
	shared_ptr<BinaryTree> thisTree = make_shared<BinaryTree>(*this);
	shared_ptr<BinaryTree> thatTree = make_shared<BinaryTree>(*anotherTree);

	//从规模较小的树上挑选出第一个节点，并用轮盘赌法挑选出第二个节点
	shared_ptr<FilterNode> thisNode(thisTree->nodes[getRandomInteger(0, thisTree->nodes.size() - 1)]);
	unsigned exchangeSize = thisNode->family;
	map<unsigned, vector<shared_ptr<FilterNode>>> sizeSub;
	for (auto i = thatTree->nodes.begin(); i != thatTree->nodes.end(); ++i)
	{
		unsigned distance = ((*i)->family >= exchangeSize) ? ((*i)->family - exchangeSize) : (exchangeSize - (*i)->family);
		sizeSub[distance].push_back(*i);
	}
	vector<unsigned> keyList{};
	vector<double> rouletteList{};
	for (auto map_it = sizeSub.cbegin(); map_it != sizeSub.cend(); ++map_it)
	{
		keyList.push_back(map_it->first);
		rouletteList.push_back(1.0 / pow(1.5, map_it->first));//此处theta的值是一个待定参数
	}
	vector<double> rouletteRatio{};
	double sum = accumulate(rouletteList.begin(), rouletteList.end(), 0.0);
	double currentSum = 0.0;
	for (auto vec_it = rouletteList.cbegin(); vec_it != rouletteList.cend(); ++vec_it)
	{
		currentSum += *vec_it;
		rouletteRatio.push_back(currentSum / sum);
	}
	double selectRatio = getRandomReal();
	size_t pos = 0;
	for (; pos != rouletteRatio.size(); ++pos)
	{
		if (selectRatio <= rouletteRatio[pos])
			break;
	}
	vector<shared_ptr<FilterNode>> &vec = sizeSub[keyList[pos]];
	shared_ptr<FilterNode> thatNode(vec[getRandomInteger(0, vec.size() - 1)]);

	//在节点选定之后，等尺寸交叉就如同普通的交叉一样，并无区别
	crossover(thisTree, thatTree, thisNode, thatNode);

	vector<shared_ptr<BinaryTree>> container{ thisTree, thatTree };
	return container;
}

void BinaryTree::mutate(std::size_t sz)
{
	switch (sz)
	{
	case 0:mutateReplace(); break;
	case 1:mutateDelete(); break;
	case 2:mutateInsert(); break;
	case 3:mutateChange(); break;
	default:break;
	}
}

void BinaryTree::mutateReplace()
{
	shared_ptr<BinaryTree> thisTree(this, &BinaryTree::free);
	shared_ptr<FilterNode> thisNode(nodes[getRandomInteger(0, nodes.size() - 1)]);
	unsigned treeSize = thisNode->family;
	shared_ptr<BinaryTree> thatTree = make_shared<BinaryTree>(treeSize);
	shared_ptr<FilterNode> thatNode(thatTree->root);

	//交换操作调用crossover函数
	crossover(thisTree, thatTree, thisNode, thatNode);
}

void BinaryTree::mutateDelete()
{
	if (!(oneInputNodes.empty()) && (nodes.size() != 1))
	{
		shared_ptr<FilterNode> thisNode(oneInputNodes[getRandomInteger(0, oneInputNodes.size() - 1)]);
		shared_ptr<FilterNode> thisNodeParents(thisNode->parents);//表示被删除结点的父节点
		shared_ptr<FilterNode> thisNodeChildren;

		//修改状态向量
		//修改与thisNode相关的状态向量及逻辑关系
		remove(nodes, thisNode);
		remove(oneInputNodes, thisNode);
		if (!(thisNode->leftChild) && !(thisNode->rightChild))
			remove(noChildNodes, thisNode);
		else
		{
			remove(oneChildNodes, thisNode);
			thisNodeChildren = (thisNode->leftChild ? thisNode->leftChild : thisNode->rightChild);
			thisNodeChildren->parents = thisNodeParents;
		}

		//修改与thisNodeParents相关的状态向量及逻辑关系
		if (thisNodeParents)
		{
			if (thisNodeParents->leftChild && thisNodeParents->rightChild)
			{
				remove(twoChildNodes, thisNodeParents);
				oneChildNodes.push_back(thisNodeParents);
				thisNodeParents->functionMessage = make_pair(false, getRandomInteger(0, optionalRange1 - 1));
			}
			else
			{
				remove(oneChildNodes, thisNodeParents);
				remove(oneInputNodes, thisNodeParents);
				noChildNodes.push_back(thisNodeParents);
			}

			thisNodeParents->leftChild == thisNode ? thisNodeParents->leftChild = thisNodeChildren : thisNodeParents->rightChild = thisNodeChildren;

			shared_ptr<FilterNode> node(thisNodeParents);
			while (node)
			{
				--(node->family);
				node = node->parents;
			}
		}
		else
			root = thisNodeChildren;
	}
}

void BinaryTree::mutateInsert()
{
	shared_ptr<FilterNode> thisNode(nodes[getRandomInteger(0, nodes.size() - 1)]);
	shared_ptr<FilterNode> thatNode = make_shared<FilterNode>();
	thatNode->parents = thisNode;
	nodes.push_back(thatNode);
	if (!(thisNode->leftChild) && !(thisNode->rightChild))//左子节点和右子节点都为空
	{
		thatNode->functionMessage = make_pair(false, getRandomInteger(0, optionalRange1 - 1));

		//修改逻辑关系
		if (getRandomBool())
			thisNode->rightChild = thatNode;
		else
			thisNode->leftChild = thatNode;

		//修改状态向量
		remove(noChildNodes, thisNode);
		oneChildNodes.push_back(thisNode);

		noChildNodes.push_back(thatNode);
		oneInputNodes.push_back(thatNode);
	}
	else if (thisNode->leftChild && thisNode->rightChild)
	{
		thatNode->functionMessage = make_pair(false, getRandomInteger(0, optionalRange1 - 1));

		//修改逻辑关系		
		if (getRandomBool())
		{
			thatNode->rightChild = thisNode->rightChild;
			thatNode->family = thatNode->rightChild->family + 1;
			thatNode->rightChild->parents = thatNode;
			thisNode->rightChild = thatNode;
		}
		else
		{
			thatNode->leftChild = thisNode->leftChild;
			thatNode->family = thatNode->leftChild->family + 1;
			thatNode->leftChild->parents = thatNode;
			thisNode->leftChild = thatNode;
		}

		oneChildNodes.push_back(thatNode);
		oneInputNodes.push_back(thatNode);

	}
	else if (thisNode->leftChild)//单亲结点，左子树存在，右子树为空
	{
		if (getRandomBool())//插入位置在右侧
		{
			thatNode->functionMessage = thisNode->functionMessage;
			thisNode->functionMessage = make_pair(true, getRandomInteger(0, optionalRange2 - 1));//父节点所代表的滤波器形式改变
			thisNode->rightChild = thatNode;

			noChildNodes.push_back(thatNode);
			oneInputNodes.push_back(thatNode);
			remove(oneChildNodes, thisNode);
			remove(oneInputNodes, thisNode);
			twoChildNodes.push_back(thisNode);
		}
		else
		{
			thatNode->functionMessage = make_pair(false, getRandomInteger(0, optionalRange1 - 1));

			//修改逻辑关系
			thatNode->leftChild = thisNode->leftChild;
			thatNode->family = thatNode->leftChild->family + 1;
			thatNode->leftChild->parents = thatNode;
			thisNode->leftChild = thatNode;

			//修改状态向量
			oneChildNodes.push_back(thatNode);
			oneInputNodes.push_back(thatNode);
		}
	}
	else//单亲节点，左子树为空，右子树存在
	{
		if (getRandomBool())//插入位置在右侧
		{
			thatNode->functionMessage = make_pair(false, getRandomInteger(0, optionalRange1 - 1));

			//修改逻辑关系		
			thatNode->rightChild = thisNode->rightChild;
			thatNode->family = thatNode->rightChild->family + 1;
			thatNode->rightChild->parents = thatNode;
			thisNode->rightChild = thatNode;

			//修改状态向量
			oneChildNodes.push_back(thatNode);
			oneInputNodes.push_back(thatNode);
		}
		else
		{
			//修改逻辑关系
			thatNode->functionMessage = thisNode->functionMessage;
			thisNode->functionMessage = make_pair(true, getRandomInteger(0, optionalRange2 - 1));
			thisNode->leftChild = thatNode;

			//修改状态向量
			noChildNodes.push_back(thatNode);
			oneInputNodes.push_back(thatNode);
			remove(oneChildNodes, thisNode);
			remove(oneInputNodes, thisNode);
			twoChildNodes.push_back(thisNode);
		}
	}

	shared_ptr<FilterNode> node(thatNode);
	while (node->parents)
	{
		++(node->parents->family);
		node = node->parents;
	}
}

void BinaryTree::mutateChange()
{
	if (!(oneInputNodes.empty()))
	{
		shared_ptr<FilterNode> node(oneInputNodes[getRandomInteger(0, oneInputNodes.size() - 1)]);
		pair<bool, size_t> func = make_pair(false, getRandomInteger(0, optionalRange1 - 1));
		while (node->functionMessage == func)
			func = make_pair(false, getRandomInteger(0, optionalRange1 - 1));
		node->functionMessage = func;
	}
}

void BinaryTree::setTreeInput(vector<Mat>& noiseImages, vector<Mat>& grayImages)
{
	input = noiseImages;
	original = grayImages;
	vector<int> widthVec;
	vector<int> heightVec;
	for (const auto &image : input)
	{
		widthVec.push_back(image.cols);
		heightVec.push_back(image.rows);
	}
	double w_average = accumulate(widthVec.begin(), widthVec.end(), 0.0) / static_cast<double>(widthVec.size());
	double h_average = accumulate(heightVec.begin(), heightVec.end(), 0.0) / static_cast<double>(heightVec.size());
	double s_average = w_average * h_average;
	for (std::size_t i = 0; i != widthVec.size(); ++i)
		ratioVec.push_back(sqrt(static_cast<double>(widthVec[i] * heightVec[i]) / (s_average)));
}

void BinaryTree::getTreeOutput()
{
	postOrderTraverse(getResult, root);
}

void BinaryTree::calculateFitness()
{	
	vector<double> performanceVec;
	std::size_t imagesNum = input.size();
	for (std::size_t i = 0; i != imagesNum; ++i)
	{
		Mat AbsResidual = Mat::zeros(output[i].size(), CV_8UC1);
		absdiff(output[i], original[i], AbsResidual);
		AbsResidual.convertTo(AbsResidual, CV_32F);
		Mat weightedImage = (AbsResidual <= 32) + (AbsResidual > 32) / 2;
		weightedImage.convertTo(weightedImage, CV_32F);
		performanceVec.push_back(ratioVec[i] * (sum(weightedImage.mul(AbsResidual)).val[0]) / (sum(weightedImage * 255).val[0]));
	}
	fitness = accumulate(performanceVec.begin(), performanceVec.end(), 0.0) / static_cast<double>(performanceVec.size());
}

BinaryTree::BinaryTree(String sourceFile):
	BinaryTree()
{
	ifstream inputFileStream(sourceFile);
	string line;
	istringstream lineStream;
	int firstNum = 0, secondNum = 0;

	getline(inputFileStream, line);
	lineStream.str(line);
	lineStream >> firstNum >> secondNum;
	root->functionMessage = make_pair(static_cast<bool>(firstNum), static_cast<size_t>(secondNum));

	shared_ptr<FilterNode> node(root);
	stack<shared_ptr<FilterNode>> nodeStack{};
	while (node || !(nodeStack.empty()))
	{
		while (node)
		{
			line.clear();
			getline(inputFileStream, line);
			auto space = line.find_first_of(" ");
			firstNum = stoi(line.substr(0, space));
			secondNum = stoi(line.substr(space + 1, line.size()));

			if (firstNum != -1)
				node->leftChild = make_shared<FilterNode>(make_pair(static_cast<bool>(firstNum), static_cast<size_t>(secondNum)));
			nodeStack.push(node);
			node = node->leftChild;
		}
		if (!(nodeStack.empty()))
		{
			node = nodeStack.top();
			nodeStack.pop();

			line.clear();
			getline(inputFileStream, line);
			auto space = line.find_first_of(" ");
			firstNum = stoi(line.substr(0, space));
			secondNum = stoi(line.substr(space + 1, line.size()));

			if (firstNum != -1)
				node->rightChild = make_shared<FilterNode>(make_pair(static_cast<bool>(firstNum), static_cast<size_t>(secondNum)));
			node = node->rightChild;
		}
	}

	node = root;
	while (node || !(nodeStack.empty()))
	{
		while (node)
		{
			if (node->leftChild && node->rightChild)
				twoChildNodes.push_back(node);
			else
			{
				oneInputNodes.push_back(node);
				if (!(node->leftChild) && !(node->rightChild))
					noChildNodes.push_back(node);
				else
					oneChildNodes.push_back(node);
			}
			nodeStack.push(node);
			node = node->leftChild;
		}
		if (!(nodeStack.empty()))
		{
			node = nodeStack.top();
			nodeStack.pop();
			node = node->rightChild;
		}
	}

	inputFileStream.close();
	
}

void BinaryTree::serialization(string destinationFile)
{
	ofstream outputFileStream(destinationFile);
	outputFileStream << root->functionMessage.first << " " << root->functionMessage.second << endl;

	shared_ptr<FilterNode> node(root);
	stack<shared_ptr<FilterNode>> nodeStack{};
	while (node || !(nodeStack.empty()))
	{
		while (node)
		{
			if (node->leftChild)
				outputFileStream << node->leftChild->functionMessage.first << " "
				<< node->leftChild->functionMessage.second << endl;
			else
				outputFileStream << -1 << " " << -1 << endl;
			nodeStack.push(node);
			node = node->leftChild;
		};
		if (!(nodeStack.empty()))
		{
			node = nodeStack.top();
			nodeStack.pop();
			if (node->rightChild)
				outputFileStream << node->rightChild->functionMessage.first << " "
				<< node->rightChild->functionMessage.second << endl;
			else
				outputFileStream << -1 << " " << -1 << endl;
			node = node->rightChild;
		}
	};

	outputFileStream.close();
}

void BinaryTree::remove(vector<shared_ptr<FilterNode>>& vec, shared_ptr<FilterNode> node)
{
	auto pos = find(vec.begin(), vec.end(), node);
	vec.erase(pos);
}

std::size_t BinaryTree::getRandomInteger(std::size_t first, std::size_t last)
{
	static uniform_int_distribution<std::size_t> u(0, 999);
	static default_random_engine e;
	u = uniform_int_distribution<std::size_t>(first, last);
	return u(e);
}

bool BinaryTree::getRandomBool()
{
	static bernoulli_distribution u;
	static default_random_engine e;
	return u(e);
}

float BinaryTree::getRandomReal()
{
	static default_random_engine e;
	static uniform_real_distribution<float> u(0, 1);
	return u(e);
}


