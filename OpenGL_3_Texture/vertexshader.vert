in vec4 vPosition;
in vec3 vColor;
in vec2 vTexCoord;

out vec2 fTexCoord;

void main()
{
    fTexCoord=vTexCoord;
    gl_Position = vPosition;
}
