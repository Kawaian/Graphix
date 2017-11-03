#version 420 core

layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

layout (location = 0) in vec3 position;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position, 1.0f);
}