#version 420 core

layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out VS_OUTPUT
{
	out vec3 position;
	out vec2 texCoord;
	out mat3 TBN;
} vs_output;

uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
	gl_Position        =  ViewProjection * model * vec4(position, 1.0f);
	vs_output.position =   (model * vec4(position, 1.0f)).xyz;
	vs_output.texCoord =   texCoord;
	vec3 tangent0      =   (model * vec4(tangent, 1.0f)).xyz;

	///Compute vs_output.TBN Matrix
	vec3 Normal = normalize(normalMatrix * normal);
	vec3 Tangent = normalize(tangent0);
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	vec3 BiTangent = cross(Tangent, Normal);
	vs_output.TBN = mat3(Tangent, BiTangent, Normal);

}
