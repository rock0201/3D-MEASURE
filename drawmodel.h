#ifndef DRAWMODEL
#define DRAWMODEL


#include "pangolin/pangolin.h"
#include <Eigen/Core>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class DrawModel
{

    Point3d head;
    Point3d chest;
    Point3d leftJoint;
    Point3d rightJoint;
    Point3d leftHand;
    Point3d rightHand;
    Point3d belly;
    Point3d leftKnee;
    Point3d rightKnee;
    Point3d leftFoot;
    Point3d rightFoot;

public:
    DrawModel(vector<Point3d> &modelPoints){
        if(modelPoints.size() != 11 )
            return;
        head       = modelPoints[0];
        chest      = modelPoints[1];
        leftJoint  = modelPoints[2];
        leftHand   = modelPoints[3];
        rightJoint = modelPoints[4];
        rightHand  = modelPoints[5];
        belly      = modelPoints[6];
        leftKnee   = modelPoints[7];
        leftFoot   = modelPoints[8];
        rightKnee  = modelPoints[9];
        rightFoot  = modelPoints[10];
    }

    ~DrawModel();
    void draw(){
        pangolin::CreateWindowAndBind("Model Viewer", 1024, 768);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        pangolin::OpenGlRenderState s_cam(
          pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
          pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
        );

        pangolin::View &d_cam = pangolin::CreateDisplay()
          .SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f)
          .SetHandler(new pangolin::Handler3D(s_cam));

        while (pangolin::ShouldQuit() == false) {
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          d_cam.Activate(s_cam);
          glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          glLineWidth(2);
          // 画出连线

          glColor3f(0.0f, 0.0f, 0.0f);
          glPointSize(10.0f);
          glBegin(GL_POINTS);
          glVertex3d(head.x, head.y, head.z);
          glVertex3d(chest.x, chest.y, chest.z);
          glVertex3d(leftJoint.x, leftJoint.y, leftJoint.z);
          glVertex3d(leftHand.x, leftHand.y, leftHand.z);
          glVertex3d(rightJoint.x, rightJoint.y, rightJoint.z);
          glVertex3d(rightHand.x, rightHand.y, rightHand.z);
          glVertex3d(belly.x, belly.y, belly.z);
          glVertex3d(leftKnee.x, leftKnee.y, leftKnee.z);
          glVertex3d(leftFoot.x, leftFoot.y, leftFoot.z);
          glVertex3d(rightKnee.x, rightKnee.y, rightKnee.z);
          glVertex3d(rightFoot.x, rightFoot.y, rightFoot.z);



          glEnd();



          glColor3f(0.0, 0.0, 0.0);
          glBegin(GL_LINES);
          glVertex3d(head.x, head.y, head.z);
          glVertex3d(chest.x, chest.y, chest.z);

          glVertex3d(chest.x, chest.y, chest.z);
          glVertex3d(leftJoint.x, leftJoint.y, leftJoint.z);

          glVertex3d(leftJoint.x, leftJoint.y, leftJoint.z);
          glVertex3d(leftHand.x, leftHand.y, leftHand.z);

          glVertex3d(chest.x, chest.y, chest.z);
          glVertex3d(rightJoint.x, rightJoint.y, rightJoint.z);

          glVertex3d(rightJoint.x, rightJoint.y, rightJoint.z);
          glVertex3d(rightHand.x, rightHand.y, rightHand.z);

          glVertex3d(chest.x, chest.y, chest.z);
          glVertex3d(belly.x, belly.y, belly.z);

          glVertex3d(belly.x, belly.y, belly.z);
          glVertex3d(leftKnee.x, leftKnee.y, leftKnee.z);

          glVertex3d(leftKnee.x, leftKnee.y, leftKnee.z);
          glVertex3d(leftFoot.x, leftFoot.y, leftFoot.z);

          glVertex3d(belly.x, belly.y, belly.z);
          glVertex3d(rightKnee.x, rightKnee.y, rightKnee.z);

          glVertex3d(rightKnee.x, rightKnee.y, rightKnee.z);
          glVertex3d(rightFoot.x, rightFoot.y, rightFoot.z);



          glEnd();
          pangolin::FinishFrame();
          //usleep(5000);   // sleep 5 ms
        }
        pangolin::QuitAll();
        pangolin::DestroyWindow("Model Viewer");
        //exit(EXIT_SUCCESS);
    }
};



#endif // DRAWMODEL

