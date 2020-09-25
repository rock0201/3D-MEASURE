#ifndef CHARACTER_H
#define CHARACTER_H

// GLAD
#include <glad/glad.h>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
// glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
// Qt
#include <QtGui/QMatrix>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
// c++
#include <iostream>
#include <map>
#include <string>

const GLuint WIDTH = 640, HEIGHT = 360;

struct SCharacter{
    GLuint TextureID;
    QVector2D Size;
    QVector2D Bearing;
    GLuint Advance;
};

class Character : protected QOpenGLFunctions_3_3_Core{
private:
        std::map<GLchar, SCharacter> Characters;
        GLuint VBO;
        void init();
public:
        GLuint VAO;
        Character(QOpenGLShaderProgram &shader);
        void renderText(QOpenGLShaderProgram &shader,std::string text, GLfloat x, GLfloat y, GLfloat scale, QVector3D color);
};

#endif // CHARACTER_H
