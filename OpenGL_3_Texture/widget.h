#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

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

    GLfloat vertex[12] = {
        -0.8,-0.4,
        -0.4,-0.4,
        -0.4,0.4,
        0.4,-0.4,
        0.4,0.4,
        0.8,-0.4
    };

    GLuint triIndexs[12] = {0,1,2,2,1,3,3,2,4,4,3,5};

    GLfloat colors[24] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    GLfloat textures[12] ={
       0.0f,0.0f,
        0.25f,0.0f,
        0.25f,1.0f,
        0.75f,0.0f,
        0.75f,1.0f,
        1.0f,0.0f
    };

    GLuint programId;

    GLuint vertexLocation;
    GLuint colorLocation;
    GLuint texLocation;

    GLuint vao;

    QOpenGLTexture* texture;
};

#endif // WIDGET_H
