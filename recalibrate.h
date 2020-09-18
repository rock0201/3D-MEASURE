#ifndef RECALIBRATE_H
#define RECALIBRATE_H

#include <QDialog>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <vector>
//#include <opencv2/imgcodecs/legacy/constants_c.h>
#include<opencv2/imgproc/types_c.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
using namespace std;
using namespace cv;
namespace Ui {
class recalibrate;
}

class recalibrate : public QDialog
{
    Q_OBJECT

public:
    explicit recalibrate(QWidget *parent = nullptr);
    ~recalibrate();

private slots:
    void on_calibrate_img_clicked();

    void on_calibrate_file_clicked();

    void on_calibrate_begin_clicked();
    void readLeftImage(vector<std::string>& filelistL);
    void readRightImage(vector<std::string>& filelistR);
    void calibration(vector<string>& filelist, Mat& cameraMatrix, Mat& distCoeff, vector<std::vector<cv::Point2f>>& imagePoint);
    void calRealPoint(std::vector<std::vector<cv::Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, float squaresize);
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::recalibrate *ui;
    QString camerafileName;
    string cfname;
    QString incalibrateName;
    string incaliname;
    string incaliname1;
    QFont font;
    QPalette palette;


    int PICS_NUMBER=68;
    Size image_size;
    const int boardWidth = 11;                               //横向的角点数目
    const int boardHeight = 8;                              //纵向的角点数据
    const float squareSize = 3.00;                           //标定板黑白格子的大小 单位cm
    cv::Size imageSize;
    const cv::Size boardSize = cv::Size(boardWidth, boardHeight);   //标定板的总内角点
    cv::Mat Rl, Rr, Pl, Pr, Q;                                  //校正旋转矩阵R，投影矩阵P 重投影矩阵Q
    cv::Mat mapLx, mapLy, mapRx, mapRy;                         //映射表
    cv::Rect validROIL, validROIR;                              //裁剪之后的区域
    int num = 0;
    int num2 = 0;
    int goodFrameCount = 0;      //可检测到的图片对数
    cv::Mat R, T, E, F;                                                  //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵
    std::vector<cv::Mat> rvecs;                                        //旋转向量
    std::vector<cv::Mat> tvecs;                                        //平移向量
    std::vector<std::vector<cv::Point2f>> imagePointL;                    //左边摄像机所有照片角点的坐标集合
    std::vector<std::vector<cv::Point2f>> imagePointR;                    //右边摄像机所有照片角点的坐标集合
    std::vector<std::vector<cv::Point3f>> objRealPoint;                   //各副图像的角点的实际物理坐标集合
    std::vector<std::string> filelistL;
    std::vector<std::string> filelistR;
    //Point3d p1, p2;
    Point3d pt1_cam_3d;
    Point3d pt2_cam_3d;
    vector<DMatch> good_matches;
    vector<KeyPoint> keypoints_1, keypoints_2;
    cv::Mat cameraMatrixL;//左相机内参矩阵
    cv::Mat distCoeffL;//左相机畸变系数
    cv::Mat cameraMatrixR;//右相机内参矩阵
    cv::Mat distCoeffR;//右相机畸变系数
};

#endif // RECALIBRATE_H
