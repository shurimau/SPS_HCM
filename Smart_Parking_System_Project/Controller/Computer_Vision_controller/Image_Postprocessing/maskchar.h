#ifndef MASKCHAR_H
#define MASKCHAR_H
#include "opencv2/opencv.hpp"
#include "fstream"
#include "../Smart_Parking_System_Project/Controller/AI_controller/Model_API/bvmodel.h"
using namespace std::chrono;
using namespace std;
using namespace cv;
using namespace dnn::dnn4_v20221220;
class Maskchar
{
public:
    Maskchar();
    int getLicenseplate(std::vector<Rect> boxes, std::vector<std::string> classes,string &licenseplate,Mat &image, std::vector<int> classIds,std::vector<float> scores);
    void preprocess(Mat &image);
    struct licenseplateInfor
    {
        int x,y;
        string classes;
        float scores;
    };
private:
    bool onelineFlag = true;
    bool bottomrowfullFlag = false;
    vector<licenseplateInfor> linecensepalteVec;
};

#endif // MASKCHAR_H
