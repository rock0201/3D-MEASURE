#include "drawmodelthread.h"

DrawmodelThread::DrawmodelThread(QObject *parent):QThread(parent)
{

}
void DrawmodelThread::getPoints(vector<Point3d> modelPoints){
    this->modelPoints = modelPoints;
}

void DrawmodelThread::run()
{
    (new DrawModel(modelPoints))->draw();
    emit isDone();  //发送完成信号
}
