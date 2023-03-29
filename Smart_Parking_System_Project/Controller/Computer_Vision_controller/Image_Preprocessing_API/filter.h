#ifndef FILTER_H
#define FILTER_H
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
class Filter
{
public:
    Filter();
    void medianFilter(Mat &input, int size, int mode);
    enum{
        MinimumFilter = 0,
        MedianFilter,
        MaximumFilter,
        AfterMedian
    };
};

#endif // FILTER_H
