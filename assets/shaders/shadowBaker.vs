#version 330

layout (location = 0) in vec3 position;

uniform mat4 LSM;
uniform mat4 model;

void main()
{
	gl_Position = LSM * model * vec4(position, 1.0f);
}