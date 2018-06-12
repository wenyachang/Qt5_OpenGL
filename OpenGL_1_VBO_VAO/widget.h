#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Widget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Widget(QOpenGLWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

    void initVBO();

private:
    QOpenGLShaderProgram* program;

    int matrixLocation;
    int vertexLocation ;
    int colorLocation;

    GLfloat vertex[6] = {
        -0.5f,0.0f,
        0.5f,0.0f,
        0.0f,0.5f
    };

    GLuint triIndexs[3] = {0,2,1};

    GLfloat colors[12] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    GLuint verVbo;
    GLuint indexVbo;
    GLuint colorVbo;

    QMatrix4x4 projection;

    GLuint programId;
    int vVerticesLen;
    int triIndexLen;
    int colorsLen ;
};

#endif // WIDGET_H
