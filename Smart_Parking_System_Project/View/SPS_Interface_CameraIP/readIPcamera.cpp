#include "sps_ui.cpp"
void MainWindow::readIPCamera(String URL,String Label){
    timer = new QTimer(this);
    if(!cap.open(URL)) {
        cout << "Link Invalid" << endl;

    }

    else
    {
        cout << "camera is open" << endl;
        cout<<Label<<endl;
        if(Label=="lb_cam_front"){
            connect(timer, SIGNAL(timeout()), this, SLOT(update_IPcamera_front()));
            timer->start(20);
        }
        if(Label=="lb_cam_behind"){
            connect(timer, SIGNAL(timeout()), this, SLOT(update_IPcamera_behind()));
            timer->start(20);
        }
    }

}

void MainWindow::update_IPcamera_front()
{


    cap.read(frame);

    cvtColor(frame, frame, COLOR_BGR2RGB);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->lb_cam_front->setPixmap(QPixmap::fromImage(qt_image));

    ui->lb_cam_front->resize(ui->lb_cam_front->pixmap()->size());
}
void MainWindow::update_IPcamera_behind()
{


    cap.read(frame);

    cvtColor(frame, frame, COLOR_BGR2RGB);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->lb_cam_behind->setPixmap(QPixmap::fromImage(qt_image));

    ui->lb_cam_behind->resize(ui->lb_cam_behind->pixmap()->size());
}

