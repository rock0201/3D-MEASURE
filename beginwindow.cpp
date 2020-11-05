#include "beginwindow.h"
#include "ui_beginwindow.h"

beginWindow::beginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::beginWindow)
{
    ui->setupUi(this);
    MainWindow *mainWindow = new MainWindow();
    mainWindow->move(200,200);
    connect(this, SIGNAL(goMainwindow(string ,string )), mainWindow, SLOT(getBeginwindow(string ,string )));
    connect(mainWindow, SIGNAL(goBeginWindow()), this, SLOT(backWindow()));

}

beginWindow::~beginWindow()
{
    delete ui;
}

void beginWindow::on_pushButton_clicked()
{
    if(caliname.empty()|| fname.empty()){
        QMessageBox::warning(NULL, "提示", "请选择标定文件或图片文件", QMessageBox::Yes);
    }else{
        emit goMainwindow(caliname,fname);
        this->setVisible(false);
        //this->destroy();
    }

}
void beginWindow:: backWindow(){
    this->show();
}

void beginWindow::on_cail_button_clicked()
{
    calibrateName = QFileDialog::getOpenFileName(this,tr("Open"),".",tr("Image File(*.yml)"),0,QFileDialog::DontUseSheet);

    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    caliname = code->fromUnicode(calibrateName).data();
    cout<<caliname<<endl;
}

void beginWindow::on_img_button_clicked()
{
    fileName = QFileDialog::getExistingDirectory ( this, tr("Open Image"), ".",QFileDialog::ShowDirsOnly);
    QTextCodec *code = QTextCodec::codecForName("UTF-8");//解决中文路径问题
    fname = code->fromUnicode(fileName).data();
    cout<<fname<<endl;
}
