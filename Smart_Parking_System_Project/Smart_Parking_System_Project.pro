QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Controller/AI_controller/Model_API/bvmodel.cpp \
        Controller/Computer_Vision_controller/Image_Postprocessing/maskchar.cpp \
        Controller/Computer_Vision_controller/Image_Preprocessing_API/filter.cpp \
        Controller/Computer_Vision_controller/Image_Preprocessing_API/illuminationcorrections.cpp \
        Controller/Computer_Vision_controller/Image_Preprocessing_API/lp_perspective_transform.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
unix {

 CONFIG += link_pkgconfig

PKGCONFIG += opencv4

 }
#INCLUDEPATH += /usr/local/include/opencv4

#LIBS += -L/usr/local/lib\
#     -lopencv_calib3d\
#     -lopencv_core\
#     -lopencv_features2d \
#     -lopencv_flann\
#     -lopencv_highgui\
#     -lopencv_imgproc\
#     -lopencv_ml\
#     -lopencv_objdetect\
#     -lopencv_photo\
#     -lopencv_stitching\
#     -lopencv_superres\
#     -lopencv_video\
#     -lopencv_videostab\
#     -lopencv_imgcodecs\
#     -lopencv_dnn\
#     -lopencv_videoio
HEADERS += \
    Controller/AI_controller/Model_API/bvmodel.h \
    Controller/Computer_Vision_controller/Image_Postprocessing/maskchar.h \
    Controller/Computer_Vision_controller/Image_Preprocessing_API/filter.h \
    Controller/Computer_Vision_controller/Image_Preprocessing_API/illuminationcorrections.h \
    Controller/Computer_Vision_controller/Image_Preprocessing_API/lp_perspective_transform.h
INCLUDEPATH += \
    ../Smart_Parking_System_Project/Controller/AI_controller/ \
    ../Smart_Parking_System_Project/Controller/Computer_Vision_controller/


