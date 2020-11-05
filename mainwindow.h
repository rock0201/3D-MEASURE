#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "selectimg.h"
#include "mylabel.h"
#include "drawmodel.h"
#include "glmodelview.h"

#include<drawmodelthread.h>
#include <thread>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTextCodec>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QPaintEvent>
#include <QFontDialog>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui_c.h>
#include "beginwindow.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include <iostream>

#include <cmath>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <vector>
//#include <opencv2/imgcodecs/legacy/constants_c.h>
#include<opencv2/imgproc/types_c.h>
#include <recalibrate.h>
#include <photo.h>

#include <vector>
using namespace std;
using namespace cv;
#define PI 3.1415926
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MyLabel *label_L;
    MyLabel *label_R;
    //DrawmodelThread* drawmodelthread;

    GLModelView *modelView;
    Point2d pixel2cam (const Point& p,const Mat& K );
    void closeEvent(QCloseEvent *);
signals:
    void giveMOdelPoint(vector<Point3d> modelPoints);
    void goBeginWindow();
public slots:
    void getPoint(int sign, float x,float y);
    void getBeginwindow(string cName,string iName);
private slots:
    void select_clicked();
    void calibrate_clicked();
    void deelThreadover();
   // void on_select_clicked();
    //void on_calibrate_clicked();
    QImage  Mat2QImage(cv::Mat cvImg);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
//    bool eventFilter(QObject *watched, QEvent *event);
//    void drawRedCircle();
//    void onMousel(int event, int x, int y, int, void*);

//    void onMouser(int event, int x, int y, int, void*);

//    void onMouse(int event, int x, int y, int, void*);
//    void onMous(int event, int x, int y, int, void*);
    void triangulation(
        const Point& pointl,
        const Point& pointr,
        const Mat& P1, const Mat& P2,
        vector< Point3d >& points);
    bool readSuccess();
//    void OnTimer();



    void on_last_clicked();

    void on_next_clicked();

    void on_recalibrate_clicked();

    void on_photo_clicked();
    void clearPoint();
    void allClear();


    void on_angle_Button_clicked();


    void on_model_button_clicked();
    void measureDistance(int sign, float x,float y);
    void measureAngle(int sign, float x,float y);
    void caculateAngle(vector<Point3d>& anglePoints);
    void measureModel(int sign, float x,float y);
    void setMode();
    void setModelInfo(int modelNum);//gen ju nums tishi
    void clearModeInfo();
    void on_generate_button_clicked();

private:
    double scale = 1.0;
    Mat backGround;
    Mat imgModel;
    QImage qImgModel;
    std::thread modelViewThread;
    bool ISANGLE = false;//是否需要测角度
    bool ISMODEL = false;//是否需要输出模型
    bool ISCAIL = false;//是否选择了标定文件
    bool ISIMG = false;//是否选择图片
    Ui::MainWindow *ui;
    Mat img;
    QGraphicsScene *scene;
    QGraphicsView view;
    QGraphicsPixmapItem item;
    QString fileName;
    QString calibrateName;
    string fname;
    string caliname;
    QFont font;
    QFont font1;
    QPalette palette;
    QPalette palette1;

    int imgNum;
    Mat imageL;
    Mat imageR;
    Mat displayImgL;
    Mat displayImgR;
    Size newSize;
    Size displaySize;
    bool ISLEFT = false;
    bool ISRIGHT = false;
    const string filePath = "/home/lhy/SungemSDK-Cpp/examples/apps/project/use_for_measuring/";
    cv::Size imageSize;
    cv::Mat Rl, Rr, Pl, Pr, Q;                                  //校正旋转矩阵R，投影矩阵P 重投影矩阵Q
    cv::Mat mapLx, mapLy, mapRx, mapRy;                         //映射表
    cv::Rect validROIL, validROIR;                              //裁剪之后的区域
    cv::Mat xyz;

    Point2f pointl;
    Point2f pointr;
//    Point2f pointl(0.f, 0.f);
//    Point2f pointr(0.f, 0.f);
    //Point2f point(0.f, 0.f);
    Point2f point;
    int NUM;
    int num2;
    int goodFrameCount = 0;      //可检测到的图片对数
    cv::Mat R, T, E, F;                                                  //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵
    std::vector<cv::Mat> rvecs;                                        //旋转向量
    std::vector<cv::Mat> tvecs;                                        //平移向量
    std::vector<std::vector<cv::Point2f>> imagePointL;                    //左边摄像机所有照片角点的坐标集合
    std::vector<std::vector<cv::Point2f>> imagePointR;                    //右边摄像机所有照片角点的坐标集合
    std::vector<std::vector<cv::Point3f>> objRealPoint;                   //各副图像的角点的实际物理坐标集合
    std::vector<std::string> filelistL;
    std::vector<std::string> filelistR;
    Point3d pt1_cam_3d;
    Point3d pt2_cam_3d;
    vector<Point3d> anglePoints;//测角度所需要的3d点
    vector<Point3d> modelPoints;//生成模型所需要点3d点
    vector<DMatch> good_matches;
    vector<KeyPoint> keypoints_1, keypoints_2;

    cv::Mat cameraMatrixL;//左相机内参矩阵

    cv::Mat distCoeffL;//左相机畸变系数

    cv::Mat cameraMatrixR;//右相机内参矩阵

    cv::Mat distCoeffR;//右相机畸变系数


    int thresh = 0;
    int max_count = 255;
    Mat img_gray;

    vector<Point2d> pointLeft;
    vector<Point2d> pointRight;
    QPoint start;
    QPoint end;

    QString photofileName;
    string pfname;
//    QLabel *label;
//    QGraphicsView *imgs;
};
#endif // MAINWINDOW_H
