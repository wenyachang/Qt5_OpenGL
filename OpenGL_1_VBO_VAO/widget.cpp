#include "widget.h"

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

    matrixLocation = glGetUniformLocation(programId,"matrix");
    vertexLocation = glGetAttribLocation(programId,"vPosition");
    colorLocation = glGetAttribLocation(programId,"vColor");

    initVBO();
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    //glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, projection.data());

    //绑定初始化时申请的颜色数组
    glBindBuffer(GL_ARRAY_BUFFER,colorVbo);
    //申请颜色数组gpu存储空间时，只是在这块空间中存储了一串数据，并不知道这个数据是怎么存的，glVertexAttribPointer函数在这就说明的存储的格式
    //colorLocation:在顶点着色器中的位置，4：由4个值组成，GL_FLOAT：指定数据类型
    /*glVertexAttribPointer函数的参数非常多，所以我会逐一介绍它们：
      第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
      第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
      第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
      第四个参数参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
      第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
      最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。*/
    glVertexAttribPointer(colorLocation,4,GL_FLOAT,GL_FALSE,0,0);
    //启用颜色数组
    glEnableVertexAttribArray(colorLocation);



    // shader绑定并启用顶点数组buffer
    glBindBuffer(GL_ARRAY_BUFFER, verVbo);
    glVertexAttribPointer( vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLocation);

    // shader绑定并顶点索引数组buffer - 索引无需启用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVbo);
    glDrawElements(GL_TRIANGLES, triIndexLen, GL_UNSIGNED_INT,0);

    // 解绑buffer、关闭启用顶点、颜色数组
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(vertexLocation);
    glDisableVertexAttribArray(colorLocation);

}

void Widget::resizeGL(int w, int h)
{
   glViewport(0,0,w,h);

//   projection.setToIdentity();

//   qreal aspect = qreal(w)/qreal(h?h:1);

//   projection.perspective(60.0f,aspect,1.0f,50.0f);

//   projection.translate(0.0f,0.0f,-2.0f);
}

void Widget::initVBO()
{
     vVerticesLen = sizeof(vertex)/sizeof(GLfloat);
     triIndexLen = sizeof(triIndexs)/sizeof(GLuint);
     colorsLen = sizeof(colors)/sizeof(GLfloat);

    //向GPU申请存储空间，返回int型的句柄
    glGenBuffers(1/*申请存储空间数量*/,&verVbo);
    //指明这一块申请的存储空间，存储的是什么类型的缓冲对数据，例如，顶点数据，索引数据等。
    //Opengl有很多缓冲对象类型：GL_ARRAY_BUFFER（顶点数据）  GL_ELEMENT_ARRAY_BUFFER（索引数据）等
    //将申请的存储空间绑定到context上，则当前的操作都是在这块申请的空间上操作的，直到它解绑
    glBindBuffer(GL_ARRAY_BUFFER,verVbo);
    //将顶点数据复制到刚申请的存储空间上，通过句柄
    glBufferData(GL_ARRAY_BUFFER,vVerticesLen*sizeof(GLfloat)/*数据长度*/,vertex/*数据*/,GL_STATIC_DRAW/*数据不变*/);

    // 初始化索引buffer并装载数据到显存
    glGenBuffers(1, &indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndexLen * sizeof(GLuint), triIndexs, GL_STATIC_DRAW);

    // 初始化颜色buffer并装载数据到显存
    glGenBuffers(1, &colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, colorsLen * sizeof(GLfloat), colors, GL_STATIC_DRAW);

}
