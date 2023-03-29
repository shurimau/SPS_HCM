#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "opencv2/opencv.hpp"
#include "Model_API/bvmodel.h"
#include "Image_Postprocessing/maskchar.h"
#include "Image_Preprocessing_API/filter.h"
#include "Image_Preprocessing_API/illuminationcorrections.h"
#include "Image_Preprocessing_API/lp_perspective_transform.h"
#include <iostream>
#include <cstring>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    //======================
    Net net;
    BVmodel bvmodel;
    Maskchar mask;
    string path ;
    Filter filter;
    Illuminationcorrections ill;
    Mat imageRaw,imageAPI;
    DetectionModel model;
    std::vector<string> classes;
    std::vector<Rect> boxes;
    std::vector<int> classIds;
    vector<Mat> channel(3);
    std::vector<float> scores;

    bvmodel.Loadmodel(model,classes,"../Smart_Parking_System_Project/Model/AI_model/character_recognition/weights_character_yolov3_17_02_2023/yolov3-tiny_best.weights"
                      ,"../Smart_Parking_System_Project/Model/AI_model/character_recognition/weights_character_yolov3_17_02_2023/yolov3-tiny.cfg",
                      "../Smart_Parking_System_Project/Model/AI_model/character_recognition/weights_character_yolov3_17_02_2023/obj.names");
    int count = 0;
    int arr[100] = {0};
    for(int i= 1;i<974;i++){
        path = "../Smart_Parking_System_Project/Model/Data_test/megaparking_lp_rotated_1000/" +to_string(i)+ ".jpg";
        imageRaw = imread(path);

        PerspectiveTransform rotate;
        // imageRaw = rotate.Transform(imageRaw).clone();
        //if(!(imageRaw.rows > 100 && imageRaw.cols > 140) )
            resize(imageRaw,imageRaw,Size(220,170));
        cout << imageRaw.size << endl;
        string  licenseplate,licenseplate1;
        imageAPI = imageRaw.clone();
        filter.medianFilter(imageAPI,3,Filter::MedianFilter);
        ill.RGB2HSVmatrix(imageAPI);
        split(imageAPI, channel);
        ill.histogramequalization(channel[2]) ;
        merge(channel, imageAPI);
        ill.HSV2RGBmatrix(imageAPI);
        bvmodel.Detect(imageAPI,model,classes,boxes,classIds,scores);
        //        bvmodel.Detect(imageAPI,model,classes,boxes,classIds);
        if(!boxes.empty()){
            mask.getLicenseplate(boxes,classes,licenseplate,imageAPI,classIds,scores);
            // mask.getLicenseplate(boxes,classes,licenseplate,imageAPI,classIds);
        }
        cout << licenseplate<<endl;

        //        char char_array[licenseplate.size() + 1];
        //        strcpy(char_array, licenseplate.c_str());
        //        if(strchr(char_array,'$') == NULL && !licenseplate.empty()){
        //            count ++;
        Mat img = imageRaw(boxes[0]);
        if(!boxes.empty())
            //   imwrite("../Smart_Parking_System_Project/Model/Data_test/LicensePlate_result/" + to_string(i) +".jpg",img);
            //        }
            //        else{
            //           imwrite("../Smart_Parking_System_Project/Model/Data_test/others/"+licenseplate+"\n"+ to_string(i) +".jpg",imageAPI);
            //        }

            //cout << count << endl << i<<endl;
            // imshow("imageAPI",imageAPI);
            imshow("image",imageAPI);
        if(licenseplate.find("$")== std::string::npos){
            imwrite("/home/banvien/smart_parking_system/00.Development/02.Code/Smart_Parking_System_Project/Model/Data_test/Test_maskchar/YOLOv3/Full/"+licenseplate+ "_" +to_string(i) + ".jpg",imageAPI);
        }
        else
            imwrite("/home/banvien/smart_parking_system/00.Development/02.Code/Smart_Parking_System_Project/Model/Data_test/Test_maskchar/YOLOv3/NotFull/"+licenseplate+  "_" +to_string(i) +".jpg",imageAPI);
        //            while(1){
        //                if ( 'q' == (char) waitKey(0) ){
        //                    imwrite("/home/banvien/smart_parking_system/00.Development/02.Code/Smart_Parking_System_Project/Model/Data_test/Test_maskchar/YOLOv3/Correct/"+to_string(i)+".jpg",imageAPI);
        //                    break;

        //                }
        //                else  if ( 'w' == (char) waitKey(0) ) {
        //                    imwrite("/home/banvien/smart_parking_system/00.Development/02.Code/Smart_Parking_System_Project/Model/Data_test/Test_maskchar/YOLOv3/Incorrect/"+to_string(i)+".jpg",imageAPI);
        //                    break;
        //                }
        //                //        imshow("imageCrop",img);

        //            }

        //        imshow("imageCrop",img);
        //waitKey();

    }


    //===========================
    return app.exec();
}
