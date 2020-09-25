
#include "glmodelview.h"

QMatrix4x4 glmToQMatrix(glm::mat4 mat);
QVector3D slerp(QVector3D &from, QVector3D &to, float t);
QVector3D getAngleTextPosition(QVector3D &start, QVector3D &middle, QVector3D &end);

GLModelView::GLModelView(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mouseLastX = 0;
    mouseLastY = 0;

    // set model data

    model.setToIdentity();
    vNum = 0;
    jointNum = 0;

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    setFormat(format);

}

GLModelView::~GLModelView()
{

}

void GLModelView::initializeGL()
{

    initializeOpenGLFunctions();

    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // init camera
    camera = new Camera(glm::vec3(0, 0, 10));

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/rock/measure1/model.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/rock/measure1/model.frag");
    shader.link();

    jointShader.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/rock/measure1/joint.vert");
    jointShader.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/rock/measure1/joint.frag");
    jointShader.link();

    charShader.addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/rock/measure1/character.vert");
    charShader.addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/rock/measure1/character.frag");
    charShader.link();

    character = new Character(charShader);
    // model data


    glPointSize(10.0f);
    glLineWidth(2.0f);


    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &lineEBO);

    jointVAOs.resize(7);
    jointVBOs.resize(7);
    jointAngles.resize(7);
    jointAnglePoses.resize(7);

    glGenVertexArrays(7, &jointVAOs[0]);
    glGenBuffers(7, &jointVBOs[0]);

    for(int i = 0; i < 7; ++i)
    {
        float data[] = {};
        glBindVertexArray(jointVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, jointVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }

    vertices = {};
    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    indices = {};
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //initJointData();
}

void GLModelView::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    QMatrix4x4 projection;
    projection.perspective(camera->Zoom, 640.0f/ 360.0f, 0.1f, 1000.0f);
    QMatrix4x4 view = glmToQMatrix(camera->GetViewMatrix());




    shader.bind();
    shader.setUniformValue("projection", projection);
    shader.setUniformValue("view", view);
    shader.setUniformValue("model", model);


    glBindVertexArray(pointVAO);
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    jointShader.bind();
    jointShader.setUniformValue("projection", projection);
    jointShader.setUniformValue("view", view);
    jointShader.setUniformValue("model", model);
    for(int i = 0 ; i < jointNum; ++i){
        glBindVertexArray(jointVAOs[i]);
        glDrawArrays(GL_LINE_STRIP, 0, 10);

    }
    for(int i = 0 ; i < jointNum; ++i){
        QVector3D temp = projection * view * model * jointAnglePoses[i];
        float x = (temp.x() + 1) / 2.0f * 640.0f;
        float y = (temp.y() + 1) / 2.0f * 360.0f;
        character->renderText(charShader, std::to_string(jointAngles[i]), x, y, 0.3, QVector3D(1, 0, 0));
    }

    glBindVertexArray(0);
    glFlush();
}

void GLModelView::resizeGL(int width, int height)
{

}

void GLModelView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        isLeftClicked = true;
        mouseLastX = event->pos().x();
        mouseLastY = event->pos().y();
    }
    if(event->button() == Qt::RightButton){
        isRightClicked = true;
    }
    update();
}

void GLModelView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        isLeftClicked = false;
    }
    if(event->button() == Qt::RightButton){
        isRightClicked = false;
    }
}

void GLModelView::mouseMoveEvent(QMouseEvent *event)
{
    float deltaX = event->pos().x() - mouseLastX;
    float deltaY = event->pos().y() - mouseLastY;
    if(isLeftClicked && !isRightClicked){
        std::cout<<"move"<<std::endl;
        camera->ProcessLeftMovement(-deltaX, deltaY);
    }
    else if(!isLeftClicked && isRightClicked){
        std::cout<<"rotate"<<std::endl;
        QVector3D axis = QVector3D::crossProduct(QVector3D(deltaX, deltaY, 0), QVector3D(0, 0, 1));
        model.rotate(5, axis);
    }
    else if(isLeftClicked && isRightClicked){
        camera->ProcessDoubleMovement(-deltaX, deltaY);
    }
    mouseLastX = event->pos().x();
    mouseLastY = event->pos().y();
    update();
}

void GLModelView::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0){
        model.scale(1.1f);
    }
    else{
        model.scale(0.9f);
    }
    camera->ProcessMouseScroll(event->delta());
    update();
}

void GLModelView::updateVertices(std::vector<float> &vert)
{
    //vertices.clear();
    vertices = vert;
    if(vert.size() == 3){
        model.translate(-vert[0], -vert[1] + 3, -vert[2]);
    }
    //vertices = {1, 20, 3};

    vNum = vert.size() / 3;
    if(vNum >= 11 ){
        jointNum = 7;
    }
    else if(vNum >= 9){
        jointNum = 5;
    }
    else if(vNum >= 7){
        jointNum = 4;
    }
    else if(vNum >= 6){
        jointNum = 2;
    }
    else if(vNum >= 4){
        jointNum = 1;
    }
    else{
        jointNum = 0;
    }
    indices.clear();
    indices.resize((vNum - 1) * 2);
    switch(vNum){
    case 11:
        indices[18] = 9;
        indices[19] = 10;
    case 10:
        indices[16] = 6;
        indices[17] = 9;
    case 9:
        indices[14] = 7;
        indices[15] = 8;
    case 8:
        indices[12] = 6;
        indices[13] = 7;
    case 7:
        indices[10] = 1;
        indices[11] = 6;
    case 6:
        indices[8] = 4;
        indices[9] = 5;
    case 5:
        indices[6] = 1;
        indices[7] = 4;
    case 4:
        indices[4] = 2;
        indices[5] = 3;
    case 3:
        indices[2] = 1;
        indices[3] = 2;
    case 2:
        indices[0] = 0;
        indices[1] = 1;
    case 1:
    default:
        break;
    }
    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(0);
    initJointData();
}


