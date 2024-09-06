#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 M_Matrix;
uniform mat4 V_Matrix;
uniform mat4 P_Matrix;

void main()
{
    gl_Position = P_Matrix * V_Matrix * M_Matrix * vec4(aPos.xyz, 1.0);
    color = aColor;
    texCoord = aTexCoord;
}