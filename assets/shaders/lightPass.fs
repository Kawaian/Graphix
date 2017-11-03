#version 420 core

struct BaseLight
{
    vec3 intensities;//a.k.a color of light
    float ambientCoeff;
};

struct Attenuation
{
    float constant;
    float linear;
    float exponential;
};

struct PointLight
{
    BaseLight base;
    Attenuation attenuation;
    vec3 position;
    float radius;
};

struct Material
{
    float shininess;
    vec3  specularColor;
    float ambientCoeff;
};


layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

layout(binding = 0) uniform sampler2D gPositionMap;
layout(binding = 1) uniform sampler2D gAlbedoMap;
layout(binding = 2) uniform sampler2D gNormalMap;
layout(binding = 3) uniform sampler2D gSpecularMap;

uniform vec3 cameraPosition;
uniform PointLight pointLight;

out vec4 fragmentColor;

const float GAMMA = 2.2f;

vec2 FetchTexCoord()
{
      return gl_FragCoord.xy / scrResolution;
}

vec3 ComputeRimLight(vec3 normal, vec3 view)
{
	float f = 1.0f - dot(normal, view);
	f = smoothstep(0.0f, 1.0f, f);
	f = pow(f, 0.7);
	return f * vec3(1.0f, 1.0f, 1.0f);
}

void main()
{
    vec2 texCoord = FetchTexCoord();
    vec3 gPosition = 		texture(gPositionMap, texCoord).xyz;
    vec3 gSurfaceColor = 	texture(gAlbedoMap, texCoord).xyz;
    vec3 gNormal = 			texture(gNormalMap, texCoord).xyz;
	vec3 gSpecColor = 		texture(gSpecularMap, texCoord).xyz;
	float gSpecPower = 		texture(gSpecularMap, texCoord).a;
 	float gDepth = 			texture(gPositionMap, texCoord).a;

	vec3 totalLight = vec3(0.0f);
	vec3 viewDir = normalize(cameraPosition - gPosition);

	vec3 lightDir = normalize(pointLight.position - gPosition);
	vec3 diffuse = max(dot(gNormal, lightDir), 0.0f) * gSurfaceColor *
	 				pointLight.base.intensities;

	vec3 halfWayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(gNormal, halfWayDir), 0.0f), 80.0f);
	vec3 specular = pointLight.base.intensities * spec /** gSpecColor*/;

	float distance = length(pointLight.position - gPosition);

	float atten = pointLight.attenuation.constant + pointLight.attenuation.linear * distance 
	 	+ pointLight.attenuation.exponential * distance * distance;
    float atten0 = pointLight.attenuation.constant;
    float attenR = pointLight.attenuation.constant + pointLight.attenuation.linear * pointLight.radius +
    		pointLight.attenuation.exponential * pointLight.radius * pointLight.radius;

	totalLight += (diffuse + specular) * max(0.0, (1./atten - 1./attenR)/(1./atten0 - 1./attenR));

	fragmentColor = vec4(totalLight, 1.0f);
    }
