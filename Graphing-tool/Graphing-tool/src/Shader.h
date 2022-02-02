#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// Program ID
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(std::string &function, const char* vertexPath, const char* fragmentPath);
	~Shader();

	// Activates the shader
	void use();

	// Uniform setting functions
	void setBool(const std::string &name, bool value) const;
	void setFloat(const std::string &name, float value) const;
	void setInt(const std::string &name, int value) const;
	void setVector3(const std::string& name, float v1, float v2, float v3) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;

private:
	unsigned int compileShader(GLenum type, const char* code);
	bool replace(std::string& str, const std::string& from, const std::string& to);
};

#endif