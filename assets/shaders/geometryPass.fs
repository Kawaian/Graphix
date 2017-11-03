#version 420 core

struct Material
{
    float shininess;
    vec3  specularColor;
    float ambientCoeff;
};

in VS_OUTPUT
{
	in vec3 position;
	in vec2 texCoord;
	in mat3 TBN;
} fs_input;

layout (location = 0) out vec4 positionOut;
layout (location = 1) out vec3 albedoOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec4 specularOut;

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D normalMap;

uniform Material material;

const float NEAR = 0.01f; // TODO(MattMatt) Remove hardcoded near and far planes
const float FAR = 500.0f; 

const float GAMMA = 2.2f;

vec3 ComputeBumpedNormal(vec2 texCoord)
{
	vec3 BumpMapNormal = texture(normalMap, texCoord).xyz;
	BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
	vec3 newNormal = normalize(fs_input.TBN *  BumpMapNormal);
	return newNormal;
}

vec4 hqFilter(sampler2D samp, vec2 tc)
{
	vec2 texSize = textureSize(diffuseMap, 0);
	vec2 uvScaled = tc * texSize + 0.5f;
	vec2 uvInt = floor(uvScaled);
	vec2 uvFrac = fract(uvScaled);
	uvFrac = smoothstep(0.0f, 1.0f, uvFrac);
	vec2 uv = (uvInt + uvFrac - 0.5f) / texSize;
	return texture(samp, uv);
}

float LinearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;
	return (2.0f * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main()
{
	positionOut.xyz = fs_input.position;
	positionOut.a = LinearDepth(gl_FragCoord.z);
	albedoOut  =    //pow(hqFilter(diffuseMap, fs_input.texCoord).rgb, vec3(GAMMA));
				      hqFilter(diffuseMap, fs_input.texCoord).rgb;	
	normalOut = ComputeBumpedNormal(fs_input.texCoord);//normalize(fs_input.normal);
	specularOut.xya = material.specularColor;
	specularOut.z = material.shininess;
}
