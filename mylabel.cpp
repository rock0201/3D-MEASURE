#include<QLabel>
#include"mylabel.h"
#include"a.out.h"

MyLabel::MyLabel(const QString & text,int sign, QWidget* parent)
    :QLabel(parent)
{
     this->setText(text);
    this->sign = sign;
     connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

void MyLabel::slotClicked()
{
    SelectImg *selectImg = new SelectImg(0, sign, disImage);
    selectImg->setGeometry(0, 0, disImage.width(), disImage.height());

    connect(selectImg, SIGNAL(giveCor(int, float,float)), this, SLOT (getPoint(int, float,float)));
    selectImg->show();
}

void MyLabel::getPoint(int sign, float x , float y)
{
    emit getCor(sign, x, y);
}
void MyLabel::mousePressEvent(QMouseEvent* event)
{

        emit clicked();

}

