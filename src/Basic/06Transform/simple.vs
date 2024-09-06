#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 _Transform;

void main()
{
    gl_Position = _Transform * vec4(aPos.xyz, 1.0);
    color = aColor;
    texCoord = aTexCoord;
}