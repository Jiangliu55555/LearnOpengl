#version 330 core

in vec3 color;
out vec4 FragColor;

uniform float _Intensity;

void main()
{
    FragColor = vec4(color * _Intensity, 1.0f);
}