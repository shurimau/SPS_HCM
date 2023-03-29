#ifndef HCM_API_H
#define HCM_API_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QFile>

#include "hcm_api_utility.h"
#include "hcm_api_employee.h"

class hcm_api : public QObject
{
    Q_OBJECT
public:
    explicit hcm_api(QObject *parent = nullptr);

signals:

public slots:

    void get_employee_data();
    void post_data_test();
    void post_data(hcm_api_employee& e_data);
    void upload_files_test();
    void upload_files(QString path_img_plate, QString path_img_front, QString path_img_back);

private slots:
    void readyRead();
    void encrypted(QNetworkReply *reply);
    void finished();


private:
    QNetworkAccessManager manager;
    hcm_api_employee e_data;
    QByteArray* mDataBuffer;
    QNetworkReply * mNetReply;
    int mTypeRequest;

    QString Pro_path;

    QString Path_img_1;
    QString Path_img_2;
    QString Path_img_3;
    QString Path_img_4;

    QString Path_get;
    QString Path_post;
    QString Path_upload;
    QString Path_table;
};


#endif // HCM_API_H
