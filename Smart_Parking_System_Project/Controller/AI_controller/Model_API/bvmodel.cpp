#include "bvmodel.h"

BVmodel::BVmodel()
{

}

void BVmodel::Loadmodel(DetectionModel &model,std::vector<std::string> &classes, string modelpath, string configpath, string objectpath)
{
    Net net;
    net =readNetFromDarknet(configpath,
                            modelpath);
    std::vector<int> classIds;
    std::ifstream file(objectpath);
    std::string line;
    while (std::getline(file, line)) {
        classes.push_back(line);
    }
    model = DetectionModel(net);
    model.setInputParams(1 / 255.0, Size(480, 640), Scalar(), true);
    net.setPreferableBackend(DNN_BACKEND_CUDA);
    net.setPreferableTarget(DNN_TARGET_CUDA);
}

void BVmodel::Detect(Mat &image, DetectionModel model, std::vector<string> classes,std::vector<Rect> &boxes,std::vector<int> &classIds,std::vector<float> &scores)
{

    model.detect(image, classIds, scores, boxes, 0.6, 0.4);
    for (int j = 0; j < classIds.size(); j++) {
        rectangle(image, boxes[j], Scalar(0, 255, 0), 2);
//        char text[100];
//        snprintf(text, sizeof(text), "%s", classes[classIds[j]].c_str());
//        putText(image, text, Point(boxes[j].x , boxes[j].y ), cv::FONT_HERSHEY_SIMPLEX, 1,
//                Scalar(0, 255, 0), 2);
    }
}
void BVmodel::Detect(Mat &image, DetectionModel model, std::vector<string> classes)
{    std::vector<int> classIds;
     std::vector<float> scores;
      std::vector<Rect> boxes;
       model.detect(image, classIds, scores, boxes, 0.6, 0.4);
        for (int j = 0; j < classIds.size(); j++) {
            rectangle(image, boxes[j], Scalar(0, 255, 0), 2);
            char text[100];
            snprintf(text, sizeof(text), "%s", classes[classIds[j]].c_str());
            putText(image, text, Point(boxes[j].x , boxes[j].y ), cv::FONT_HERSHEY_SIMPLEX, 1,
                    Scalar(0, 255, 0), 2);
        }
}

float BVmodel::BVKmean( std::vector<Rect> boxes, Mat &image, int &flags)
{
    flags =0;
    Point I1 = Point(0,0),Icenter,normalvec,directvec;
    Point I2 = Point(0,image.rows/2);
    Point I3 = Point(image.cols,0);
    Point I4 = Point(image.cols,image.rows/2);
    vector <Point> group1,group2,group3,group4;
    int c;
    int count = 100;
    float min1, min2,min3;
    while(count){
         group1.clear(); group2.clear(); group3.clear(); group4.clear();
        for(int i = 0; i <boxes.size(); i++){
            min1 =minof2(distance(Point(boxes[i].x,boxes[i].y),I1),distance(Point(boxes[i].x,boxes[i].y),I2));
            min2 =minof2(distance(Point(boxes[i].x,boxes[i].y),I3),distance(Point(boxes[i].x,boxes[i].y),I4));
            min3 = minof2(min1,min2);

            if(min3 ==distance(Point(boxes[i].x,boxes[i].y),I1) ){
                group1.push_back(Point(boxes[i].x,boxes[i].y)) ;
            }
            else if(min3 ==distance(Point(boxes[i].x,boxes[i].y),I2) ){
                group2.push_back(Point(boxes[i].x,boxes[i].y)) ;
            }
            else if(min3 ==distance(Point(boxes[i].x,boxes[i].y),I3) ){
                group3.push_back(Point(boxes[i].x,boxes[i].y)) ;
            }
            else{
                group4.push_back(Point(boxes[i].x,boxes[i].y)) ;
            }
        }
        //
        I1 =Point(0.0);
        I2 =Point(0.0);
        I3 =Point(0.0);
        I4 =Point(0.0);
        for(int i= 0 ; i < group1.size();i++){
            if(group1.size() == 0)
                break;
            I1+=group1[i];

        }
        for(int i= 0 ; i < group2.size();i++){
            if(group2.size() == 0)
                break;
            I2+=group2[i];

        }
        for(int i= 0 ; i < group3.size();i++){
            if(group3.size() == 0)
                break;
            I3+=group3[i];
        }
        for(int i= 0 ; i < group4.size();i++){
            if(group4.size() == 0)
                break;
            I4+=group4[i];

        }
        if(group1.size() != 0)
            I1 = I1/int(group1.size());
        if(group2.size() != 0)
            I2 = I2/int(group2.size());
        if(group3.size() != 0)
            I3 = I3/int(group3.size());
        if(group4.size() != 0)
            I4 = I4/int(group4.size());

        count--;
        if(count == 0){
            circle(image,I1,2,Scalar(0,0,255),5);
            circle(image,I2,2,Scalar(255,0,0),5);
            circle(image,I3,2,Scalar(0,255,0),5);
            circle(image,I4,2,Scalar(255,0,255),5);
        }
        imshow("abc",image);
    }
    if(group1.size()|| group3.size()){
        flags++;
    }
    if(group4.size()|| group2.size()){
        flags+=2;
    }
    if(group1.size()&&group2.size()&&group4.size() && group3.size()){
        directvec = ((I3 -I1) + (I4-I2))/2;
    }
    else if(group1.size()&&group3.size()){
         directvec = (I3 -I1) ;
    }
    else if(group2.size()&&group4.size()){
         directvec = (I4 -I2) ;
    }
    else
        return 0;
    float angel = acos(directvec.x*image.rows/(sqrt(directvec.x*directvec.x +directvec.y*directvec.y)*image.rows));
    //cout << angel*180/3.14159 << endl;
    return angel;
}



float BVmodel::distance(Point point1, Point point2)
{
    Point point = point1 - point2;
    return sqrt(point.x*point.x + point.y*point.y);
}

float BVmodel::minof2(float a, float b)
{
    return a<b ? a:b;
}

void BVmodel::findcontours(Mat image, float &angel)
{
    Mat canny_output;
    cvtColor(image,image,COLOR_BGR2GRAY);
        Canny( image, canny_output, 200, 255 );
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( size_t i = 0; i< contours.size(); i++ )
        {
            if(contours[i].size() >100){
            Scalar color = Scalar(0,0,255);
            drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
            }
        }
        imshow( "Contours", drawing );
        //waitKey();
}



