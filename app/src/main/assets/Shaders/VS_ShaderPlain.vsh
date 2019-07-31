#version 320 es

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 uPMatrix;
uniform mediump float z;

void main()
{
    gl_Position = uPMatrix * vec4(position, z, 1.0f);
    TexCoord = texCoord;
}