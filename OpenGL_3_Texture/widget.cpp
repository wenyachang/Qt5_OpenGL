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

    glBindVertexArray(vao);

    //vao,ibo绘制梯形
    //1
    glDrawArrays(GL_TRIANGLES,0,3);
    glDrawArrays(GL_TRIANGLES,1,3);
    glDrawArrays(GL_TRIANGLES,2,3);
    glDrawArrays(GL_TRIANGLES,3,3);

    //2
    //glDrawElements(GL_TRIANGLES, sizeof(triIndexs), GL_UNSIGNED_INT,0);

    //解绑vao
    glBindVertexArray(0);
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
    glVertexAttribPointer(vertexLocation,2,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(vertexLocation);

    GLuint colorVbo;
    glGenBuffers(1,&colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER,colorVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
    glVertexAttribPointer(colorLocation,4,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(colorLocation);

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


    /*
    加载纹理时，需设置处理纹理的三种属性
    1.纹理环绕方式
     环绕方式	    描述
     GL_REPEAT	对纹理的默认行为。重复纹理图像。
     GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
     GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
     GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
    2.纹理过滤
    3.多级渐远纹理
    */
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
}

