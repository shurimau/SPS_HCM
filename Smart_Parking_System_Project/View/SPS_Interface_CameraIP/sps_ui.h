#ifndef SPS_UI_H
#define SPS_UI_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>
using namespace cv;
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void update_IPcamera_front();
    void update_IPcamera_behind();
    void readIPCamera(string URL,string Label);

private:
    QTimer *timer;
    VideoCapture cap;

    Mat frame;
    QImage qt_image;
    Ui::MainWindow *ui;
};
#endif // SPS_UI_H
