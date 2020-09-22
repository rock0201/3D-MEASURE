#include "selectimg.h"
#include "ui_selectimg.h"

SelectImg::SelectImg(QWidget *parent
                     ) :
    QDialog(parent),
    ui(new Ui::SelectImg)
{
    ui->setupUi(this);
}
SelectImg::SelectImg(QWidget *parent,
                     int sign,
                     QImage img
                     ) :
    QDialog(parent),
    ui(new Ui::SelectImg)
{
    ui->setupUi(this);
    this->sign = sign;
    ui->image->setPixmap(QPixmap::fromImage(img));
    ui->image->setGeometry(0, 0, img.width(), img.height());
    this->setGeometry(0, 0, img.width(), img.height());
}
SelectImg::~SelectImg()
{
    delete ui;
}
void SelectImg::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        {
        float x = event->pos().x();
        float y = event->pos().y();
        std::cout << "x==" << x << std::endl;
        std::cout << "y==" << y << std::endl;
        emit giveCor(sign, x,y);
        this->close();
    }

}
