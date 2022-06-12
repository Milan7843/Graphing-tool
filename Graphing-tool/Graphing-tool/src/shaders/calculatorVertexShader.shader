#version 460 core
layout(location = 0) in vec3 aPos;

out vec4 vertexColor;

// Buffer that holds the height of every point
layout(std430, binding = 0) buffer Heights
{
	float heights[];
};
// Offset between each vertex, required for index calculation
uniform float offset;
// Number of vertices in each dimension
uniform int size;

uniform bool edgeMode;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;
uniform float graphWidth;
uniform float verticalScale;
uniform vec3 upperColor;
uniform vec3 lowerColor;
uniform int calculationMode;

float calculate(float x, float z);

// Constants
#define pi 3.14159265359
#define e 2.71828182846
#define epsilon 0.001

void main()
{
	// Calculating index from world position
	// Epsilon required to avoid round-down errors (incorrectly rounding down below the intended value)
	int cx = int((aPos.x + 1.0) / offset + epsilon);
	int cz = int((aPos.z + 1.0) / offset + epsilon);

	// Index calculated from the individual indices for x and z
	int i = int(cx + size * cz);

	float y = heights[i];


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
	if (calculationMode == 0)
	{
		return $function;
	}

	float f = $function;
	x += epsilon;
	z += epsilon;
	return (($function) - f) / epsilon;
}