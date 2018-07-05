#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <memory>
#include <utility>

#include <numeric>
#include <algorithm>

#include <vector>
#include <stack>
#include <map>
#include <list>
#include <set>
#include <tuple>

#include <chrono>
#include <random>
#include <cstddef>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
using ImageFunction = void(*)(vector<Mat> &, Mat &);