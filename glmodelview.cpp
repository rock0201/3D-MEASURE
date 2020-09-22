#include "glmodelview.h"


GLModelView::GLModelView(std::vector<Point3d> &points, QWidget *parent)
    : QOpenGLWidget(parent),
      modelPoints(points)
{
    rotVelocity = 0.5;
    modelAngleX = 0;
    modelAngleY = 0;

    mouseLastX = 0;
    mouseLastY = 0;

}

GLModelView::~GLModelView()
{

}

void GLModelView::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);

    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_COLOR_MATERIAL);
}

void GLModelView::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!modelPoints.empty()){
        std::cout<<"translate..."<<std::endl;
        glLoadIdentity();
        glTranslated(-modelPoints[0].x, -modelPoints[0].y, -modelPoints[0].z);
        glTranslated(0, 0, -100);
        //glRotatef(modelAngleX, 1, 0 ,0);
        //glRotatef(modelAngleY, 0, 1, 0);
    }

    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor3f(0, 1, 0);
    glColor3f(1, 0 ,0);
    //glVertex3d(0, , -10);
    for(int i = 0; i < modelPoints.size(); ++i){
        glVertex3d(modelPoints[i].x, modelPoints[i].y, modelPoints[i].z);
    }

    glEnd();

    glBegin(GL_LINES);
    glLineWidth(4.0);
    glColor3f(0, 0 ,1);

    switch(modelPoints.size()){
    case 11:
        glVertex3d(modelPoints[9].x, modelPoints[9].y, modelPoints[9].z);
        glVertex3d(modelPoints[10].x, modelPoints[10].y, modelPoints[10].z);
    case 10:
        glVertex3d(modelPoints[6].x, modelPoints[6].y, modelPoints[6].z);
        glVertex3d(modelPoints[9].x, modelPoints[9].y, modelPoints[9].z);
    case 9:
        glVertex3d(modelPoints[7].x, modelPoints[7].y, modelPoints[7].z);
        glVertex3d(modelPoints[8].x, modelPoints[8].y, modelPoints[8].z);
    case 8:
        glVertex3d(modelPoints[6].x, modelPoints[6].y, modelPoints[6].z);
        glVertex3d(modelPoints[7].x, modelPoints[7].y, modelPoints[7].z);
    case 7:
        glVertex3d(modelPoints[1].x, modelPoints[1].y, modelPoints[1].z);
        glVertex3d(modelPoints[6].x, modelPoints[6].y, modelPoints[6].z);
    case 6:
        glVertex3d(modelPoints[4].x, modelPoints[4].y, modelPoints[4].z);
        glVertex3d(modelPoints[5].x, modelPoints[5].y, modelPoints[5].z);
    case 5:
        glVertex3d(modelPoints[1].x, modelPoints[1].y, modelPoints[1].z);
        glVertex3d(modelPoints[4].x, modelPoints[4].y, modelPoints[4].z);
    case 4:
        glVertex3d(modelPoints[2].x, modelPoints[2].y, modelPoints[2].z);
        glVertex3d(modelPoints[3].x, modelPoints[3].y, modelPoints[3].z);
    case 3:
        glVertex3d(modelPoints[1].x, modelPoints[1].y, modelPoints[1].z);
        glVertex3d(modelPoints[2].x, modelPoints[2].y, modelPoints[2].z);
    case 2:
        glVertex3d(modelPoints[0].x, modelPoints[0].y, modelPoints[0].z);
        glVertex3d(modelPoints[1].x, modelPoints[1].y, modelPoints[1].z);
    case 1:
    default:
        break;
    }
    glEnd();
}

void GLModelView::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    float zNear = 0.1, zFar = 1000;
    GLdouble rFov = 90 * 3.14159265 / 180.0;
    glFrustum( -zNear * tan( rFov / 2 ) * ((GLfloat)width/(GLfloat)height),
               zNear * tan( rFov / 2) * ((GLfloat)width/(GLfloat)height),
               zNear * tan( rFov / 2),
               -zNear * tan( rFov / 2),
               zNear, zFar );

    //glOrtho(-width/2,width/2,-height/2,height/2,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void GLModelView::mousePressEvent(QMouseEvent *event)
{
    mouseLastX = event->pos().x();
    mouseLastY = event->pos().y();
}

void GLModelView::mouseMoveEvent(QMouseEvent *event)
{
    float deltaX = event->pos().x() - mouseLastX;
    float deltaY = event->pos().y() - mouseLastY;

    mouseLastX = event->pos().x();
    mouseLastY = event->pos().y();

    modelAngleY += deltaX * rotVelocity; // deltaX -> rotation in y-axis
    modelAngleX += deltaY * rotVelocity; // deltaY -> rotation in x-axis

    update();
}
