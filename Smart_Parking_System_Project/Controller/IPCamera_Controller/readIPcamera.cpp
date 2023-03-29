#include "mainwindow.cpp"
void MainWindow::readIPCamera(String URL){
    timer = new QTimer(this);



    if(!cap.open(URL)) {
        cout << "Link Invalid" << endl;

    }
    else
    {
        cout << "camera is open" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_IPcamera()));
        timer->start(20);
    }
}

void MainWindow::update_IPcamera()
{

    cap.read(frame);

    cvtColor(frame, frame, COLOR_BGR2RGB);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->Frame->setPixmap(QPixmap::fromImage(qt_image));

    ui->Frame->resize(ui->Frame->pixmap()->size());
}
