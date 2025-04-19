#include "mainwindow.h"
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>
#include <qdebug.h>


QVector<float> readData(const QString& filename) {
    QVector<float> data;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open the file!";
        return data;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        float value = in.readLine().toFloat();
        if(std::isnan(value)){
            data.append(0);
        }
        else{
             data.append(value);
        }
    }

    return data;
}



QVector<float> normalizeData(QVector<float>& data) {
    QVector<float> normalizedData;
    float minVal = *std::min_element(data.begin(), data.end());
    float maxVal = *std::max_element(data.begin(), data.end());

    for (float value : data) {
        float normalizedValue = 2 * (value - minVal) / (maxVal - minVal) - 1;
        normalizedData.append(normalizedValue);
    }

    return normalizedData;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), videoWidget(new OpenGLWidget()) ,eventvideoWidget(new OpenGLWidget()),sLayerOpenglWidget(new OpenGLWidget()),
      fusionOpenglWidget(new OpenGLWidget()),iLayerOpenglWidget(new OpenGLWidget()),manage_widget(new QWidget()),spareOpenglWidget(new OpenGLWidget())
{

    setWindowTitle("Main Window");
    resize(300, 200);

    kernel_i_layer= (cv::Mat_<float>(3, 3) <<
                       0.f, -1.f, 0.f,
                       -1.f,  5.f, 0.f,
                       0.f, -1.f, 0.f);

    kernel_s_layer= (cv::Mat_<float>(3, 3) <<
                       0.5f, 0.25f, 0.5f,
                       0.25f,  2.f, 0.25f,
                       0.5f, 0.25f, 0.5f);

    kernel_test = (cv::Mat_<float>(3, 3) <<
                       -1, -1, -1,
                       -1,  8, -1,
                       -1, -1, -1);

    udpserver=new UdpServer();

    for (int var = 0; var < 3; ++var) {

        cycle_frame_queue.append(cv::Mat::zeros(cv::Size(640, 480), CV_8UC3));
    }



//    QString filename = "/home/jetson/output_fusion_post_d2.txt";
//    QVector<float> data = readData(filename);
//     QVector<float> normalizedData = normalizeData(data);

//       QLineSeries *series = new QLineSeries();
//       for (int i = 0; i < normalizedData.size(); ++i) {
//           series->append(i * 10, normalizedData[i]);
//          }

//       QChart *chart = new QChart();
//       chart->addSeries(series);
//       chart->createDefaultAxes();
//       chart->setTitle("Normalized Data Curve Over Time");

//       chart->axes(Qt::Horizontal).first()->setTitleText("Time (ms)");
//       chart->axes(Qt::Vertical).first()->setTitleText("Signal Value");
//       QChartView *chartView = new QChartView(chart);
//       chartView->setRenderHint(QPainter::Antialiasing);
//       chartView->setGeometry(100, 100, 600, 400);
//       chartView->setWindowTitle(tr("碰撞信号数值控制台"));
//       chartView->show();

//    return ;

    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 60);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 60);
    pipe.start(cfg);

//    videoWidget->setWindowTitle(tr("opengl 原始图像窗口"));

//    eventvideoWidget->setWindowTitle(tr("opengl 事件图像窗口"));

//     fusionOpenglWidget->setWindowTitle(tr("p layer opengl 图像窗口"));

//     iLayerOpenglWidget->setWindowTitle(tr("i layer opengl 图像窗口"));

//     sLayerOpenglWidget->setWindowTitle(tr("s layer opengl 图像窗口"));

//     spareOpenglWidget->setWindowTitle(tr("备用窗口"));


    QGridLayout *gridLayout = new QGridLayout(manage_widget);
    gridLayout->addWidget(videoWidget, 0, 0);
    gridLayout->addWidget(eventvideoWidget, 0, 1);
    gridLayout->addWidget(fusionOpenglWidget, 0, 2);
    gridLayout->addWidget(iLayerOpenglWidget, 1, 0);
    gridLayout->addWidget(sLayerOpenglWidget, 1, 1);
    gridLayout->addWidget(spareOpenglWidget, 1, 2);

   manage_widget->setFixedSize(QSize(1920,1080));
   manage_widget->setWindowTitle(QString("碰撞检测实验OpenGL控制平台"));
    videoWidget->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() + 1);
    eventvideoWidget->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() + 1);
    fusionOpenglWidget->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() + 1);
    iLayerOpenglWidget->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() + 1);

  sLayerOpenglWidget->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() + 1);
  spareOpenglWidget->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height() + 1);



//   videoWidget->show();
   eventvideoWidget->show();
   //fusionOpenglWidget->show();
   iLayerOpenglWidget->show();
   sLayerOpenglWidget->show();
   spareOpenglWidget->show();
    manage_widget->show();
   frameTimer=new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &MainWindow::updateFrame);
    frameTimer->start(33);


    wave_front_sensor=new WaveFrontSensor();


     wave_front_sensor->show();




}



MainWindow::~MainWindow() {

    pipe.stop();

//    writeTOCSV();
}




void MainWindow::writeTOCSV(){
    QFile file;
    QTextStream text_stream(&file);
   QString filename ="/home/jetson/output_fusion_post_normal.txt";
    file.setFileName(filename);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error: Could not open the file!";
        return;
    }

    for (float& data : data_vector) {
        text_stream << data << "\n";
    }
    file.close();
}


void MainWindow::generateEventImage(const cv::Mat &colorMat, cv::Mat &eventImage) {
    static cv::Mat previousFrame;

    if (previousFrame.empty()) {
        previousFrame = colorMat.clone();
        eventImage = cv::Mat::zeros(colorMat.size(), CV_8UC3);
    }
    cv::Mat diff;
    cv::absdiff(colorMat, previousFrame, diff);

    cv::Mat thresholded;
    cv::threshold(diff, thresholded, 25, 255, cv::THRESH_BINARY);

    eventImage = thresholded;
    previousFrame = colorMat.clone();
}


