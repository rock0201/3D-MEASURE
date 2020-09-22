#include "mainwindow.h"
#include "./ui_mainwindow.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pointl.x = 0.f;
    pointl.y = 0.f;
    pointr.x = 0.f;
    pointr.y = 0.f;
    NUM = 0;
    num2 = 0;
    displaySize.width = 720;//展示的左右图的大小
    displaySize.height = 405;

    imgModel = imread("../lable/modelImg1.png");//
    QImage qImgModel = Mat2QImage(imgModel);
    ui->modelIMg->setPixmap(QPixmap::fromImage(qImgModel));
    setMode();

    ui->next->setIcon(QIcon("../lable/right.png"));
    ui->last->setIcon(QIcon("../lable/left.png"));
    ui->photo->setIcon(QIcon("../lable/photo.png"));
    ui->recalibrate->setIcon(QIcon("../lable/cail.png"));
    ui->calibrate->setIcon(QIcon("../lable/cailse.png"));
    ui->select->setIcon(QIcon("../lable/img.png"));
    ui->angle_Button->setIcon(QIcon("../lable/angle.png"));
    ui->model_button->setIcon(QIcon("../lable/mod.png"));
    ui->generate_button->setIcon(QIcon("../lable/generate.png"));
    //this->setStyleSheet("background-color:rgba(0,0,139,255)");

    backGround = imread("../lable/timg1.jpeg");
    QImage qBack = Mat2QImage(backGround);
    QPalette pal;
    pal.setBrush(QPalette::Background,QBrush(QPixmap::fromImage(qBack)));
    this->setPalette(pal);
    //this->setStyleSheet();

    label_L = new MyLabel("", 0, this);//显示左图的lable
    //label_L->setGeometry(QRect(150, 400, 320, 180));
    ui->imgLayout->addStretch();//在每个控件之间添加STRETCH 可以保证居中 并且间距相同
    ui->imgLayout->addWidget(label_L);
    ui->imgLayout->addStretch();
    connect(label_L, SIGNAL(getCor(int,float,float)), this, SLOT(getPoint (int ,float, float)));

    label_R = new MyLabel("", 1, this);//显示左图的lable
    //label_R->setGeometry(QRect(620, 400, 320, 180));
    ui->imgLayout->addWidget(label_R);
    ui->imgLayout->addStretch();
    connect(label_R, SIGNAL(getCor(int,float,float)), this, SLOT(getPoint (int ,float, float)));
    drawmodelthread = new DrawmodelThread(this);
    connect(drawmodelthread,SIGNAL(isDone()),this,SLOT(deelThreadover()));
    connect(this,SIGNAL(giveMOdelPoint(vector<Point3d>)),drawmodelthread,SLOT(getPoints(vector<Point3d>)));


    // 模型视口
    modelView = new GLModelView(modelPoints, this);
    modelView->setGeometry(300, 720, 640, 360);




}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setModelInfo(int modelNum){
    switch (modelNum) {
    case 0:
         ui->model_chest->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 1:
         ui->model_leftjoint->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 2:
         ui->model_lefthand->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 3:
         ui->model_rightjoint->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 4:
         ui->model_righthand->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 5:
         ui->model_belly->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 6:
         ui->model_leftknee->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 7:
         ui->model_leftfoot->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 8:
         ui->model_rightknee->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 9:
         ui->model_rightfoot->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    case 10:
         ui->model_head->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}");
        break;
    default:
        clearModeInfo();
        break;
    }
}
void MainWindow::clearModeInfo(){
    ui->model_head->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_chest->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_leftjoint->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_lefthand->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_rightjoint->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_righthand->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_belly->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_leftknee->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_leftfoot->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_rightknee->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_rightfoot->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    ui->model_head->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
}
void MainWindow::setMode(){//设置正确的mode
    if(!ISANGLE&&!ISMODEL){
        ui->angle_mode->setStyleSheet("QLabel{background-color:rgb(255,255,255)}");
        ui->distance_mode->setStyleSheet("QLabel{background-color:rgb(200,101,102)}");
        ui->model_mode->setStyleSheet("QLabel{background-color:rgb(255,255,255)}");
    }else if(ISANGLE){
        ui->angle_mode->setStyleSheet("QLabel{background-color:rgb(200,101,102)}");
        ui->distance_mode->setStyleSheet("QLabel{background-color:rgb(255,255,255)}");
        ui->model_mode->setStyleSheet("QLabel{background-color:rgb(255,255,255)}");
    }else if(ISMODEL){
        ui->angle_mode->setStyleSheet("QLabel{background-color:rgb(255,255,255)}");
        ui->distance_mode->setStyleSheet("QLabel{background-color:rgb(255,255,255)}");
        ui->model_mode->setStyleSheet("QLabel{background-color:rgb(200,101,102)}");
    }
}

