#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer Vertices
{
	float vertices[];
};

layout(std430, binding = 1) buffer Indices
{
	int indices[];
};

uniform int size;

uniform float offset;

void main()
{
	// Calculating the 2 dimensional indices
	int cx = int(gl_GlobalInvocationID.x);
	int cy = int(gl_GlobalInvocationID.y);

	// Calculating the total index, used to map the 2D indices to a 1D array
	int i = int(cx + size * cy);

	vertices[i * 3]		= (cx * offset - 1.0);	// x
	vertices[i * 3 + 1] = 0.0;						// y
	vertices[i * 3 + 2] = (cy * offset - 1.0);	// z

	// Index generation
	if (cx < size - 1 && cy < size - 1)
	{
		// 0 1
		// 2 3
		// Right tri: 0 1 3
		indices[i * 6 + 0] = cx + cy * size; // 0
		indices[i * 6 + 1] = cx + cy * size + 1; // 1
		indices[i * 6 + 2] = cx + cy * size + size + 1; // 3
		// Left tri: 0 3 2	 ]
		indices[i * 6 + 3] = cx + cy * size; // 0
		indices[i * 6 + 4] = cx + cy * size + size + 1; // 3
		indices[i * 6 + 5] = cx + cy * size + size; // 2
	}
}