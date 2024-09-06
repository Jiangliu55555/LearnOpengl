#version 330 core

in vec3 color;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D _Texture1;
uniform sampler2D _Texture2;

uniform float _Intensity;

void main()
{
    vec4 color0 = texture(_Texture1, texCoord);
    vec4 color1 = texture(_Texture2, texCoord);
    vec3 color = mix(color0.rgb, color1.rgb, 0.5f);
    FragColor = vec4(color.rgb * _Intensity, 1.0f);
}