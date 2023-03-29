#include "sps_ui.h"

#include <QApplication>
#include <QDebug>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QMainWindow>
#include <QUrl>
#include <QDebug>
#include <iostream>

using namespace std;
using namespace cv;
int main(int argc, char ** argv)
{

//    Mat image;
//    const String videoStreamAddress = "rtsp://admin:admin@192.168.30.112:8554/live";
//    // const string videoStreamAddress = "http";
//     VideoCapture vcap(videoStreamAddress);
//    if(!vcap.isOpened() ) {
//        cout << "Link Invalid" << endl;
//        return -1;
//    }

//    while(true){
//        if(!vcap.read(image)) {
//            cout << "No frame" << endl;
//            waitKey();
//        }
//        cv::imshow("Output Window", image);

//        if(waitKey(1) >= 0) break;
//    }



    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    return 0;
}
