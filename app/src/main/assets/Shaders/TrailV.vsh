#version 320 es

layout (location = 0) in vec3 position;
layout (location = 1) in float alpha;

out mediump float brightness;
uniform mat4 uPMatrix;

void main()
{
    gl_Position = uPMatrix * vec4(position, 1.0f);
    brightness = alpha;
}