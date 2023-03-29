#include "sps_ui.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "QDebug"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("SPS");
}

MainWindow::~MainWindow()
{

}


