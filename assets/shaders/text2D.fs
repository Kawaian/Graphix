#version 330 core

uniform sampler2D fontMap;

in vec2 texCoord0;
out vec4 fragColor;

void main()
{
	fragColor = texture2D(fontMap, texCoord0);
}
