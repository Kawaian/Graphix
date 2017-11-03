#version 420 core

layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

uniform mat4 LSM;
uniform mat4 model;
uniform mat3 normalMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out VS_OUTPUT
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec4 fragPosLS;
} vs_output;


void main()
{
	gl_Position = Projection * View * model * vec4(position, 1.0f);
	vs_output.fragPos = vec3(model * vec4(position, 1.0f));
	vs_output.normal = normalMatrix * normal;
	vs_output.texCoords = texCoord;
	vs_output.fragPosLS = LSM * vec4(vs_output.fragPos, 1.0);
}
