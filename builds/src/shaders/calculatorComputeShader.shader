#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer Heights
{
	float heights[];
};

uniform int size;
uniform float offset;
uniform float scale;
uniform float graphWidth;

// User variables
uniform float a;
uniform float b;
uniform float c;
uniform float d;
uniform float e;
uniform float f;


// Constants
#define pi 3.14159265359
//#define e 2.71828182846
#define epsilon 0.001

void main()
{
	// Calculating the 2 dimensional indices
	int cx = int(gl_GlobalInvocationID.x);
	int cz = int(gl_GlobalInvocationID.y);

	// Calculating world position from index
	float x = (float(cx) * offset - 1.0) * scale * graphWidth;
	float z = (float(cz) * offset - 1.0) * scale * graphWidth;

	// Calculating the total index, used to map the 2D indices to a 1D array
	int i = int(cx + size * cz);

	// Assigning the value
	float height = float($function) / scale;
	heights[i] = height;
}