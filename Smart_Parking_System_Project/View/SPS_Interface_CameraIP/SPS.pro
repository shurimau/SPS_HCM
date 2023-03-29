QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    main.cpp \
    readIPcamera.cpp \
    sps_ui.cpp

HEADERS += \
    readIPcamera.h \
    sps_ui.h

FORMS += \
    sps_ui.ui
INCLUDEPATH += /usr/local/include/opencv4/

LIBS += -L/usr/local/lib\
    -lopencv_calib3d\
    -lopencv_core\
    -lopencv_features2d \
    -lopencv_flann\
    -lopencv_highgui\
    -lopencv_imgproc\
    -lopencv_ml\
    -lopencv_objdetect\
    -lopencv_photo\
    -lopencv_stitching\
    -lopencv_superres\
    -lopencv_video\#
    -lopencv_videostab\
    -lopencv_imgcodecs\
    -lopencv_dnn\
    -lopencv_videoio

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc \
    resource.qrc