void MainWindow::deelThreadover(){
   // drawmodelthread->quit();
    drawmodelthread->terminate();
}

void MainWindow::getPoint(int sign, float x,float y){
    std::cout<<sign << " " << x<<" "<<y<<std::endl;
    if(!ISANGLE&&!ISMODEL){
        measureDistance(sign,x,y);
        //测距
    }else if(ISANGLE&&!ISMODEL){
        //测角度
        measureAngle(sign,x,y);
    }else if(ISMODEL&&!ISANGLE){
        //输出模型
        measureModel(sign,x,y);
    }else {
        //出错 需要将ismodel 和 isangle 重置
        ISMODEL = false;
        ISANGLE = false;
        ISLEFT =false;
        ISRIGHT = false;
        NUM = 0;
        QMessageBox::warning(NULL, "提示", "请重新选择模式", QMessageBox::Yes);
    }

}
void MainWindow::measureModel(int sign, float x,float y){

    if(sign==0){//左图
        pointl.x = x;
        pointl.y = y;
        ISLEFT = true;
    }else{
        pointr.x = x;
        pointr.y = y;
        ISRIGHT = true;
    }
    if(ISLEFT&&ISRIGHT){//左图右图都选择完毕 num记录选择的点数 num=11 人体数据 num=3 测角度 num=2 测距 num = 11 输出模型
        vector<Point3d> points;
        triangulation(pointl, pointr, Pl, Pr, points);
        if(NUM<11){
            clearModeInfo();
            setModelInfo(NUM);
            Point3d modelPoint;
            //if > 500 , then give scale
            if(NUM==0&&points[0].z>500){
                scale = points[0].z/500.0;
            }
            modelPoint.x = points[0].x;
            modelPoint.y = points[0].y;
            modelPoint.z = points[0].z/scale;

            modelPoints.push_back(modelPoint);

//            Point3d modelPoint;

//            switch(NUM+1){
//            case 7:
//                modelPoint.x = 0.0 ;  modelPoint.y = 74.0 ; modelPoint.z = 500.0 ;
//                modelPoints.push_back(modelPoint);break;
//            case 6:
//                modelPoint.x = 28.0 ;  modelPoint.y = 75.0 ; modelPoint.z = 480.0 ;
//                modelPoints.push_back(modelPoint);break;
//            case 5:
//                modelPoint.x = 25.0 ;  modelPoint.y = 60.0 ; modelPoint.z = 490.0 ;
//                modelPoints.push_back(modelPoint);break;
//            case 4:
//                modelPoint.x = -25.0 ;  modelPoint.y = 41.0 ; modelPoint.z = 460.0 ;
//                modelPoints.push_back(modelPoint);break;
//            case 3:
//                modelPoint.x = -35.0 ;  modelPoint.y = 39.0 ; modelPoint.z = 480.0 ;
//                modelPoints.push_back(modelPoint);break;
//            case 2:
//                modelPoint.x = 3.0 ;  modelPoint.y = 34.0 ; modelPoint.z = 500.0 ;
//                modelPoints.push_back(modelPoint);break;
//            case 1:
//                modelPoint.x = 0.0 ;  modelPoint.y = 10.0 ; modelPoint.z = 500.0 ;
//                modelPoints.push_back(modelPoint);break;
//            default:
//                break;
//            }



            NUM++;
            //modelView->paintGL();
            cout<<modelPoint.x<<" "<<modelPoint.y<<" "<<modelPoint.z<<endl; //判断是否超过500 超过了就按比例缩小
            cout<<"这是模型的第"<<NUM<<"个点"<<endl;
            ISLEFT =false;
            ISRIGHT = false;
        }
        if(NUM ==11){
            QMessageBox::warning(NULL, "提示", "模型采集完毕", QMessageBox::Yes);
            //展示模型
            emit(giveMOdelPoint(modelPoints));
            drawmodelthread->start();
            //(new DrawModel(modelPoints))->draw();
//            DrawModel* drawModel = new DrawModel(modelPoints);
//            modelView = thread(&DrawModel::draw,drawModel);
//            modelView.join();
            clearModeInfo();
            modelPoints.clear();
            ISMODEL = false;
            ui->model_button->setStyleSheet("background: rgb(255,255,255)");
            NUM = 0;
        }
    }

}

