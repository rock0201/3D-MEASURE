//#include "photo.h"
//#include "ui_photo.h"

//photo::photo(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::photo)
//{
//    ui->setupUi(this);
//    ui->label_camera1->setScaledContents(true);
//    ui->label_camera2->setScaledContents(true);
//    font.setFamily("楷体");
//    font.setBold(true);
////    palette.setBrush(QPalette::Window, QBrush(Qt::white));
////    font.setStyle(QFont::StyleItalic);
//    ui->pushButton_open->setFont(font);
//    ui->pushButton_close->setFont(font);
//    ui->pushButton_select->setFont(font);
//    ui->pushButton_collect->setFont(font);
//    i = 1;
//}

//photo::~photo()
//{
//    dv1.~Device();
//    delete ui;
//}
//void photo::on_pushButton_open_clicked()
//{
////    timer   = new QTimer(this);
//////    image1    = new QImage();
////    connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
////    connect(ui->pushButton_open, SIGNAL(clicked()), this, SLOT(openCamara()));


////_____________putong___________

////    if (cap1.isOpened())
////        cap1.release();
////    if (cap2.isOpened())
////        cap2.release();
////        cap1.open(0);
////        cap2.open(2);
////        rate= cap1.get(CV_CAP_PROP_FPS);
////        if (cap1.isOpened()&&cap2.isOpened())
////        {
////            cap1 >> frame1;
////            cap2 >> frame2;
////        }


////        if (!frame1.empty()&&!frame2.empty())
////        {

////            image1 = Mat2QImage(frame1);
////            image2 = Mat2QImage(frame2);
////            ui->label_camera1->setPixmap(QPixmap::fromImage(image1));
////            ui->label_camera2->setPixmap(QPixmap::fromImage(image2));
////            ui->label_camera1->setFixedSize(320, 240);
////            ui->label_camera2->setFixedSize(320, 240);
////            //ui->label_camera1->adjustSize();
////            timer = new QTimer(this);
////            timer->setInterval(1000/rate);   //set timer match with FPS
////            connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
////            timer->start();
////            this->update();
////        }

//        //_______________jiaofeng________________
//            cameraOpen = 1;
////            bool isTheDevHS = true;
////            qDebug() << "cap1----" << cap1.isOpened() ;
////            if (cap1.isOpened())
////                cap1.release();
////            if (cap2.isOpened())
////                cap2.release();
////            if (!isTheDevHS){
////                cap1.open(0);
////qDebug() << "cap1----" << cap1.isOpened() ;
////                cap2.open(2);
////            }
////            qDebug() << "cap1----" << cap1.isOpened() ;
//            cout << "cameraOpenn____" << cameraOpenn <<endl;
////            if(!cameraOpenn)
//                Device dv1 = Device(0, Device::LogLevel::Verbose);
//                Device dv2 = Device(1, Device::LogLevel::Verbose);
////                dv1 = Device(0, Device::LogLevel::Verbose);
//            qDebug() << "Device dv1 = Device(0, Device::LogLevel::Verbose);";
//        //    if (cap1.isOpened()&&cap2.isOpened())
////            if (cap1.isOpened())
////            {

////               rate= cap1.get(CV_CAP_PROP_FPS);
////               qDebug() << "rate= cap1.get(CV_CAP_PROP_FPS);____" << rate;
//               cout << "cameraOpen:::" << cameraOpen << endl;
//               while(cameraOpen){
//               frame1 =dv1.getDeviceImage(false);
//               frame2 =dv2.getDeviceImage(false);
////               qDebug() << "frame1 =dv1.getDeviceImage(true);";
//               if (!frame1.empty()&&!frame2.empty())
////                if (!frame1.empty()&&!frame2.empty())
//                {
//                   //void MainWindow::nextFrame(Device dv1)
//                   //{
//                   //    frame1 =dv1.getDeviceImage(true);
//                   ////    cap1 >> frame1;
//                   ////    cap2 >> frame2;

