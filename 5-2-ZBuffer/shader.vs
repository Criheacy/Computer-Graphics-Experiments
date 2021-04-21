#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float aOffset;

out vec4 vertexColor;

void main()
{
    float posX = aPos.x + aOffset;
    gl_Position = vec4(posX, aPos.y, aPos.z, 1.0);
    vertexColor = vec4(aColor, 1.0);
}