#version 320 es

layout (location = 0) in vec2 position;

uniform mat4 uPMatrix;
uniform mediump float z;

void main()
{
    gl_Position = uPMatrix * vec4(position, z, 1.0f);
}