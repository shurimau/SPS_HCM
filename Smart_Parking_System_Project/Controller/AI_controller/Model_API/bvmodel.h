#ifndef BVMODEL_H
#define BVMODEL_H
#include "opencv2/opencv.hpp"
#include "fstream"
#include "algorithm"
using namespace std::chrono;
using namespace std;
using namespace cv;
using namespace dnn::dnn4_v20221220;

class BVmodel
{
public:
    BVmodel();
    void Loadmodel(DetectionModel &model, std::vector<std::string> &classes,string modelpath, string configpath, string objectpath);
    void Detect(Mat &image,DetectionModel model, std::vector<std::string> classes,std::vector<Rect> &boxes,std::vector<int> &classIds,std::vector<float> &scores);
    void Detect(Mat &image, DetectionModel model, std::vector<string> classes);
    float BVKmean(std::vector<Rect> boxes, Mat &image, int &flags);
    float distance(Point point1,Point point2);
    float minof2(float a, float b);
    void findcontours(Mat image,float &angel);
};

#endif // BVMODEL_H
