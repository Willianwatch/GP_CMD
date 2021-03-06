// GP_CMD.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Population.h"

int main()
{
	string command;
	cout << "train(n), test(t) or try(y):" << endl;
	cin >> command;
	if (command == "n")
	{
		Population terminator(10000, 80);
		terminator.training();

		ofstream output;
		output.open("TestDataSet/OutputImages/data.txt");
		for (auto &i : terminator.records)
		{
			output << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;
		}
		output.close();
	}
	else if (command == "t")
	{
		string sourceFile("1000.txt");
		BinaryTree tree(sourceFile);
		vector<Mat> input{ cv::imread("TestDataSet/TestImages/38.bmp", 0) };
		vector<Mat> original{ cv::imread("TestDataSet/OriginalImages/38.jpg", 0) };
		tree.setTreeInput(input, original);
		tree.getTreeOutput();
		imshow("filtered", tree.output[0]);
		waitKey();
	}
	else
	{
	}
    return 0;
}   