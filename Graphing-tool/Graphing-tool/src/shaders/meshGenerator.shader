#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std140, binding = 0) buffer Vertices
{
	float vertices[];
};

layout(std140, binding = 1) buffer Indices
{
	int indices[];
};


int cx = int(gl_GlobalInvocationID.x);
int cy = int(gl_GlobalInvocationID.y);

int i = int(gl_LocalInvocationIndex);

void main()
{
	float xoffset = 2.0 / (400.0 - 1.0);
	float yoffset = 2.0 / (400.0 - 1.0);

	vertices[i * 3] = (cx * xoffset - 1.0f); // x
	vertices[i * 3 + 1] = 0.0f; // y
	vertices[i * 3 + 2] = (cy * yoffset - 1.0f); // z
}