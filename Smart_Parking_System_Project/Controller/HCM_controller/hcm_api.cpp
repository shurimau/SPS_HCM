#include "hcm_api.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QNetworkRequest>
#include <iostream>
#include <string>
#include <QDateTime>

using namespace std;

typedef enum {
    E_NOT_OK,
    E_OK
} ReturnType;

typedef enum {
    NONE_REQUEST,
    GET_REQUEST,
    POST_REQUEST,
    UPLOAD_REQUEST
} mode;

typedef enum {
    CARD_ID,
    CICO_TYPE,
    DEVICE_ID,
    IMG_BACK,
    IMG_FRONT,
    IMG_PLATE,
    IN_OUT,
    LICENSE_PLATE,
    OFFICE_ID,
    TIME,
    TYPE
} data_post;

const QByteArray &key = "Authorization";
const QByteArray &value = "Bearer eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICJmT081aTE1RHBpY3hnckxyZ3FsZ29aSTBYcG9yaHRmZ3JxVi1xdFpuc2JNIn0.eyJleHAiOjE2NzQxODM2MTUsImlhdCI6MTY3NDA5NzIxNSwianRpIjoiZTE2ZmIxMWMtNmEzNy00N2QxLWEzMGYtNjBiNGM1ZWE0ZTg0IiwiaXNzIjoiaHR0cDovLzE5Mi4xNjguMjAwLjQ1OjMyMTEwL3JlYWxtcy9ocm1zLXJlYWxtLXFhIiwiYXVkIjoiaHJtcy1hY2NvdW50Iiwic3ViIjoiMDY2YzNlY2ItZWE3ZS00ZmFlLWE1ODMtMzBlYTQ2MDlmYTg2IiwidHlwIjoiQmVhcmVyIiwiYXpwIjoiaHJtcy1hY2NvdW50Iiwic2Vzc2lvbl9zdGF0ZSI6IjFlZjFlZjZiLTYyYmQtNDBjNS1hY2RmLWFlODhlNTFjMzBmNiIsImFsbG93ZWQtb3JpZ2lucyI6WyJodHRwOi8vMTkyLjE2OC4yMDAuNDU6MzIxMTAiXSwicmVhbG1fYWNjZXNzIjp7InJvbGVzIjpbIm9mZmxpbmVfYWNjZXNzIiwidW1hX2F1dGhvcml6YXRpb24iXX0sInJlc291cmNlX2FjY2VzcyI6eyJhY2NvdW50Ijp7InJvbGVzIjpbIm1hbmFnZS1hY2NvdW50IiwibWFuYWdlLWFjY291bnQtbGlua3MiLCJ2aWV3LXByb2ZpbGUiXX19LCJhdXRob3JpemF0aW9uIjp7InBlcm1pc3Npb25zIjpbeyJyc2lkIjoiMTRhY2IzZWEtYTQ4My00OTg4LWIxNTUtNGQzNDkzZjcyNWFkIiwicnNuYW1lIjoiRGVmYXVsdCBSZXNvdXJjZSJ9XX0sInNjb3BlIjoiZW1haWwgcHJvZmlsZSIsInNpZCI6IjFlZjFlZjZiLTYyYmQtNDBjNS1hY2RmLWFlODhlNTFjMzBmNiIsImVtYWlsX3ZlcmlmaWVkIjpmYWxzZSwidGVuYW50SWQiOiIyYTZiM2M0Ni1hMTVlLTQ0Y2QtYjE4ZS0xYWU5ZmVlM2ZjY2EiLCJuYW1lIjoiVGhhbmggTmdoaWEgVHJhbiIsInByZWZlcnJlZF91c2VybmFtZSI6Im5naGlhLnRyYW4tdGhhbmgiLCJnaXZlbl9uYW1lIjoiVGhhbmggTmdoaWEiLCJ1c2VySWQiOiIwNjZjM2VjYi1lYTdlLTRmYWUtYTU4My0zMGVhNDYwOWZhODYiLCJmYW1pbHlfbmFtZSI6IlRyYW4iLCJvcmdJZCI6IjM0YTZhNTJjLTkyZjctNGE2OC1iZjRlLWQ4YmY5NTU0ZDliYSJ9.TS9uHZ4wE6YbRBQEsvsTFmoI2Hu7UVnYqa8Z2U1_olDiJlPrIigK2NyMoTB9ZBoiY8x56D9fbEYLWSdYSUjviAfrZps-RV0qA56odWgmM_1KKwzuxCzsnCadvkZRBUwq6lEDmuWJNODnqhLNCMn7EPxRg00k-0IrcdcPmwN07ZEJ9tFWGFIuHgFAEjwr9XnCLIRrD143RRgIH5_e4SbTgSPHd7LODZdUyOcU6Im7p296gymyhSbQFPDqFsTwlUP3SXHO2lwngO7K1wTeT-w0EpjMClrrgNy4JbvHDOXjLKRET6d6olA1r9nxepkm008XO9_xAeh6SUZldr09VVouRw";

