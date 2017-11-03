#version 330 core

uniform samplerCube texCubeMap;

in vec3 texCoord0;

out vec4 fragColor;

void main()
{
	fragColor = texture(texCubeMap, texCoord0);
}

