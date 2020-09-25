#ifndef GLMODELVIEW_H
#define GLMODELVIEW_H


#include "camera.h"
#include "character.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QtGui/QMouseEvent>
#include <glm.hpp>
#include <math.h>
#include <QMouseEvent>
#include <vector>

class GLModelView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core

{

    Q_OBJECT

public:
    int vNum;
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    explicit GLModelView(QWidget *parent = 0);
    void updateVertices(std::vector<float> &vert);
    ~GLModelView();

protected:
    void initializeGL() Q_DECL_OVERRIDE;  //初始化OpenGL窗口部件

    void paintGL() Q_DECL_OVERRIDE;  //绘制整个OpenGL窗口，只要有更新发生，这个函数就会被调用

    void resizeGL(int width,int height) Q_DECL_OVERRIDE; //处理窗口大小变化事件的，参数是新状态下的宽和高

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

    bool fullscreen;  //判断是否全屏的变量

private:



    Camera *camera;
    Character *character;

    QOpenGLShaderProgram shader;
    QOpenGLShaderProgram jointShader;
    QOpenGLShaderProgram charShader;

    GLuint pointVAO, pointVBO;
    GLuint lineVAO, lineVBO, lineEBO;


    int jointNum;
    std::vector<GLuint> jointVAOs;
    std::vector<GLuint> jointVBOs;
    std::vector<QVector3D> jointAnglePoses;
    std::vector<int> jointAngles;

    // data about model
    QMatrix4x4 model;

    // record mouse position
    float mouseLastX;
    float mouseLastY;
    // check mouse button
    bool isLeftClicked;
    bool isRightClicked;

    static std::vector<QVector3D> enrichCurve(QVector3D &start, QVector3D &middle, QVector3D &end, int num);
    void initJointData();
};

#endif // GLMODELVIEW_H
