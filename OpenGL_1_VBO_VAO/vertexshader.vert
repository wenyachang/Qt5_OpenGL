uniform mat4 matrix;
in vec4 vPosition;
in vec4 vColor;
out vec4 fColor;

void main()
{
    fColor=vColor;
    //gl_Position = matrix * vPosition;
    gl_Position = vPosition;
}
