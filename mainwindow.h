#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "OpenglWidget.h"
#include <librealsense2/rs.hpp>
#include <qvector.h>
#include <QtCharts/qchart.h>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <qdatetime.h>
#include  <cuda_runtime.h>
#include <opencv2/core/cuda.hpp>
#include <qlist.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qvector.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <cmath>
#include "UDP_Server.h"

#include "WaveFrontSensor/WaveFrontSensor.h"


using QtCharts::QChart;
using QtCharts::QChartView;
using QtCharts::QLineSeries;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateFrame();
    void generateEventImage(const cv::Mat &colorMat, cv::Mat &eventImage) ;
    void generalLGMDModel(cv::Mat& ,cv::Mat&,cv::Mat&);
 public:
    void  getPcellOutput(cv::Mat&,cv::Mat&,cv::Mat&,cv::Mat&,cv::Mat&);
    void  getEcellOutput(cv::Mat&,cv::Mat&,cv::Mat&,cv::Mat&,cv::Mat&);
    void  halfWaveRectification(cv::Mat&,cv::Mat&,float threshold);
    void  getIcellOutput(cv::Mat&,cv::Mat&);
    void  getScellOutput(cv::Mat&,cv::Mat&,cv::Mat&);
    void writeTOCSV();
private:
    OpenGLWidget *videoWidget;
    rs2::pipeline pipe;
    QTimer *frameTimer;
    rs2::config cfg;
    rs2::colorizer                                       color_map;
    OpenGLWidget*                                 eventvideoWidget;
    QList<cv::Mat>                                   cycle_frame_queue;
    cv::Mat_<float>                                   kernel_i_layer;
    cv::Mat_<float>                                   kernel_s_layer;
    cv::Mat_<float>                                   kernel_test;
    float                                                         threshold=0.02;
    OpenGLWidget*                                  sLayerOpenglWidget;
    OpenGLWidget*                                  fusionOpenglWidget;
    OpenGLWidget*                                  iLayerOpenglWidget;
    OpenGLWidget*                                   spareOpenglWidget;
    QVector<float>                                      data_vector;
    QWidget*                                                 manage_widget;
    float                                                           sum_=0;
    UdpServer*                                            udpserver;
    WaveFrontSensor*                             wave_front_sensor;
};

#endif // MAINWINDOW_H
