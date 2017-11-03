#version 450 core

layout (binding = 0) uniform sampler2D gColorMap;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 bloomColor;


layout (std140) uniform Viewport
{
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 ViewProjection;
	uniform vec2 scrResolution;
};

uniform float exposure = 0.5f;
const float GAMMA = 2.2f;

vec2 FetchTexCoord()
{
      return gl_FragCoord.xy / scrResolution;
}

void main()
{

	vec2 texCoord = FetchTexCoord();
    vec3 hdrColor = texture(gColorMap, FetchTexCoord()).xyz;

 	vec3 mapped	= vec3(1.0f) - exp(-hdrColor * exposure);
 	//mapped = pow(mapped, vec3(1.0f / GAMMA));
 	fragmentColor = vec4(mapped, 1.0f);
 	bloomColor = vec4(0.5f);

/*
    int i;
    float lum[25];
    vec2 tex_scale = vec2(1.0f) / textureSize(gColorMap, 0);

    for(i = 0; i < 25; i++)
    {
        vec2 tc = (2.0f * gl_FragCoord.xy +
                    3.5 * vec2(i % 5 - 2, i / 5 - 2));
        vec3 col = texture(gColorMap, tc * tex_scale).rgb;
        lum[i] = dot(col, vec3(0.3, 0.59, 0.11));
    }

    vec3 vColor = texelFetch(gColorMap,
                              2 * ivec2(gl_FragCoord.xy),
                              0).rgb;
    float kernelLuminance = (
            (1.0 * (lum[0] + lum[4] + lum[20] + lum[24])) +
            (4.0 * (lum[1] + lum[3] + lum[5] + lum[9] +
                lum[15] + lum[19] + lum[21] + lum[23])) +
            (7.0 * (lum[2] + lum[10] + lum[14] + lum[22])) +
            (16.0 * (lum[6] + lum[8] + lum[16] + lum[18])) +
            (26.0 * (lum[7] + lum[11] + lum[13] + lum[17])) +
            (41.0 * lum[12])
    ) / 273.0;

    float exposure = sqrt(8.0f / (kernelLuminance + 0.25f));

    fragmentColor.rgb = 1.0f - exp2(-vColor * exposure);
    fragmentColor.a = 1.0f;
    fragmentColor.rgb = pow(fragmentColor.rgb, vec3(1.0/GAMMA));
*/
}
