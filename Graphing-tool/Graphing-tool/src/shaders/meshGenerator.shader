#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer Vertices
{
	float vertices[];
};

layout(std430, binding = 1) buffer Indices
{
	unsigned int indices[];
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
		// Making sure these calculations get done only once instead of six times
		int baseIndex = cx + cy * size;
		unsigned int offsetIndex = i * 6
			- (i/size)*6; // Compensating for the fact that this index should skip the edges, while these are included in the original index

		// 0 1
		// 2 3
		// Right tri: 0 1 3
		indices[offsetIndex + 0] = baseIndex; // 0
		indices[offsetIndex + 1] = baseIndex + 1; // 1
		indices[offsetIndex + 2] = baseIndex + size + 1; // 3
		// Left tri: 0 3 2	 ]
		indices[offsetIndex + 3] = baseIndex; // 0
		indices[offsetIndex + 4] = baseIndex + size + 1; // 3
		indices[offsetIndex + 5] = baseIndex + size; // 2
	}
}