QString Url_get    = "https://qa-nexthcm-api.banvien.com.vn/gatewayapp/sps/employees?page=0&size=10&sort=&paging_ignored=true&office_id=";
QString Url_post   = "https://qa-nexthcm-api.banvien.com.vn/gatewayapp/sps/parking-logs";
QString Url_upload = "https://qa-nexthcm-api.banvien.com.vn/fileapp/sps/multi-upload";

QString Url_post_test   = "https://postman-echo.com/post";
int status = 1;

hcm_api::hcm_api(QObject *parent) : QObject(parent)
{
    mDataBuffer = new QByteArray,
    mNetReply = nullptr,
    mTypeRequest = NONE_REQUEST;
    //reply = nullptr,
    connect(&manager,&QNetworkAccessManager::encrypted,this,&hcm_api::encrypted);
    connect(&manager,&QNetworkAccessManager::finished,this,&hcm_api::finished);

    Pro_path = __FILE__;
    Pro_path = Pro_path.remove(Pro_path.length() - 11, 11);
    qDebug() << "Project path : " << Pro_path;

    Path_img_1 = Pro_path + "img/01.png";
    Path_img_2 = Pro_path + "img/02.png";
    Path_img_3 = Pro_path + "img/03.png";

    Path_get    = Pro_path + "json/get_test.js";
    Path_post   = Pro_path + "json/post_test.js";
    Path_upload = Pro_path + "json/upload_test.js";
    Path_table  = Pro_path + "json/table.txt";
}

/***********************************************************************************************************************
** Function Name         : get_employee_data()
**
** Description           : Get employee data from HCM through HCM API
**
** Input Parameters      : N/A
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : ReturnType(E_OK / E_NOT_OK)
**
** Preconditions         : "Url_get" must be defined, also must have "key" and "value" in header for Authorization
**
** Global Variables Used : Url_get, key, value
**
** Function(s) invoked   : hcm_api::readyRead, hcm_api::finished
**
***********************************************************************************************************************/

void hcm_api::get_employee_data()
{
    qInfo() << "Getting from server...";
    mTypeRequest = GET_REQUEST;
    mDataBuffer->clear();
    QNetworkRequest req;

    req.setUrl(Url_get);
    req.setRawHeader(key, value);

    mNetReply = manager.get(req);
    connect(mNetReply,&QNetworkReply::readyRead,this,&hcm_api::readyRead);
    connect(mNetReply,&QNetworkReply::finished,this,&hcm_api::finished);
}

/***********************************************************************************************************************
** Function Name         : post_data_test()
**
** Description           : This function was made for functional testing hcm_api::post_data()
**
** Input Parameters      : N/A
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : N/A
**
** Preconditions         : N/A
**
** Global Variables Used : N/A
**
** Function(s) invoked   : hcm_api_employee :: employee_data, hcm_api::post_data
**
***********************************************************************************************************************/

