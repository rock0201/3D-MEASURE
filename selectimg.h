#ifndef SELECTIMG_H
#define SELECTIMG_H

#include <QDialog>
#include <QMouseEvent>
#include<iostream>
namespace Ui {
class SelectImg;
}

class SelectImg : public QDialog
{
    Q_OBJECT

public:
    int sign;

    explicit SelectImg(QWidget *parent = 0);
    SelectImg(QWidget *parent, int, QImage);
    ~SelectImg();
signals:
     void giveCor(int sign, float x,float y);
private:
    Ui::SelectImg *ui;
protected:
    void mousePressEvent(QMouseEvent* event);
};


#endif // SELECTIMG_H
