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
};

struct Material
{
    float shininess;
    vec3  specularColor;
    float ambientCoeff;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 2) uniform sampler2D dispMap;

uniform vec3 cameraPosition;
uniform float dispMapScale;
uniform float dispMapBias;
uniform PointLight pointLight;
uniform Material material;

const float GAMMA = 2.2f;

in VS_OUTPUT
{
    in vec3 position;
    in vec2 texCoord;
    in mat3 TBN;
} fs_input;

out vec4 fragmentColor;

vec3 ComputeBumpedNormal(vec2 texCoord)
{
	vec3 BumpMapNormal = texture(normalMap, texCoord).xyz;
	BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
	vec3 newNormal = normalize(fs_input.TBN *  BumpMapNormal);
	return newNormal;
}

vec2 ComputeParrallaxDisp(vec3 surfaceToCamera)
{
	return fs_input.texCoord.xy + (surfaceToCamera * fs_input.TBN).xy * (texture2D(dispMap, fs_input.texCoord.xy).r * dispMapScale + dispMapBias);
}

                                        /*! Does not comupute ambient becouse of the attenuation factor
                                            that can be added later on !                                !*/

vec3 ComputeLightInternal(BaseLight light, vec4 surfaceColor, vec3 surfaceToLight, vec3 surfaceToCamera, vec3 normal)
{
    //diffuse
    float diffuseCoeff = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoeff * pow(surfaceColor.rgb, vec3(GAMMA)) * pointLight.base.intensities;
/*
    //specular
    float specularCoeff = 0.0;
    if(diffuseCoeff > 0.0)
        specularCoeff = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess);
*/

	vec3 halfWayDir = normalize(surfaceToLight + surfaceToCamera);

	float specularCoeff = pow(max(dot(normal, halfWayDir), 0.0), material.shininess);



    vec3 specular = specularCoeff * material.specularColor * pointLight.base.intensities;

    return diffuse + specular;
}

vec3 ComputeSpotLight()
{
    return vec3(0.0, 0.0, 0.0);
};

vec3 ComputePointLight(PointLight light, vec4 surfaceColor, vec3 surfaceToCamera, vec3 normal)
{
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = normalize(light.position - fs_input.position);

    vec3 Color = ComputeLightInternal(light.base, surfaceColor, surfaceToLight, surfaceToCamera, normal);

    //ambient
    vec3 ambient = light.base.ambientCoeff *  surfaceColor.rgb * pointLight.base.intensities;

    //attenuation
    float distanceToLight = length(pointLight.position - fs_input.position);
    float attenuation = light.attenuation.constant +
                        light.attenuation.linear * distanceToLight +
                        light.attenuation.exponential * distanceToLight * distanceToLight;

    return ambient + (Color / attenuation);
}

void main()
{
	//cam distance
    vec3 surfaceToCamera = normalize(cameraPosition - fs_input.position);

	//Apply Normal Map + Parrallax Displacment Map
	vec2 texCoord = ComputeParrallaxDisp(surfaceToCamera);
    vec3 normal = ComputeBumpedNormal(texCoord);

    //color of the current fragment
    vec4 surfaceColor = texture(diffuseMap, texCoord);

    vec3 totalLight = ComputePointLight(pointLight, surfaceColor, surfaceToCamera, normal);

    fragmentColor = vec4(totalLight, surfaceColor.a);
    fragmentColor.rgb = pow(fragmentColor.rgb, vec3(1.0 / GAMMA));
}