void hcm_api::post_data_test()
{
    qint64 unixTime = QDateTime::currentDateTime().toSecsSinceEpoch();
    QString current_time = QString::number(unixTime);
    qDebug() << "time: " << current_time;

    QString Img_back = e_data.get_string_data(IMG_BACK);
    QString Img_front = e_data.get_string_data(IMG_FRONT);
    QString Img_plate = e_data.get_string_data(IMG_PLATE);
    if(Img_back == " " || Img_front == " " || Img_plate == " ") qInfo() << "No Image !!!";

    e_data.employee_data("444445555",
                         1,
                         "sps-1234567890-mc",
                         status,
                         "60B-049.89",
                         "",
                         current_time,
                         1);

    post_data(e_data);

    status = status == 2 ? 1 : status+1;
}

/***********************************************************************************************************************
** Function Name         : post_data()
**
** Description           : Post employee data from SPS to HCM through HCM API when employee go in/out parking lot
**
** Input Parameters      : hcm_api_employee& e_data
**l
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : E_OK / E_NOT_OK
**
** Preconditions         : "Url_post" must be defined, also must have "key" and "value" in header for Authorization
**
** Global Variables Used : Url_post/Url_post_test, key, value
**
** Function(s) invoked   : hcm_api::readyRead
**
***********************************************************************************************************************/

void hcm_api::post_data(hcm_api_employee& e_data)
{
    QJsonObject obj;
    obj["card_id"]          = e_data.get_string_data(CARD_ID);//"RFID190109";//RFID190109
    obj["cico_type"]        = e_data.get_int_data(CICO_TYPE);
    obj["device_id"]        = e_data.get_string_data(DEVICE_ID);
    obj["img_back"]         = e_data.get_string_data(IMG_BACK);
    obj["img_front"]        = e_data.get_string_data(IMG_FRONT);
    obj["img_plate"]        = e_data.get_string_data(IMG_PLATE);
    obj["in_out"]           = e_data.get_int_data(IN_OUT);
    obj["license_plate"]    = e_data.get_string_data(LICENSE_PLATE);
    obj["office_id"]        = e_data.get_string_data(OFFICE_ID);
    obj["time"]             = e_data.get_string_data(TIME);
    obj["type"]             = e_data.get_int_data(TYPE);
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

//    qInfo() <<data;

    qInfo() << "Posting to server...";

    mTypeRequest = POST_REQUEST;
    mDataBuffer->clear();
    QNetworkRequest request;
    request.setUrl(Url_post);
    request.setRawHeader(key, value);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    mNetReply = manager.post(request, data);

    connect(mNetReply,&QNetworkReply::readyRead,this,&hcm_api::readyRead);
}

void hcm_api::readyRead()
{
    qInfo() << "ReadyRead";
    mDataBuffer->append(mNetReply->readAll());
    qInfo() << *mDataBuffer;
}

void hcm_api::encrypted(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    qInfo() << "encrypted";
}

void hcm_api::finished()
{
    if(mNetReply->error())
    {
        qDebug() << "There was some error : " <<mNetReply->errorString();
    }
    else
    {
//          mDataBuffer->clear();

        mDataBuffer->append(mNetReply->readAll());
        QJsonDocument doc = QJsonDocument::fromJson(*mDataBuffer);
        QJsonObject data = doc.object();

        switch(mTypeRequest)
        {
            case GET_REQUEST:
            {
                qInfo() << "Export to JSON file: " << writeInJson(Path_get,data);

                if(Check_API(Path_get))
                {
                    qInfo() << "Convert to table text: ";
                    deleteLinesInJson(Path_get, 6, 8);
                    addCharactersToLinesInJson(Path_get,"[","]");
                    convertJsonToTable(Path_get,Path_table);
                }
                else qInfo() << "Get data fail !!!";
                break;
            }
            case POST_REQUEST :
            {
                qInfo() << writeInJson(Path_post,data);
                if(Check_API(Path_post))
                {
                    qInfo() << "Post success !!!";
                    e_data.employee_data(" ",
                                         -1,
                                         " ",
                                         -1,
                                         " ",
                                         " ",
                                         " ",
                                         -1);

                    e_data.update_img_name(" "," "," ");

                }
                else qInfo() << "Post data fail !!!";
                break;
            }
            case UPLOAD_REQUEST :
            {
                qInfo() << writeInJson(Path_upload,data);

//                qDebug() << "old_plate: " <<  e_data.get_string_data(IMG_PLATE);
//                qDebug() << "old_front: " <<  e_data.get_string_data(IMG_FRONT);
//                qDebug() << "old_back : " <<  e_data.get_string_data(IMG_BACK);

                deleteLinesInJson(Path_upload, 3, 3);
                addCharactersToLinesInJson(Path_upload,"{","}");

                QString new_image_plate = getStringFromJson(Path_upload,"img_plate");
                QString new_image_front = getStringFromJson(Path_upload,"img_front");
                QString new_image_back = getStringFromJson(Path_upload,"img_back");

                qDebug() << "new_plate: " <<  new_image_plate;
                qDebug() << "new_front: " <<  new_image_front;
                qDebug() << "new_back : " <<  new_image_back;

                e_data.update_img_name(new_image_plate,new_image_front,new_image_back);

                break;
            }
        }
        qInfo() << "finished";
    }
}

