#version 420 core

layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

layout(binding = 1) uniform sampler2D gAlbedoMap;
out vec4 fragmentColor;

vec2 FetchTexCoord()
{
      return gl_FragCoord.xy / scrResolution;
}

void main()
{
    vec2 texCoord = FetchTexCoord();
    vec3 gSurfaceColor = texture(gAlbedoMap, texCoord).xyz;
    
	vec3 totalLight = gSurfaceColor * 0.1f; //TODO remove hardcoded ambient light
	fragmentColor = vec4(totalLight, 1.0f);
}
