#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "beginwindow.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    beginWindow begin;
    begin.move(800,400);
    //MainWindow w;
    begin.show();
    return a.exec();



}