void MainWindow::measureAngle(int sign, float x, float y){
    if(sign==0){//左图
        pointl.x = x;
        pointl.y = y;
        ISLEFT = true;
    }else{
        pointr.x = x;
        pointr.y = y;
        ISRIGHT = true;
    }
    if(ISLEFT&&ISRIGHT){//左图右图都选择完毕 num记录选择的点数 num=3 测角度 num=2 测距 num = 11 输出模型
        vector<Point3d> points;
        triangulation(pointl, pointr, Pl, Pr, points);
        if(NUM<3){
            Point3d anglePoint;
            anglePoint.x = points[0].x;
            anglePoint.y = points[0].y;
            anglePoint.z = points[0].z;
            anglePoints.push_back(anglePoint);
            NUM++;
            cout<<anglePoint.x<<" "<<anglePoint.y<<" "<<anglePoint.z<<endl;
            cout<<"这是角度的第"<<NUM<<"个点"<<endl;
            ISLEFT =false;
            ISRIGHT = false;
        }
        if(NUM ==3){
            //计算角度
            caculateAngle(anglePoints);
            cout<<"角度测量完毕"<<endl;
            anglePoints.clear();
            ui->angle_Button->setStyleSheet("QPushButton{background-color: rgb(245,245,220);selection-color : rgb(225,255,255);border-radius:25px;}QPushButton:hover{background-color:rgb(0,128,128)}QPushButton:focus{outline:none;}");
            setMode();
            ISANGLE = false;
            NUM = 0;
        }
 }
}
void MainWindow::caculateAngle(vector<Point3d>& anglePoints){
    Point3d s1,s2;
    s1 = anglePoints[0]-anglePoints[1];
    s2 = anglePoints[2]-anglePoints[1];
    double a1 = s1.x*s2.x + s1.y*s2.y +s1.z*s2.z;//a1/(a2*a3)
    double a2 = sqrt(s1.x*s1.x + s1.y*s1.y +s1.z*s1.z);
    double a3 = sqrt(s2.x*s2.x + s2.y*s2.y +s2.z*s2.z);
    cout<<a1<<" "<<a2<<" "<<a3<<endl;
    ui->angle->setText(QString::number(acos(a1/(a2*a3))*180/PI,'f',2)+" 度");
}

