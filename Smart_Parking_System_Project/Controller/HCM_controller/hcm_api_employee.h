#ifndef HCM_API_EMPLOYEE_H
#define HCM_API_EMPLOYEE_H

#include <QObject>
#include <QDebug>

class hcm_api_employee : public QObject
{
    Q_OBJECT
public:
    //hcm_api_employee();
    explicit hcm_api_employee(QObject *parent = nullptr);
    ~hcm_api_employee();

signals:

public slots:
    void employee_data(QString card_id,
                     int cico_type,
                     QString device_id,
                     int in_out,
                     QString license_plate,
                     QString office_id,
                     QString time,
                     int type);

    void update_img_name(QString img_back,
                         QString img_front,
                         QString img_plate);

    QString get_string_data(int data);
    int get_int_data(int data);

private:
    QString card_id;
    int cico_type;
    QString device_id;
    QString img_back;
    QString img_front;
    QString img_plate;
    int in_out;
    QString license_plate;
    QString office_id;
    QString time;
    int type;

};

#endif // HCM_API_EMPLOYEE_H
