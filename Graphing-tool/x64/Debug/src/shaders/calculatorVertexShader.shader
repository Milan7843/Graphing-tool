#version 330 core
layout(location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform bool edgeMode;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;
uniform float graphWidth;
uniform vec3 upperColor;
uniform vec3 lowerColor;

float calculate(float x, float z);

// Constants
#define pi 3.14159265359
#define e 2.71828182846

void main()
{
	float y = calculate(aPos.x, aPos.z)/scale;
	gl_Position = projection * view * model * vec4(aPos.x * graphWidth, y, aPos.z * graphWidth, 1.0);
	if (edgeMode)
	{
		float yt = (y + 1.0) / 2.0;
		vertexColor = vec4(yt*1.2, 0.0, (1.0 - yt)*1.2, 1.);
		vertexColor = vec4((yt * upperColor + (1-yt) * lowerColor) * 1.2, 1.);
	}
	else
	{
		float yt = (y + 1.0) / 2.0;
		vertexColor = vec4((yt * upperColor + (1 - yt) * lowerColor), 1.);
	}
}

float calculate(float x, float z)
{
	x = x * scale * graphWidth;
	z = z * scale * graphWidth;
	return $function;
}