#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std140, binding = 0) buffer Vertices
{
	vec3 vertices[];
};

layout(std140, binding = 1) buffer Indices
{
	int indices[];
};

void main()
{

}