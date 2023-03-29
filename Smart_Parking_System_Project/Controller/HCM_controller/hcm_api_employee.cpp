#include "hcm_api_employee.h"

hcm_api_employee::hcm_api_employee(QObject *parent) : QObject(parent)
{
    this->card_id           = " ";
    this->cico_type         = -1;
    this->device_id         = " ";
    this->img_back          = " ";
    this->img_front         = " ";
    this->img_plate         = " ";
    this->in_out            = -1;
    this->license_plate     = " ";
    this->office_id         = " ";
    this->time              = " ";
    this->type              = -1;
}

hcm_api_employee::~hcm_api_employee()
{

}

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

void hcm_api_employee :: employee_data(QString card_id,
                                       int cico_type,
                                       QString device_id,
                                       int in_out,
                                       QString license_plate,
                                       QString office_id,
                                       QString time,
                                       int type)
{
    this->card_id           = card_id;
    this->cico_type         = cico_type;
    this->device_id         = device_id;
//    this->img_back          = img_back;
//    this->img_front         = img_front;
//    this->img_plate         = img_plate;
    this->in_out            = in_out;
    this->license_plate     = license_plate;
    this->office_id         = office_id;
    this->time              = time;
    this->type              = type;
}

void hcm_api_employee :: update_img_name(QString img_back,
                                         QString img_front,
                                         QString img_plate)
{
    this->img_back          = img_back;
    this->img_front         = img_front;
    this->img_plate         = img_plate;
}

QString hcm_api_employee :: get_string_data(int data)
{
    switch(data)
    {
        case CARD_ID:           return this->card_id;
        case DEVICE_ID:         return this->device_id;
        case IMG_BACK:          return this->img_back;
        case IMG_FRONT:         return this->img_front;
        case IMG_PLATE:         return this->img_plate;
        case LICENSE_PLATE:     return this->license_plate;
        case OFFICE_ID:         return this->office_id;
        case TIME:              return this->time;
        default: return "";
    }

}

int hcm_api_employee :: get_int_data(int data)
{
    switch(data)
    {
        case CICO_TYPE:         return this->cico_type;
        case IN_OUT:            return this->in_out;
        case TYPE:              return this->type;
        default: return -1;
    }
}
