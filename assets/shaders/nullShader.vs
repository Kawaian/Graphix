#version 420 core

layout (location = 0) in vec3 position;

layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

uniform mat4 model;

void main()
{
      gl_Position = ViewProjection * model * vec4(position, 1.0f);
}