void MainWindow::measureDistance(int sign, float x,float y){
    if(sign==0){//左图
        pointl.x = x;
        pointl.y = y;
        ISLEFT = true;
    }else{
        pointr.x = x;
        pointr.y = y;
        ISRIGHT = true;
    }
    if(ISLEFT&&ISRIGHT){//左图右图都选择完毕 num记录选择的点数 num=3 测角度 num=2 测距 num = 11 输出模型
        vector<Point3d> points;
        triangulation(pointl, pointr, Pl, Pr, points);
        if(NUM==0){
            pt1_cam_3d.x = points[0].x;
            pt1_cam_3d.y = points[0].y;
            pt1_cam_3d.z = points[0].z;
            cout << "point projected from 3D(1): " << pt1_cam_3d << endl;
            ui->point1->setText("3Dpoint  (" + QString::number(pt1_cam_3d.x,'f',2) + "," + QString::number(pt1_cam_3d.y,'f',2) + "," + QString::number(pt1_cam_3d.z,'f',2) + ")");
            //ui->point1->setText("3Dpoint  (" + QString::number(-20.12,'f',2) + "," + QString::number(120.85,'f',2) + "," + QString::number(1501.61,'f',2) + ")");
            ui->point1->adjustSize();
            NUM++;
        }else if(NUM==1){
            pt2_cam_3d.x = points[0].x;
            pt2_cam_3d.y = points[0].y;
            pt2_cam_3d.z = points[0].z;
            //cout << "point projected from 3D " << pt1_cam_3d << ", d=" << points[0].z << endl;
            cout << "point projected from 3D(2); " << pt2_cam_3d << endl;
            ui->point2->setText("3Dpoint  (" + QString::number(pt2_cam_3d.x,'f',2) + "," + QString::number(pt2_cam_3d.y,'f',2) + "," + QString::number(pt2_cam_3d.z,'f',2) + ")");
            //ui->point2->setText("3Dpoint  (" + QString::number(-21.54,'f',2) + "," + QString::number(49.31,'f',2) + "," + QString::number(1499.31,'f',2) + ")");
            ui->point2->adjustSize();
            NUM++;
            if(NUM==2)
            {
                float dx = pt1_cam_3d.x - pt2_cam_3d.x;
                float dy = pt1_cam_3d.y - pt2_cam_3d.y;
                float dz = pt1_cam_3d.z - pt2_cam_3d.z;
                float d = sqrt(dx * dx + dy * dy + dz * dz );
                cout << "d = " << d << endl;
                ui->distance->setText(QString::number(d,'f',2) + "cm");
               // ui->distance->setText(QString::number(70.92,'f',2) + "cm");
                NUM = 0;
            }
        }
        ISLEFT =false;
        ISRIGHT = false;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
////    MainWindow::paintEvent(event);//必须有，才能让背景图片显示出来
//    QPainter painter(this);
//    QPen pen;
//    pen.setColor(Qt::red);
//    pen.setWidth(20);
//    painter.setPen(pen);
////    QPoint p(100.0,100.0);
//    painter.drawPoint(start);
////    this->update();
////    painter.drawLine(lineStartPoint,lineEndPoint);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{//单击
//    if(event->button() == Qt::LeftButton)
//        {
//        qDebug() << "event->pos()---------" << event->pos();
//        int x = event->pos().x();
//        int y = event->pos().y();
//        cout << "x==" << x << endl;
//        cout << "y==" << y << endl;
//            if(y >= 30){
//                if(y <= 510 && x >= 660 && y <= 1300)//above
//                {


////                    statusBar()->showMessage("point("+x+ ","+y +")",10);
//                }
//                else if(y >=540 && y <=1020){
//                    if(x >= 10 && x <= 650)//left
//                    {
//                        start = event->pos();
//                        QPainter painter(this);
//                        QPen pen;
//                        pen.setColor(Qt::red);
//                        pen.setWidth(20);
//                        painter.setPen(pen);
//                    //    QPoint p(100.0,100.0);
//                        painter.drawPoint(start);
//                        x -= 10;
//                        y -= 540;
//                        pointl.x = x;
//                        pointl.y = y;
//                        cout << "in left picture :x =" << x << "y =" << y << endl;
//                        num++;
//                        vector<Point3d> points;
//                        if (num == 2)
//                        {
//                                    //cout << "pointl:" << pointl << endl;
//                                    //cout << "pointr:" << pointr << endl;
//                            triangulation(pointl, pointr, Pl, Pr, points);
//                            pointl.x = 0;
//                            pointl.y = 0;
//                            pointr.x = 0;
//                            pointr.y = 0;
//                                    /*Point3d pt1_cam_3d(
//                                        points[0].x,
//                                        points[0].y,
//                                        points[0].z
//                                    );*/
//                            if (num2 == 0) {
//                                pt1_cam_3d.x = points[0].x;
//                                pt1_cam_3d.y = points[0].y;
//                                pt1_cam_3d.z = points[0].z;
//                                cout << "point projected from 3D(1): " << pt1_cam_3d << endl;
//                                ui->point1->setText("3Dpoint(" + QString::number(pt1_cam_3d.x) + "," + QString::number(pt1_cam_3d.y) + "," + QString::number(pt1_cam_3d.z) + ")");
//                                num2++;
//                            }
//                            else if (num2 == 1) {
//                                pt2_cam_3d.x = points[0].x;
//                                pt2_cam_3d.y = points[0].y;
//                                pt2_cam_3d.z = points[0].z;
//                                //cout << "point projected from 3D " << pt1_cam_3d << ", d=" << points[0].z << endl;
//                                cout << "point projected from 3D(2); " << pt2_cam_3d << endl;
//                                ui->point2->setText("3Dpoint(" + QString::number(pt2_cam_3d.x) + "," + QString::number(pt2_cam_3d.y) + "," + QString::number(pt2_cam_3d.z) + ")");
//                                num2++;
//                            }
//                                    //cout << "point projected from 3D " << pt1_cam_3d << ", d=" << points[0].z << endl;
//                            if (num2 == 2)
//                            {
//                                float dx = pt1_cam_3d.x - pt2_cam_3d.x;
//                                float dy = pt1_cam_3d.y - pt2_cam_3d.y;
//                                float dz = pt1_cam_3d.z - pt2_cam_3d.z;
//                                float d = sqrt(dx * dx + dy * dy + dz * dz);
//                                cout << "d = " << d << endl;
//                                ui->distance->setText(QString::number(d) + "cm");
//                                num2 = 0;
//                            }
//                            num = 0;
//                        }

//                    }
//                    else if (x >= 660 && x <= 1300)//middle
//                    {
//                        QPainter painter(this);
//                         QPen pointPen(Qt::red);
//                         pointPen.setWidth(6);
//                         painter.setPen(pointPen);
//                         painter.drawPoint(pos().x(),pos().y());
////                         this->update();
//                        x -= 660;
//                        y -= 540;
//                        pointr.x = x;
//                        pointr.y = y;
//                        cout << "in middle picture :x =" << x << "y =" << y << endl;
//                        num++;
//                        vector<Point3d> points;
//                        if (num == 2)
//                        {
//                            //cout << "pointl:" << pointl << endl;
//                            //cout << "pointr:" << pointr << endl;
//                            triangulation(pointl, pointr, Pl, Pr, points);
//                            pointl.x = 0;
//                            pointl.y = 0;
//                            pointr.x = 0;
//                            pointr.y = 0;
//                            /*Point3d pt1_cam_3d(
//                                points[0].x,
//                                points[0].y,
//                                points[0].z
//                            );*/
//                            if (num2 == 0) {
//                                pt1_cam_3d.x = points[0].x;
//                                pt1_cam_3d.y = points[0].y;
//                                pt1_cam_3d.z = points[0].z;
//                                cout << "point projected from 3D(1); " << pt1_cam_3d << endl;
//                                ui->point1->setText("3Dpoint(" + QString::number(pt1_cam_3d.x) + "," + QString::number(pt1_cam_3d.y) + "," + QString::number(pt1_cam_3d.z) + ")");
//                                num2++;
//                            }
//                            else if (num2 == 1) {
//                                pt2_cam_3d.x = points[0].x;
//                                pt2_cam_3d.y = points[0].y;
//                                pt2_cam_3d.z = points[0].z;
//                                //cout << "point projected from 3D " << pt1_cam_3d << ", d=" << points[0].z << endl;
//                                cout << "point projected from 3D(2); " << pt2_cam_3d << endl;
//                                ui->point2->setText("3Dpoint(" + QString::number(pt2_cam_3d.x) + "," + QString::number(pt2_cam_3d.y) + "," + QString::number(pt2_cam_3d.z) + ")");

//                                num2++;

//                            }

//                            //cout << "pt1_cam_3d:" << pt1_cam_3d << endl;
//                            //cout << "pt2_cam_3d:" << pt2_cam_3d << endl;
//                            if (num2 == 2)
//                            {
//                                float dx = pt1_cam_3d.x - pt2_cam_3d.x; cout << " dx=" << dx << endl;
//                                float dy = pt1_cam_3d.y - pt2_cam_3d.y; cout << " dy=" << dy << endl;
//                                float dz = pt1_cam_3d.z - pt2_cam_3d.z; cout << " dz=" << dz << endl;
//                                float d = sqrt(dx * dx + dy * dy + dz * dz);
//                                cout << "d = " << d << endl;
//                                ui->distance->setText(QString::number(d) + "cm");
//                                num2 = 0;
//                            }
//                            num = 0;
//                        }
//                    }
//                    else if(x >= 1310 && x <= 1950)//right
//                    {

//                    }
//                }
//            }
//            qDebug() << "event->pos()x" << event->pos().x()-50;
//            qDebug() << "event->pos()y" << event->pos().y()-50;
//        }
//        else if(event->button() == Qt::RightButton)
//        {
//            statusBar()->showMessage(tr("右键"));
//        }
//        else if(event->button() == Qt::MidButton)
//        {
//            statusBar()->showMessage(tr("中键"));
//        }
//    // 如果是鼠标左键按下
//    if (event->button() == Qt::LeftButton){
//        //qDebug() << "left click";
//        setMouseState( MouseState::L_C, 0);
//    }
//    // 如果是鼠标右键按下
//    else if (event->button() == Qt::RightButton){
//        //qDebug() << "right click";
//        setMouseState(MouseState::R_C, 0);
//    }
//    else if (event->button() == Qt::MidButton){
//        //qDebug() << "mid click";
//        setMouseState(MouseState::M_C, 0);
//    }

}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
    QString sendMsg = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
    //emit sendMessageSignal(sendMsg);

}
void MainWindow::on_select_clicked()
{
    fileName = QFileDialog::getExistingDirectory ( this, tr("Open Image"), ".",QFileDialog::ShowDirsOnly);
    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    fname = code->fromUnicode(fileName).data();
    newSize.width = imageSize.width;
    newSize.height = imageSize.height;
    cv::stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q,
            cv::CALIB_ZERO_DISPARITY, 0, newSize, &validROIL, &validROIR);
    cv::initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pl, newSize,
        CV_32FC1, mapLx, mapLy);
    cv::initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, newSize,
        CV_32FC1, mapRx, mapRy);
    std::cout << "---------------cameraMatrixL & distCoeffL ----------------- " << std::endl;
    std::cout << "cameraMatrixL" << std::endl << cameraMatrixL << std::endl;
    std::cout << "distCoeffL" << std::endl << distCoeffL << std::endl;

    std::cout << "---------------cameraMatrixR & distCoeffR ----------------- " << std::endl;
    std::cout << "cameraMatrixR" << std::endl << cameraMatrixR << std::endl;
    std::cout << "distCoeffR  " << std::endl << distCoeffR << std::endl;

    std::cout << "---------------R & T ----------------- " << std::endl;
    std::cout << "R " << std::endl << R << std::endl;
    std::cout << "T " << std::endl << T << std::endl;

    std::cout << "---------------Pl & Pr ----------------- " << std::endl;
    std::cout << "Pl " << std::endl << Pl << std::endl;
    std::cout << "Pr " << std::endl << Pr << std::endl;

    std::cout << "---------------Rl & Rr ----------------- " << std::endl;
    std::cout << "Rl " << std::endl << Rl << std::endl;
    std::cout << "Rr " << std::endl << Rr << std::endl;

    std::cout << "---------------  Q ----------------- " << std::endl;
    std::cout << "Q " << std::endl << Q << std::endl;

    imageL = cv::imread( fname + "/left" + (QString::number(imgNum)).toStdString() + ".jpg");
    imageR = cv::imread( fname + "/right" +(QString::number(imgNum)).toStdString() + ".jpg");

    //std::cout<<imageL.cols<<" "<<imageL.rows<<std::endl;

    cv::Mat rectifyImageL, rectifyImageR;

    cv::remap(imageL, rectifyImageL, mapLx, mapLy, cv::INTER_LINEAR);
    cv::remap(imageR, rectifyImageR, mapRx, mapRy, cv::INTER_LINEAR);

    cv::resize(imageL, displayImgL,displaySize);
    cv::resize(imageR, displayImgR,displaySize);
    QImage disImage1=Mat2QImage(displayImgL);
    QImage disImage2=Mat2QImage(displayImgR);


    label_L->disImage = Mat2QImage(imageL);//传入原图进行点击
    label_L->setPixmap(QPixmap::fromImage(disImage1));

    label_R->disImage = Mat2QImage(imageR);

    label_R->setPixmap(QPixmap::fromImage(disImage2));

    if(!ISCAIL){
        QMessageBox::warning(NULL, "提示", "请选择标定文件！", QMessageBox::Yes);
    }
    allClear();





}

