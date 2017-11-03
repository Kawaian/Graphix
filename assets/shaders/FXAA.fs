#version 420 core

layout(binding = 0) uniform sampler2D gColorMap;
out vec4 fragmentColor;

const float GAMMA = 2.2f;


layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

vec2 FetchTexCoord()
{
      return gl_FragCoord.xy / scrResolution;
}

void main()
{
	vec2 texCoord = FetchTexCoord();
    vec3 gColor = texture(gColorMap, FetchTexCoord()).xyz;
 
 	fragmentColor = vec4(gColor, 1.0f) + vec4(1.0f);
}
