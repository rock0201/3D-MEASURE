//#ifndef PHOTO_H
//#define PHOTO_H

//#include <QWidget>
//#include <opencv2/opencv.hpp>
////#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//#include <math.h>
//#include <QLabel>
//#include <QPushButton>
//#include <QFileDialog>
//#include <QTextCodec>
//#include <QDebug>


//#include "libhs_highapi/include/device.hpp"
//#include "libhs_highapi/include/hsapi.hpp"

//using namespace cv;
//using namespace std;
//using namespace hs;
//namespace Ui {
//class photo;
//}

//class photo : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit photo(QWidget *parent = nullptr);
//    ~photo();
//private:
//    VideoCapture cap1;
//    VideoCapture cap2;
//    Mat frame1;
//    Mat frame2;
//    Mat frame3;
//    Mat frame4;
//    QImage  image1;
//    QImage  image2;
//    QTimer *timer;
//    double rate; //FPS
//    VideoWriter writer;
//    QPushButton pushButton_open;
//    QLabel label_camera1;
//    QLabel label_camera2;

//    QString photofileName;
//    string pfname;
//    QFont font;
//    QPalette palette;
//    int i;

//    Device dv1;
//    Device dv2;
//    int cameraOpen = 1;
//    bool  cameraOpenn = false;
//private slots:
//    void on_pushButton_open_clicked();
//    void on_pushButton_collect_clicked();
//    QImage  Mat2QImage(cv::Mat cvImg);
//    void Mat2Mat(cv::Mat cvImg);
//    void nextFrame();
////    void nextFrame(Device dv1);
//    /*Device transmit(Device dv1)*/;
//    void on_pushButton_select_clicked();
////    void openCamara();
////void readFarme();
//    void on_pushButton_close_clicked();
//private:
//    Ui::photo *ui;
//};

//#endif // PHOTO_H