std::vector<QVector3D> GLModelView::enrichCurve(QVector3D &start, QVector3D &middle, QVector3D &end, int num)
{
    float length = std::min((start - middle).length() * 0.2, (end - middle).length() * 0.2);
    QVector3D ps = (start - middle).normalized();
    QVector3D pe = (end - middle).normalized();
    std::vector<QVector3D> res;
    res.push_back(ps * length + middle);
    for(int i = 1; i < num; i++){
        QVector3D temp = slerp(ps, pe, (float)i / (float)num);
        res.push_back(temp * length + middle);
    }
    res.push_back(pe * length + middle);

    return res;
}

void GLModelView::initJointData()
{
    if(jointNum < 1)
        return;


    for(int i = 0; i < jointNum; ++i)
    {
        QVector3D start, middle, end;
        switch(i){
        case 0:
            start = QVector3D(vertices[3], vertices[4], vertices[5]);
            middle = QVector3D(vertices[6], vertices[7], vertices[8]);
            end = QVector3D(vertices[9], vertices[10], vertices[11]);
            break;
        case 1:
            start = QVector3D(vertices[3], vertices[4], vertices[5]);
            middle = QVector3D(vertices[12], vertices[13], vertices[14]);
            end = QVector3D(vertices[15], vertices[16], vertices[17]);
            break;
        case 2:
            start = QVector3D(vertices[18], vertices[19], vertices[20]);
            middle = QVector3D(vertices[3], vertices[4], vertices[5]);
            end = QVector3D(vertices[6], vertices[7], vertices[8]);
            break;
        case 3:
            start = QVector3D(vertices[18], vertices[19], vertices[20]);
            middle = QVector3D(vertices[3], vertices[4], vertices[5]);
            end = QVector3D(vertices[12], vertices[13], vertices[14]);
            break;
        case 4:
            start = QVector3D(vertices[18], vertices[19], vertices[20]);
            middle = QVector3D(vertices[21], vertices[22], vertices[23]);
            end = QVector3D(vertices[24], vertices[25], vertices[26]);
            break;
        case 5:
            start = QVector3D(vertices[18], vertices[19], vertices[20]);
            middle = QVector3D(vertices[27], vertices[28], vertices[29]);
            end = QVector3D(vertices[30], vertices[31], vertices[32]);
            break;
        case 6:
            start = QVector3D(vertices[21], vertices[22], vertices[23]);
            middle = QVector3D(vertices[18], vertices[19], vertices[20]);
            end = QVector3D(vertices[27], vertices[28], vertices[29]);
            break;
        default:
            break;
        }
        //std::cout<< "start: "<< start.x() << " " <<start.y() << " " << start.z() << std::endl;
        //std::cout<< "middle: "<< middle.x() << " " <<middle.y() << " " << middle.z() << std::endl;
        //std::cout<< "end: "<< end.x() << " " <<end.y() << " " << end.z() << std::endl;
        jointAnglePoses[i] = getAngleTextPosition(start, middle, end);
        float tempAngle = acos(QVector3D::dotProduct((start-middle).normalized(), (end - middle).normalized()));
        jointAngles[i] = tempAngle / 3.1415926 * 180;

        std::vector<QVector3D> joint = enrichCurve(start, middle, end, 9);
        GLfloat *jointData = new GLfloat[joint.size() * 3];
        for(int j = 0; j < joint.size(); ++j){
            //std::cout<< "joint: " << joint[j].x() << " " << joint[j].y() << "  " << joint[j].z() << std::endl;
            jointData[j * 3] = joint[j].x();
            jointData[j * 3 + 1] = joint[j].y();
            jointData[j * 3 + 2] = joint[j].z();
        }
        glBindVertexArray(jointVAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, jointVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * joint.size() * 3, jointData, GL_STATIC_DRAW);

        /*
         * float data[6];
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 6 *sizeof(float), data);
        std::cout<< "data1" << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << " " << data[4] << " " << data[5] << std::endl;
        */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        //delete[] jointData;
    }

}


inline QVector3D slerp(QVector3D &from, QVector3D &to, float t){
    float angle = acos(QVector3D::dotProduct(from.normalized(), to.normalized()));
    return (sin((1 - t) * angle) / sin(angle)) * from + (sin(t * angle) / sin(angle)) * to;
}
QVector3D getAngleTextPosition(QVector3D &start, QVector3D &middle, QVector3D &end)
{
    QVector3D vec1 = middle + (start - middle).normalized() * 0.2;
    QVector3D vec2 = middle + (end - middle).normalized() * 0.2;
    return slerp(vec1, vec2, 0.5);
}


QMatrix4x4 glmToQMatrix(glm::mat4 mat)
{
    QMatrix4x4 res;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            res(i, j) = mat[j][i];
        }
    }
    return res;
}
