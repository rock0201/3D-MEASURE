#ifndef GLMODELVIEW
#define GLMODELVIEW


#include <GL/gl.h>
#include <GL/glu.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

class GLModelView : public QOpenGLWidget, protected QOpenGLFunctions

{

    Q_OBJECT

public:

    std::vector<Point3d> &modelPoints;


    explicit GLModelView(std::vector<Point3d> &points, QWidget *parent = 0);

    ~GLModelView();

protected:



    void initializeGL() Q_DECL_OVERRIDE;  //初始化OpenGL窗口部件

    void paintGL() Q_DECL_OVERRIDE;  //绘制整个OpenGL窗口，只要有更新发生，这个函数就会被调用

    void resizeGL(int width,int height) Q_DECL_OVERRIDE; //处理窗口大小变化事件的，参数是新状态下的宽和高

    //voidkeyPressEvent(QKeyEvent *e);  //鼠标处理函数

protected:

    bool fullscreen;  //判断是否全屏的变量

};

#endif // GLMODELVIEW

