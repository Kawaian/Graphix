#version 330 core

in vec2 position;
in vec2 texCoord;

out vec2 texCoord0;

void main()
{
	vec2 positionh = position - vec2(400.0, 300.0);
	positionh /= vec2(400.0, 300.0);
	gl_Position = vec4(positionh, 0.0, 1.0);

	texCoord0 = texCoord;
}