void MainWindow::on_calibrate_clicked()
{
    calibrateName = QFileDialog::getOpenFileName(this,tr("Open"),".",tr("Image File(*.yml)"));

    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    caliname = code->fromUnicode(calibrateName).data();
    if (calibrateName.isEmpty())
    {
        return;
    }

    cameraMatrixL = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
    distCoeffL = (cv::Mat_<double>(5, 1) << 0, 0, 0, 0, 0);
    cameraMatrixR = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
    distCoeffR = (cv::Mat_<double>(5, 1) << 0, 0, 0, 0, 0);
    R = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
    T = (cv::Mat_<double>(3, 1) << 0, 0, 0);
    E = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
    F = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
    double a[3][3];
    double b[5][1];
    char c;
    string str;
    //cameraMatrixL = (Mat_<double>(3, 3) << a[0][0], 0, 0, 0, 0, 0, 0, 0, 0);
    FileStorage fs(caliname, FileStorage::READ);
    fs["imageSize"] >> imageSize;
    fs["cameraMatrixL"] >> cameraMatrixL;
    fs["distCoeffL"] >> distCoeffL;
    fs["cameraMatrixR"] >> cameraMatrixR;
    fs["distCoeffR"] >> distCoeffR;
    fs["R"] >> R;
    fs["T"] >> T;
    fs["E"] >> E;
    fs["F"] >> F;
    fs.release();
    if (!readSuccess()) {
        cout << "标定文件错误！" << endl;
    }
    cout << "imageSize" << imageSize << endl;
    cout << "cameraMatrixL" << cameraMatrixL << endl;
    cout << "distCoeffL" << distCoeffL << endl;
    cout << "cameraMatrixR" << cameraMatrixR << endl;
    cout << "distCoeffR" << distCoeffR << endl;
    cout << "R" << R << endl;
    cout << "T" << T << endl;
    cout << "E" << E << endl;
    cout << "F" << F << endl;
    ISCAIL = true;
    allClear();

}
QImage  MainWindow::Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {
        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;

}
bool MainWindow::readSuccess()
{
    Mat m1 = (Mat_<double>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
    Mat m2 = (cv::Mat_<double>(5, 1) << 0, 0, 0, 0, 0);
    Mat m3 = (cv::Mat_<double>(3, 1) << 0, 0, 0);
    if (imageSize.width == 0 && imageSize.height == 0) return false;
    if(countNonZero(cameraMatrixL)==0) return false;
    if (countNonZero(distCoeffL) == 0) return false;
    if (countNonZero(cameraMatrixR) == 0) return false;
    if (countNonZero(distCoeffR) == 0) return false;
    if (countNonZero(R) == 0) return false;
    if (countNonZero(T) == 0) return false;
    if (countNonZero(E) == 0) return false;
    if (countNonZero(F) == 0) return false;
    return true;
}
void MainWindow::triangulation(
    const Point& pointl,
    const Point& pointr,
    const Mat& P1, const Mat& P2,
    vector< Point3d >& points)
{
    vector<Point2f> pts_1, pts_2;
    pts_1.push_back(pixel2cam(pointl, cameraMatrixL));
    pts_2.push_back(pixel2cam(pointr, cameraMatrixR));
    Mat pts_4d;
    //cv::triangulatePoints(P1, P2, pts_1, pts_2, pts_4d);
    Mat T1 = (Mat_<float>(3,4) <<
              1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0);
    Mat T2 = (Mat_<float>(3,4) <<
              R.at<double>(0,0), R.at<double>(0,1), R.at<double>(0,2), T.at<double>(0,0),
              R.at<double>(1,0), R.at<double>(1,1), R.at<double>(1,2), T.at<double>(1,0),
              R.at<double>(2,0), R.at<double>(2,1), R.at<double>(2,2), T.at<double>(2,0));
    cv::triangulatePoints(T1, T2, pts_1, pts_2, pts_4d);
    // 转换成非齐次坐标
    for (int i = 0; i < pts_4d.cols; i++)
    {
        Mat x = pts_4d.col(i);
        x /= x.at<float>(3, 0); // 归一化
        Point3d p(
            x.at<float>(0, 0),
            x.at<float>(1, 0),
            x.at<float>(2, 0)
        );
        points.push_back(p);
    }
}

void MainWindow::on_last_clicked()
{
    if(imgNum == 1)
    {
        QMessageBox::warning(NULL, "提示", "已经是第一张了", QMessageBox::Yes);
    }
    else
    {
        imgNum--;
        imageL = cv::imread( fname + "/left" + (QString::number(imgNum)).toStdString() + ".jpg");
        imageR = cv::imread( fname + "/right" +(QString::number(imgNum)).toStdString() + ".jpg"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   );
        cv::resize(imageL, displayImgL,displaySize);
        cv::resize(imageR, displayImgR,displaySize);
        QImage disImage1=Mat2QImage(displayImgL);
        QImage disImage2=Mat2QImage(displayImgR);
        label_L->disImage = Mat2QImage(imageL);
        label_L->setPixmap(QPixmap::fromImage(disImage1));

        label_R->disImage = Mat2QImage(imageR);
        label_R->setPixmap(QPixmap::fromImage(disImage2));

        clearPoint();
    }
}

void MainWindow::on_next_clicked()
{
    string ffname =fname + "/left" + (QString::number(imgNum+1)).toStdString() + ".jpg";
    Mat imagee = imread(ffname);
    if(!imagee.data)
    {
         QMessageBox::warning(NULL, "提示", "已经是最后一张了", QMessageBox::Yes);
    }
    else
    {
        imgNum++;
        imageL = cv::imread( fname + "/left" + (QString::number(imgNum)).toStdString() + ".jpg");
        imageR = cv::imread( fname + "/right" +(QString::number(imgNum)).toStdString() + ".jpg");

        cv::Mat rectifyImageL, rectifyImageR;

        cv::remap(imageL, rectifyImageL, mapLx, mapLy, cv::INTER_LINEAR);
        cv::remap(imageR, rectifyImageR, mapRx, mapRy, cv::INTER_LINEAR);

        cv::resize(imageL, displayImgL,displaySize);
        cv::resize(imageR, displayImgR,displaySize);
        QImage disImage1=Mat2QImage(displayImgL);
        QImage disImage2=Mat2QImage(displayImgR);

        label_L->disImage = Mat2QImage(imageL);
        label_L->setGeometry(150, 400, disImage1.width(), disImage1.height());
        label_L->setPixmap(QPixmap::fromImage(disImage1));

        label_R->disImage = Mat2QImage(imageR);
        label_R->setGeometry(disImage1.width() + 300, 400, disImage2.width(), disImage2.height());
        label_R->setPixmap(QPixmap::fromImage(disImage2));

        clearPoint();
    }
}

void MainWindow::on_recalibrate_clicked()
{
    recalibrate *v = new recalibrate();
    v->show();
}

void MainWindow::on_photo_clicked()
{
    cout<<"photo"<<endl;
    //---------use opencv-------
//    VideoCapture cap1;
//    VideoCapture cap2;
//    Device dv1 = Device(0, Device::LogLevel::Verbose);
//    Device dv2 = Device(1, Device::LogLevel::Verbose);
//    qDebug() << "Device dv1 = Device(0, Device::LogLevel::Verbose);";
//    Mat frame1, frame2;
//    int i = 1;
//    while(1){
//        frame1 =dv1.getDeviceImage(true);
//        frame2 =dv2.getDeviceImage(true);
//        char c = cv::waitKey(1);
//        imshow("camera_left", frame1);
//        imshow("camera_middle", frame2);
//        pfname = "/home/lhy/qtsopencvphoto";
//        if (c == ' ') //按空格采集图像
//        {
//            waitKey(0);//实时拍摄暂停的程序
//            imwrite(pfname + "/left"+ to_string(i) + ".jpg", frame1);
//            imwrite(pfname + "/right"+ to_string(i) + ".jpg", frame2);
//            //imwrite(filename3, frame3);
//            //imwrite(filename4, frame4);
//            cout << "save the " << i << "th image\n" << endl;
//                    i++;
//         }
//         if (c == 'q' || c == 'Q') // 按q退出
//         {
//            break;
//         }
//    }

//---------------use qt-----------
   // photo *p = new photo();
  //  p->show();
}
void MainWindow::clearPoint()//  reset all mode
{
    NUM = 0;
    num2 = 0;
    ui->point1->setText("point(0,0)");
    ui->point2->setText("point(0,0)");
    ui->distance->setText("0cm");
    pointl.x = 0.f;
    pointl.y = 0.f;
    pointr.x = 0.f;
    pointr.y = 0.f;
    pt1_cam_3d.x = 0.f;
    pt1_cam_3d.y = 0.f;
    pt1_cam_3d.z = 0.f;
    ui->angle->setText("0 度");
    scale = 1.0;
    setMode();
    clearModeInfo();

}
void MainWindow::allClear()
{
    imgNum=1;
    clearPoint();
}
Point2d MainWindow::pixel2cam (const Point& p,const Mat& K )
{
    std::cout<<K.at<double>(0,0)<< " " <<K.at<double>(0,1) <<" "<< K.at<double>(0,2)<< endl;
    std::cout<<K.at<double>(1,0)<< " " <<K.at<double>(1,1) <<" "<< K.at<double>(1,2)<< endl;
    std::cout<<K.at<double>(2,0)<< " " <<K.at<double>(2,1) <<" "<< K.at<double>(2,2)<< endl;
   return Point2d
          (
              ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
              ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
          );
}

void MainWindow::on_angle_Button_clicked()
{
    ISANGLE = !ISANGLE;//变绿表示正在使用
    if(ISANGLE&&ISMODEL){//model 和 angle 不允许同时开启
        ui->angle_Button->setStyleSheet("background: rgb(255,255,255)");
        ui->model_button->setStyleSheet("background: rgb(255,255,255)");
        ISANGLE = false;
        ISMODEL = false;
        QMessageBox::warning(NULL, "提示", "请重新选择模式", QMessageBox::Yes);
    }
   // ISANGLE == true?ui->angle_Button->setStyleSheet("background: rgb(0,255,0)"):ui->angle_Button->setStyleSheet("background: rgb(255,255,255)");
    ISANGLE == true?ui->angle_Button->setStyleSheet("QPushButton{background-color: rgb(0,128,128);border-radius:25px;}QPushButton:focus{outline:none;}"):ui->angle_Button->setStyleSheet("QPushButton{background-color: rgb(245,245,220);selection-color : rgb(225,255,255);border-radius:25px;}QPushButton:hover{background-color:rgb(0,128,128)}QPushButton:focus{outline:none;}");
    setMode();
}

void MainWindow::on_model_button_clicked()
{
    if(!ISMODEL){
    QMessageBox::warning(NULL, "提示", "请按照头-胸-左肘-左手-右肘-右手-肚-左膝-左脚-右膝-右脚的顺序选择", QMessageBox::Yes);
    }
    ISMODEL = !ISMODEL;//变绿表示正在使用
    if(ISANGLE&&ISMODEL){
        ui->angle_Button->setStyleSheet("background: rgb(255,255,255)");
        ui->model_button->setStyleSheet("background: rgb(255,255,255)");
        ISANGLE = false;
        ISMODEL = false;
        QMessageBox::warning(NULL, "提示", "请重新选择模式", QMessageBox::Yes);
    }
//    QPushButton{background-color: rgb(245,245,220);selection-color : rgb(225,255,255);border-radius:25px;}QPushButton:hover{background-color:rgb(0,128,128)}QPushButton:focus{outline:none;}
    ISMODEL == true?ui->model_button->setStyleSheet("QPushButton{background-color: rgb(0,128,128);border-radius:25px;}QPushButton:focus{outline:none;}"):ui->model_button->setStyleSheet("QPushButton{background-color: rgb(245,245,220);selection-color : rgb(225,255,255);border-radius:25px;}QPushButton:hover{background-color:rgb(0,128,128)}QPushButton:focus{outline:none;}");
    ISMODEL == true?ui->model_head->setStyleSheet("QLabel{background-color:rgba(200,101,102,200)}"):ui->model_head->setStyleSheet("QLabel{background-color:rgba(200,101,102,0)}");
    setMode();
}

void MainWindow::on_generate_button_clicked()
{
    QMessageBox::warning(NULL, "提示", "模型采集完毕", QMessageBox::Yes);
    //展示模型
    emit(giveMOdelPoint(modelPoints));
    drawmodelthread->start();
    clearModeInfo();
    modelPoints.clear();
    ISMODEL = false;
    ui->model_button->setStyleSheet("background: rgb(255,255,255)");
    NUM = 0;
}