void MainWindow::getPcellOutput(cv::Mat &input_mat_first, cv::Mat &input_mat_second, cv::Mat& input_mat_thd,cv::Mat&output_mat_post,cv::Mat &output_mat_current){
    output_mat_post=input_mat_second-input_mat_first;
    output_mat_current=input_mat_thd-input_mat_second;
}



void MainWindow::getEcellOutput(cv::Mat &input_mat_first, cv::Mat & input_mat_second, cv::Mat & input_mat_thd, cv::Mat& output_mat_1,cv::Mat& output_mat_2){

    getPcellOutput(input_mat_first,input_mat_second,input_mat_thd,output_mat_1,output_mat_2);
}


void MainWindow::halfWaveRectification(cv::Mat &input_mat, cv::Mat& output_mat,float threshold){

    cv::threshold(input_mat,output_mat,threshold,0,cv::THRESH_TOZERO);
}



void MainWindow::getIcellOutput(cv::Mat &input_mat, cv::Mat &output_mat){


 cv::filter2D(input_mat,output_mat,-1,kernel_test);
}






void MainWindow::getScellOutput(cv::Mat &input_mat_e_layer, cv::Mat &input_mat_i_layer, cv::Mat & s_layer_output){

    cv::absdiff(input_mat_e_layer,cv::Scalar::all(0),input_mat_e_layer);
    cv::absdiff(input_mat_i_layer,cv::Scalar::all(0),input_mat_i_layer);
    cv::filter2D(input_mat_i_layer,input_mat_i_layer,-1,kernel_s_layer);
    s_layer_output=input_mat_e_layer-input_mat_i_layer;
    halfWaveRectification(s_layer_output,s_layer_output,threshold);

}



void MainWindow::generalLGMDModel(cv::Mat& before_frame,cv::Mat& current_frame,cv::Mat& next_frame){

    cv::Mat  result_need_1;
    cv::Mat  result_need_2;
     getPcellOutput(before_frame,current_frame,next_frame,result_need_1,result_need_2);
//    QImage qimg1(result_need_2.data, result_need_2.cols, result_need_2.rows, result_need_2.step, QImage::Format_RGB888);
//    fusionOpenglWidget->updateFrame(qimg1);

     getIcellOutput(before_frame,result_need_1);
    QImage qimg2(result_need_1.data, result_need_1.cols, result_need_1.rows,  result_need_1.step, QImage::Format_RGB888);
    iLayerOpenglWidget->updateFrame(qimg2);

     getScellOutput(current_frame,result_need_1,result_need_2);
     cv::Mat grayImage;
     cv::cvtColor(result_need_2, grayImage, cv::COLOR_BGR2GRAY);
    cv::medianBlur(grayImage,grayImage,3);
    cv::GaussianBlur(grayImage, grayImage, cv::Size(5, 5), 1.5);
    cv::Canny(grayImage, grayImage, 100, 200);

    float sum=cv::sum(grayImage)[0];
    float sum_t=sum-sum_;

//    float sum_t=cv::sum(grayImage)[0]-cv::sum(result_out)[0];
//    result_out=grayImage.clone();
    data_vector.append(sum_t);
    sum_=sum;
     QImage qimg3(grayImage.data, grayImage.cols, grayImage.rows, grayImage.step, QImage::Format_Grayscale8);
     sLayerOpenglWidget->updateFrame(qimg3);
}



void MainWindow::updateFrame() {
    rs2::frameset frames;
    if (pipe.poll_for_frames(&frames)) {

        rs2::frame depthframe=frames.get_depth_frame().apply_filter(color_map);
        rs2::frame colorFrame = frames.get_color_frame();


        if (!colorFrame || !depthframe) {

            qDebug() << "No color / depth frame received!";
            return;
        }

        cv::Mat colorMat(cv::Size(640, 480), CV_8UC3, (void*)colorFrame.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2RGB);

        cv::Mat depth_image(cv::Size(640, 480), CV_8UC3, (void*)depthframe.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(depth_image, depth_image, cv::COLOR_BGR2RGB);

        cv::Mat eventImage;
        cv::Mat fusion_image;
        generateEventImage(colorMat, eventImage);

        // 前置lgmd模型
       //cv::addWeighted(colorMat, 0.7, eventImage, 0.3, 0.0, fusion_image);


        QImage qimg(colorMat.data, colorMat.cols, colorMat.rows, colorMat.step, QImage::Format_RGB888);
        QImage event_qimg(eventImage.data, eventImage.cols, eventImage.rows, eventImage.step, QImage::Format_RGB888);
        QImage depth_qimg(depth_image.data, depth_image.cols, depth_image.rows,depth_image.step, QImage::Format_RGB888);
      //  QImage fusion_qimg(fusion_image.data, fusion_image.cols, fusion_image.rows, fusion_image.step, QImage::Format_RGB888);

        //  videoWidget->updateFrame(qimg);
        eventvideoWidget->updateFrame(event_qimg);
        spareOpenglWidget->updateFrame(depth_qimg);
        udpserver->sendImage(event_qimg);
        //fusionOpenglWidget->updateFrame(fusion_qimg);
        cycle_frame_queue.removeFirst();

         /*朴素LGMD模型和前置事件图像融合LGMD模型*/
        // cycle_frame_queue.append(colorMat.clone());
        /*后置事件图像融合LGMD模型*/
        cycle_frame_queue.append(eventImage.clone());
         generalLGMDModel(cycle_frame_queue[0],cycle_frame_queue[1],cycle_frame_queue[2]);
    } else {
    }
}
