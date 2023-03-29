#include "sps_ui.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "QDebug"
//#include "readIPcamera.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("SPS_Test");

    const String videoStreamAddress_front = "rtsp://admin:admin@192.168.30.87:8554/live";
    const String videoStreamAddress_behind = "https://192.168.30.92:8080";
    string label_front="lb_cam_front";
    //string label_behind="lb_cam_behind";
    readIPCamera(videoStreamAddress_front,label_front);
    //readIPCamera(videoStreamAddress_behind,label_behind);
}

MainWindow::~MainWindow()
{

}


