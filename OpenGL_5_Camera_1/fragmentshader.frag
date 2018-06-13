in vec2 fTexCoord;

uniform sampler2D iTexture;

void main(void)
{
    gl_FragColor = texture2D(iTexture,fTexCoord);
}
