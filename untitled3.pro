QT       += core gui
QT       +=  charts
QT       +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    OpenglWidget.cpp \
    WaveFrontSensor/ConfigurationWidget.cpp \
    WaveFrontSensor/WaveFrontSensor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    OpenglWidget.h \
    UDP_Server.h \
    WaveFrontSensor/Algorithm.h \
    WaveFrontSensor/Configuration.h \
    WaveFrontSensor/ConfigurationWidget.h \
    WaveFrontSensor/CustomException.h \
    WaveFrontSensor/Legendre.h \
    WaveFrontSensor/ModalMethodCalculateModel.h \
    WaveFrontSensor/OMDA_Algorithm.h \
    WaveFrontSensor/Phase.h \
    WaveFrontSensor/WaveFrontSensor.h \
    WaveFrontSensor/Zernike.h \
    mainwindow.h

FORMS += \
    WaveFrontSensor/ConfigurationWidget.ui \
    WaveFrontSensor/WaveFrontSensor.ui \
    mainwindow.ui


INCLUDEPATH += /usr/include/opencv4
LIBS += -L/usr/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_imgproc

LIBS += -L/path/to/opencv/lib \
        -lopencv_cudaimgproc -lopencv_cudaarithm -lopencv_cudaobjdetect

CUDA_DIR = /usr/local/cuda
INCLUDEPATH += $$CUDA_DIR/include
LIBS += -L$$CUDA_DIR/lib64 -lcudart

## 指定 librealsense2 头文件路径
INCLUDEPATH += /usr/local/include/librealsense2

## 指定 librealsense2 运行库路径
LIBS += -L/usr/local/lib -lrealsense2


INCLUDEPATH += /usr/include/eigen3
INCLUDEPATH += /usr/include/eigen3/Eigen

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    WaveFrontSensor/Resource.qrc \
    WaveFrontSensor/WaveFrontSensor.qrc

DISTFILES += \
    WaveFrontSensor/WaveFrontSensor.ico \
    WaveFrontSensor/WaveFrontSensor.rc \
    WaveFrontSensor/WaveFrontSensor.txt \
    WaveFrontSensor/back/b1.png \
    WaveFrontSensor/back/b3.png \
    WaveFrontSensor/back/icon2.png \
    WaveFrontSensor/back/icon3.png \
    WaveFrontSensor/data_example_21/ex21_res_int_pr_se.dat \
    WaveFrontSensor/data_example_21/ex21_res_int_pr_se.png \
    WaveFrontSensor/data_example_21/xy.bmp \
    WaveFrontSensor/ex21_res_int_pr_se.dat.png \
    WaveFrontSensor/h2.png \
    WaveFrontSensor/h3.png \
    WaveFrontSensor/test.png
