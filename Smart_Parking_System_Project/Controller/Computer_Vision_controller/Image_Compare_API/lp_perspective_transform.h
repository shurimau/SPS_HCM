#ifndef _LP_PERSPECTIVE_TRANSFORM_HPP_
#define _LP_PERSPECTIVE_TRANSFORM_HPP_

#include <iostream>
#include <string.h>
#include <queue>
#include <iterator>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define kR 0.299
#define kG 0.587
#define kB 0.144
#define MAX_8_BIT_VALUE 255

class PerspectiveTransform
{
protected:
    vector<int> classIds;
    vector<float> scores;
    vector<Rect> boxes;
    vector<string> classes;
    float confidence_threshold;
    float nms_threshold;

public:
    PerspectiveTransform(void);
    float LuminanceValue(Mat image);
    Mat Transform(Mat image);
};

#endif