#version 420 core

layout (vertices = 3) out;

uniform vec3 cameraPosition;

in VS_OUTPUT
{
	out vec3 position;
	out vec2 texCoord;
	out mat3 TBN;
} tess_input[];

out VS_OUTPUT
{
	out vec3 position;
	out vec2 texCoord;
	out mat3 TBN;
} tess_output[];

float GetTessLevel(float distance0, float distance1)
{
	float avgDistance = (distance0 + distance1) / 2.0f;

	if (avgDistance <= 2.0f)
	{
		return 10.0f;
	}
	else if (avgDistance <= 5.0)
	{
		return 7.0f;
	}
	else
	{
		return 3.0f;
	}
}

void main()
{
	//control points of output patch
	tess_output[glInvocationID].texCoord = tess_input[glInvocationID].texCoord;
	tess_output[glInvocationID].TBN = tess_input[glInvocationID].TBN;
	tess_output[glInvocationID].position = tess_input[glInvocationID].position;

	//gen tessalation
	float camToVertex0 = distance(cameraPosition, tess_output[0].position);
	float camToVertex1 = distance(cameraPosition, tess_output[1].position);
	float camToVertex2 = distance(cameraPosition, tess_output[2].position);

	gl_TessLevelOuter[0] = GetTessLevel(camToVertex1, camToVertex2); 
	gl_TessLevelOuter[1] = GetTessLevel(camToVertex2, camToVertex0);
	gl_TessLevelOuter[2] = GetTessLevel(camToVertex0, camToVertex1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}

