#version 320 es

out mediump vec4 color;
in mediump float brightness;
uniform mediump mat2 angle;

void main()
{
    color = vec4(0, 1, 0, 1);
}