void hcm_api :: upload_files_test()
{
    upload_files(Path_img_1,Path_img_2,Path_img_3);
}

/***********************************************************************************************************************
** Function Name         : upload_files()
**
** Description           : Post images from SPS to HCM through HCM API when employee go in/out parking lot.
**
** Input Parameters      : N/A
**l
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : E_OK / E_NOT_OK
**
** Preconditions         : "Url_upload" must be defined, also must have "key" and "value" in header for Authorization
**                         Must have paths of 3 images
**
** Global Variables Used : Url_upload, key, value
**
** Function(s) invoked   : hcm_api::readyRead
**
***********************************************************************************************************************/

void hcm_api::upload_files(QString path_img_plate, QString path_img_front, QString path_img_back)
{
    qInfo() << "Uploading: ";

    mTypeRequest = UPLOAD_REQUEST;
    mDataBuffer->clear();

    QNetworkRequest request;
    request.setUrl(Url_upload);
    request.setRawHeader(key, value);

    QHttpMultiPart * multipart = new QHttpMultiPart();

//==============================================================================================================

    QHttpPart subPath;
    subPath.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    subPath.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"subPath\""));
    subPath.setBody("true");
    multipart->append(subPath);

//==============================================================================================================

    QFile *file_1 = new QFile(path_img_plate);
    if(!file_1->open(QIODevice::ReadOnly))    qDebug()<<"file not found";
    else qDebug()<<"file_1:"<<file_1->size();

    QHttpPart img_plate;
    img_plate.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    img_plate.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"img_plate\"; filename=\"01.png\""));
    img_plate.setBodyDevice(file_1);
    file_1->setParent(multipart);
    multipart->append(img_plate);

//==============================================================================================================

    QFile *file_2 = new QFile(path_img_front);
    //file_2->setParent(data);
    if(!file_2->open(QIODevice::ReadOnly))    qDebug()<<"file not found";
    qDebug()<<"file_2:"<<file_2->size();

    QHttpPart img_front;
    img_front.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    img_front.setHeader(QNetworkRequest::ContentDispositionHeader,  QVariant("form-data; name=\"img_front\"; filename=\"02.png\""));
    img_front.setBodyDevice(file_2);
    file_2->setParent(multipart);
    multipart->append(img_front);

//==============================================================================================================

    QFile *file_3 = new QFile(path_img_back);
    //file_3->setParent(data);
    if(!file_3->open(QIODevice::ReadOnly))    qDebug()<<"file not found";
    qDebug()<<"file_3:"<<file_3->size();

    QHttpPart img_back;
    img_back.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    img_back.setHeader(QNetworkRequest::ContentDispositionHeader,  QVariant("form-data; name=\"img_back\"; filename=\"03.png\""));
    img_back.setBodyDevice(file_3);
    file_3->setParent(multipart);
    multipart->append(img_back);

//==============================================================================================================

    mNetReply = manager.post(request, multipart);
    connect(mNetReply,&QNetworkReply::readyRead,this,&hcm_api::readyRead);

}






























