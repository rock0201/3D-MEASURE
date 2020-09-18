#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>
#include<QString>
#include<QWidget>
#include<QMainWindow>
#include <QMouseEvent>
#include"selectimg.h"
#include<iostream>


class MyLabel:public QLabel
{
    Q_OBJECT
public:
    MyLabel(const QString &text, int sign, QWidget *parent=0);
    ~MyLabel(){}

    QImage disImage;
    int sign;
signals:
    void clicked();
    //void giveCor(float x,float y);
    void getCor(int sign, float x, float y);
public slots:
    void slotClicked();
    void getPoint(int, float,float);
protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // MYLABEL_H

