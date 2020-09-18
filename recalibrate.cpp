#include "recalibrate.h"
#include "ui_recalibrate.h"

recalibrate::recalibrate(QWidget *parent) :
    //QDialog(parent),
    ui(new Ui::recalibrate)
{
    ui->setupUi(this);
    font.setFamily("楷体");
    font.setBold(true);
//    palette.setBrush(QPalette::Window, QBrush(Qt::white));
//    font.setStyle(QFont::StyleItalic);
    ui->calibrate_img->setFont(font);
    ui->calibrate_file->setFont(font);
    ui->calibrate_begin->setFont(font);
    ui->lineEdit->setFont(font);
}

recalibrate::~recalibrate()
{
    delete ui;
}



void recalibrate::on_calibrate_begin_clicked()
{
        FileStorage fs(incaliname +"/" + incaliname1 +".yml", FileStorage::WRITE);
        cv::Mat imageL;
        cv::Mat imageR;
        cout << "开始读取图片……" << endl;
        readLeftImage(filelistL);
        readRightImage(filelistR);
        cout << "左相机标定……" << endl;
        calibration(filelistL, cameraMatrixL, distCoeffL, imagePointL);
        cout << "右相机标定……" << endl;
        calibration(filelistR, cameraMatrixR, distCoeffR, imagePointR);
        cout << "左相机内参数：" << endl;
        cout << cameraMatrixL << endl;

        cout << "左相机畸变系数：" << endl;
        cout << distCoeffL << endl;
        cout << "右相机内参数：" << endl;
        cout << cameraMatrixR << endl;

        cout << "右相机畸变系数：" << endl;
        cout << distCoeffR << endl;
        imageSize = cv::Size(image_size.width, image_size.height);
        calRealPoint(objRealPoint, boardWidth, boardHeight, PICS_NUMBER / 2, squareSize);
        std::cout << "calculate success" << std::endl;

        double rms = cv::stereoCalibrate(objRealPoint, imagePointL, imagePointR,
            cameraMatrixL, distCoeffL,
            cameraMatrixR, distCoeffR,
            cv::Size(image_size.width, image_size.height),
            R, T, E, F, cv::CALIB_USE_INTRINSIC_GUESS,
            cv::TermCriteria(cv::TermCriteria::COUNT
                + cv::TermCriteria::EPS, 100, 1e-5));
        std::cout << "Stereo Calibration done with RMS error = " << rms << std::endl;
        fs << "imageSize " << imageSize;
        fs << "cameraMatrixL " << cameraMatrixL;
        fs << "distCoeffL " << distCoeffL;
        fs << "cameraMatrixR " << cameraMatrixR;
        fs << "distCoeffR " << distCoeffR;
        fs << "R " << R;
        fs << "T " << T;
        fs << "E " << E;
        fs << "F " << F;
        fs.release();
        QMessageBox::information(NULL, "", "标定完成!");
}
void recalibrate::readLeftImage(vector<std::string>& filelistL)
{

    //读取左摄像头的文件
    for (int i = 1; i <= PICS_NUMBER / 2; i++) {
        std::stringstream str;
        str << cfname << "/left" << i << ".jpg";
        std::cout << str.str() << std::endl;
        filelistL.push_back(str.str());
    }

}
void recalibrate::readRightImage(vector<std::string>& filelistR)
{
    //读取右摄像头的文件
    for (int i = 1; i <= PICS_NUMBER / 2; i++) {
        std::stringstream str;
        str <<  cfname << "/right" << i << ".jpg";
        std::cout << str.str() << std::endl;
        filelistR.push_back(str.str());
    }
}
void recalibrate::calibration(vector<string>& filelist, Mat& cameraMatrix, Mat& distCoeff, vector<std::vector<cv::Point2f>>& imagePoint)
{
    cout << "开始提取角点……" << endl;
    vector<cv::Point2f> corner_points_buf;//建一个数组缓存检测到的角点，通常采用Point2f形式
    vector<cv::Point2f>::iterator corner_points_buf_ptr;
    int image_num = 0;
    string filename;
    while (image_num < filelist.size())
    {
        filename = filelist[image_num++];
        cout << "image_num = " << image_num << endl;
        cout << filename.c_str() << endl;
        cv::Mat imageInput = cv::imread(filename.c_str());
        if (image_num == 1)
        {
            image_size.width = imageInput.cols;
            image_size.height = imageInput.rows;
            cout << "image_size.width = " << image_size.width << endl;
            cout << "image_size.height = " << image_size.height << endl;
        }
        if (findChessboardCorners(imageInput, boardSize, corner_points_buf) == 0)
        {
            cout << "can not find chessboard corners!\n";   //找不到角点
            exit(1);
        }
        else
        {
            cv::Mat gray;
            cv::cvtColor(imageInput, gray, CV_RGB2GRAY);
            cv::find4QuadCornerSubpix(gray, corner_points_buf, cv::Size(5, 5));
            cv::drawChessboardCorners(gray, boardSize, corner_points_buf, true);
            imagePoint.push_back(corner_points_buf);
//            cv::imshow("camera calibration", gray);
            cv::waitKey(100);
        }
    }

    int total = imagePoint.size();
    cout << "total=" << total << endl;
    cout << endl << "角点提取完成" << endl;

    //摄像机标定
    cout << "开始标定……" << endl;
    cameraMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));//内外参矩阵，H——单应性矩阵
    distCoeff = cv::Mat(1, 5, CV_32FC1, cv::Scalar::all(0));//摄像机的5个畸变系数：k1,k2,p1,p2,k3
    vector<cv::Mat> tvecsMat;//每幅图像的平移向量，t
    vector<cv::Mat> rvecsMat;//每幅图像的旋转向量（罗德里格旋转向量）
    vector<vector<cv::Point3f>> objectPoints;//保存所有图片的角点的三维坐标
                                             //初始化每一张图片中标定板上角点的三维坐标
    int i, j, k;
    for (k = 0; k < image_num; k++)//遍历每一张图片
    {
        vector<cv::Point3f> tempCornerPoints;//每一幅图片对应的角点数组
        //遍历所有的角点
        for (i = 0; i < boardSize.height; i++)
        {
            for (j = 0; j < boardSize.width; j++)
            {
                cv::Point3f singleRealPoint;//一个角点的坐标
                singleRealPoint.x = i * 10;
                singleRealPoint.y = j * 10;
                singleRealPoint.z = 0;//假设z=0
                tempCornerPoints.push_back(singleRealPoint);
            }
        }
        objectPoints.push_back(tempCornerPoints);
    }
    cv::calibrateCamera(objectPoints, imagePoint, image_size, cameraMatrix, distCoeff, rvecsMat, tvecsMat, 0);

    cout << "标定完成" << endl;
}
void recalibrate::calRealPoint(std::vector<std::vector<cv::Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, float squaresize)
{

    std::vector<cv::Point3f> imgpoint;
    for (float rowIndex = 0.; rowIndex < boardheight; rowIndex++)
    {
        for (float colIndex = 0.; colIndex < boardwidth; colIndex++)
        {
            imgpoint.push_back(cv::Point3f(rowIndex * squaresize, colIndex * squaresize, 0));
        }
    }

    for (float imgIndex = 0.; imgIndex < imgNumber; imgIndex++)
    {
        obj.push_back(imgpoint);
    }

}
void recalibrate::on_calibrate_img_clicked()
{
    camerafileName = QFileDialog::getExistingDirectory ( this, tr("Open"), ".",QFileDialog::ShowDirsOnly);
    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    cfname = code->fromUnicode(camerafileName).data();
    cv::String pattern = cfname.c_str();//数据集图片路径
    vector<cv::String> fn;
    glob(pattern, fn, false);
    size_t count = fn.size();
    PICS_NUMBER = count;

}

void recalibrate::on_calibrate_file_clicked()
{
//    incalibrateName = QFileDialog::getOpenFileName(this,tr("Open TXT"),".",tr("Image File(*.txt)"));
    incalibrateName = QFileDialog::getExistingDirectory ( this, tr("Open"), ".",QFileDialog::ShowDirsOnly);
    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    incaliname = code->fromUnicode(incalibrateName).data();
}
void recalibrate::on_lineEdit_textChanged(const QString &arg1)
{
    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    incaliname1 = code->fromUnicode(arg1).data();
}
