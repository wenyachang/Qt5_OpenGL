#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTime>

class Widget : public QOpenGLWidget,protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    explicit Widget(QOpenGLWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);

    void display();

private:
    QOpenGLShaderProgram* program;

    GLfloat vertex[15] = {
        -1.0f,1.0f,0,
        -1.0f,-1.0f,0,
        1.0f,-1.0f,0,
        1.0f,1.0f,0,
        0.0f,0.0f,2.0f
    };

    GLuint triIndexs[18] = {0,1,2,2,0,3,1,2,4,2,3,4,3,0,4,0,1,4};

    GLfloat textures[36] ={
        0.0f,1.0f,
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        0.5f,0.5f
    };

    QVector3D cubePositions[10] = {
      QVector3D( 0.0f,  0.0f,  -1.0f),
      QVector3D( 2.0f,  5.0f, -15.0f),
      QVector3D(-1.5f, -2.2f, -2.5f),
      QVector3D(-3.8f, -2.0f, -12.3f),
      QVector3D( 2.4f, -0.4f, -3.5f),
      QVector3D(-1.7f,  3.0f, -7.5f),
      QVector3D( 1.3f, -2.0f, -2.5f),
      QVector3D( 1.5f,  2.0f, -2.5f),
      QVector3D( 1.5f,  0.2f, -1.5f),
      QVector3D(-1.3f,  1.0f, -1.5f)
    };

    GLuint programId;

    GLuint vertexLocation;
    GLuint texLocation;

    GLuint vao;

    QOpenGLTexture* texture;
    QTime time;
};

#endif // WIDGET_H
