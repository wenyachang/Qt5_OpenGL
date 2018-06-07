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

    displayVAO();
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glBindVertexArray(vao);

    //vao,ibo绘制梯形
    //1
    glDrawArrays(GL_TRIANGLES,0,3);
    glDrawArrays(GL_TRIANGLES,1,3);
    glDrawArrays(GL_TRIANGLES,2,3);
    glDrawArrays(GL_TRIANGLES,3,3);

    //2
    glDrawElements(GL_TRIANGLES, sizeof(triIndexs), GL_UNSIGNED_INT,0);

    //解绑vao
    glBindVertexArray(0);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Widget::displayVAO()
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

    GLuint indexVbo;
    glGenBuffers(1,&indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triIndexs),triIndexs,GL_STATIC_DRAW);


    glBindVertexArray(0);
}

/*
opengl中VAO,VBO,IBO用法小结
这三个玩意全面取代旧社会的glBegin/glEnd,显示列表,顶点数组。

VBO为顶点缓冲区对象，用于存储顶点坐标/顶点uv/顶点法线/顶点颜色，也可以用于其他用途，比如tangent。

IBO为索引缓冲区，里面的值可以是unsigned int或者unsigned short。

最后用VAO将多个VBO，IBO绑定到一起。

一个常规的网格模型就是VAO.attach(PositionVBO,TexcoordVBO,NormalVBO,IBO)
*/
