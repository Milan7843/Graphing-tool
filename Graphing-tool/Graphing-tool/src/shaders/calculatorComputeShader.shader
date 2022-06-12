#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer Heights
{
	float heights[];
};

uniform int size;
uniform float offset;

void main()
{
	// Calculating the 2 dimensional indices
	int cx = int(gl_GlobalInvocationID.x);
	int cz = int(gl_GlobalInvocationID.y);

	// Calculating world position from index
	float x = (float(cx) * offset - 1.0);
	float z = (float(cz) * offset - 1.0);

	// Calculating the total index, used to map the 2D indices to a 1D array
	int i = int(cx + size * cz);

	// Assigning the value
	heights[i] = float($function);
}