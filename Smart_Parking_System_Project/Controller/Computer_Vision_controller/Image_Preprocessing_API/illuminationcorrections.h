#ifndef ILLUMINATIONCORRECTIONS_H
#define ILLUMINATIONCORRECTIONS_H
#include "opencv2/opencv.hpp"
//#include <QtCharts/QChartGlobal>
using namespace cv;
using namespace std;
class Illuminationcorrections
{
public:
    Illuminationcorrections();
    Mat Linght_correction(Mat inputImage, float alpha, float beta);
    int Max(Vec3b vec);
    float Average(Mat kernel, int x1, int y1, int x2, int y2m ,int chanel);
    Mat FourierTransform(Mat img);
    void RGBtoHSV(Vec3b &input);
    void RGB2HSVmatrix(Mat &RGBraw);
    void HSV2RGBmatrix(Mat &HSVmatrix);
    void imageHistogram(Mat image, int histogram[]);
    void cumulativeImageHistogram(int histogram[], int cumulativeHistogram[]);
    void histogramDisplay(int histogram[], const char* name) ;
    void histogramequalization(Mat &image);
};


#endif // ILLUMINATIONCORRECTIONS_H

