#include "widget.h"
#include <QOpenGLFunctions_3_3_Core>

Widget::Widget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    program = new QOpenGLShaderProgram(this);

    if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/vertexshader.vert"))
        return;

    if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/fragmentshader.frag"))
        return;

    if(!program->link())
        return;

    if(!program->bind())
        return;


    programId =program->programId();

    vertexLocation = glGetAttribLocation(programId,"vPosition");
    colorLocation = glGetAttribLocation(programId,"vColor");
    texLocation = glGetAttribLocation(programId,"vTexCoord");

    display();
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glActiveTexture(0);
    texture->bind(0);

    QMatrix4x4 model;
    model.rotate((float)time.elapsed()/10,QVector3D(1.0f,0.5f,0.0f));

    //观察的模型，平移、旋转、缩放
    program->setUniformValue(program->uniformLocation("model"),model);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(triIndexs), GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    update();
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Widget::display()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint verVbo;
    glGenBuffers(1,&verVbo);
    glBindBuffer(GL_ARRAY_BUFFER,verVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(vertexLocation);

    GLuint texVbo;
    glGenBuffers(1,&texVbo);
    glBindBuffer(GL_ARRAY_BUFFER,texVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(textures),textures,GL_STATIC_DRAW);
    glVertexAttribPointer(texLocation,2,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(texLocation);


    GLuint indexVbo;
    glGenBuffers(1,&indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triIndexs),triIndexs,GL_STATIC_DRAW);


    texture = new QOpenGLTexture(QImage(":/resource/container.jpg").mirrored(),QOpenGLTexture::GenerateMipMaps/*3*/);

    if(!texture->isCreated())
    {
         qDebug() << "Failed to load texture" << endl;
    }
    texture->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);//1
    texture->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);//2
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    program->setUniformValue(program->uniformLocation("iTexture"),0);

    glBindVertexArray(0);

    time.start();
    QMatrix4x4 projection,view;
    projection.perspective(65,1.0f,0.1f,100.0f);
    view.translate(QVector3D(0.0f,0.0f,-1.0f));
    view.scale(0.3);

    //观察点
    program->setUniformValue(program->uniformLocation("view"),view);

    //perspective 投影矩阵
    program->setUniformValue(program->uniformLocation("projection"),projection);

}

