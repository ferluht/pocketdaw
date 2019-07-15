attribute highp vec3    myVertex;

uniform highp mat4      uPMatrix;


void main(void)
{
    highp vec4 p = vec4(myVertex,1);
    gl_Position = uPMatrix * p;
}