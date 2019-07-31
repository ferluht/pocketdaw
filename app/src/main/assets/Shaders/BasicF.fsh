#version 320 es

out mediump vec4 outcolor;

uniform mediump mat2 angle;
uniform mediump vec4 color;

void main()
{
    outcolor = color;
}