//                   //    if (!frame1.empty())
//                   //    {
//                   //        image1 = Mat2QImage(frame1);
//                   ////        image2 = Mat2QImage(frame2);
//                   //        ui->label_camera1->setPixmap(QPixmap::fromImage(image1));
//                   ////        ui->label_camera2->setPixmap(QPixmap::fromImage(image2));
//                   //        this->update();
//                   //    }
//                   //}
//                    image1 = Mat2QImage(frame1);
//                    image2 = Mat2QImage(frame2);
//                    ui->label_camera1->setPixmap(QPixmap::fromImage(image1));
//                    ui->label_camera2->setPixmap(QPixmap::fromImage(image2));
//                    ui->label_camera1->setFixedSize(320, 180);
//                    ui->label_camera2->setFixedSize(320, 180);
//                    //ui->label_camera1->adjustSize();
//                    waitKey(0);
////                    timer = new QTimer(this);
////                    timer->setInterval(30);   //set timer match with FPS
////                    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
////                    qDebug() << "connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));";
////                    timer->start();
////                    qDebug() << " timer->start();";
////                    this->update();
////                    qDebug() << "this->update();";
//                    }
//                }
////            }
//}

//void photo::on_pushButton_collect_clicked()
//{
////    const QPixmap *pixmap = label_camera.pixmap();cout << "000000000" << endl;
////    if(pixmap) { cout << "111111111" << endl;pixmap->save("/home/lhy/a.jpg");}
//    string filename1 = pfname + "/left" + to_string(i) + ".jpg";
//    string filename2 = pfname + "/right" + to_string(i) + ".jpg";

////    rgbSwapped()
//    Mat2Mat(frame1);
//    Mat2Mat(frame2);
////    imshow("1111",frame1);
////    imshow("2222",frame2);
//    imwrite(filename1, frame1);
//    imwrite(filename2, frame2);
//    cout << "save the " << i++ << "th image\n" << endl;
//}
////void MainWindow::nextFrame(Device dv1)
//void photo::nextFrame()
//{
////    frame1 =dv1.getDeviceImage(true);
////    frame2 =dv2.getDeviceImage(true);
//////    cap1 >> frame1;
//////    cap2 >> frame2;
////qDebug() << "nextFrame    frame1 =dv1.getDeviceImage(true);";
////    if (!frame1.empty())
////    {
////        image1 = Mat2QImage(frame1);
////        image2 = Mat2QImage(frame2);
////        ui->label_camera1->setPixmap(QPixmap::fromImage(image1.rgbSwapped()));
////        ui->label_camera2->setPixmap(QPixmap::fromImage(image2.rgbSwapped()));
////        this->update();
////    }
//}
//QImage  photo::Mat2QImage(cv::Mat cvImg)
//{
//    QImage qImg;
//    if(cvImg.channels()==3)                             //3 channels color image
//    {

//        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
//        qImg =QImage((const unsigned char*)(cvImg.data),
//                    cvImg.cols, cvImg.rows,
//                    cvImg.cols*cvImg.channels(),
//                    QImage::Format_RGB888);
//    }
//    else if(cvImg.channels()==1)                    //grayscale image
//    {
//        qImg =QImage((const unsigned char*)(cvImg.data),
//                    cvImg.cols,cvImg.rows,
//                    cvImg.cols*cvImg.channels(),
//                    QImage::Format_Indexed8);
//    }
//    else
//    {
//        qImg =QImage((const unsigned char*)(cvImg.data),
//                    cvImg.cols,cvImg.rows,
//                    cvImg.cols*cvImg.channels(),
//                    QImage::Format_RGB888);
//    }

//    return qImg;

//}
//void  photo::Mat2Mat(cv::Mat cvImg)
//{

//    cv::cvtColor(cvImg,cvImg,CV_RGB2BGR);
////    return qImg;

//}
//void photo::on_pushButton_select_clicked()
//{
//    photofileName = QFileDialog::getExistingDirectory ( this, tr("Open"), ".",QFileDialog::ShowDirsOnly);
//    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
//    pfname = code->fromUnicode(photofileName).data();
//    i = 1;
//}
////Device MainWindow::transmit(Device dv1){
////    return dv1;
////}

//void photo::on_pushButton_close_clicked()
//{
//    cameraOpen = 0;
//    dv1.~Device();
////    cap1.release();
////    dv1=Device();
////    timer->stop();
//}
