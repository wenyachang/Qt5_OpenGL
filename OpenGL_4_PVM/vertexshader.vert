in vec4 vPosition;
in vec3 vColor;
in vec2 vTexCoord;

out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    fTexCoord=vTexCoord;
    gl_Position = projection*view*model*vPosition;
    //gl_Position = model*projection*view*vPosition;
}
