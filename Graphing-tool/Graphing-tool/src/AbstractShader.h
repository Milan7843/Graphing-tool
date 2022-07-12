#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AbstractShader
{
public:
	// Program ID
	unsigned int ID;

	// Activates the shader
	void use();

	// Uniform setting functions
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setDouble(const std::string& name, double value) const;
	void setInt(const std::string& name, int value) const;
	void setVector2(const std::string& name, float v1, float v2) const;
	void setVector2(const std::string& name, glm::vec2 v) const;
	void setVector3(const std::string& name, float v1, float v2, float v3) const;
	void setVector3(const std::string& name, glm::vec3 v) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;

protected:
	unsigned int compileShader(GLenum type, const char* code);
	bool replace(std::string& str, const std::string& from, const std::string& to);
	std::string readFile(const char* shaderPath);
	void linkProgram(bool throwError);

	// Cannot be instantiated
	AbstractShader() {}
	~AbstractShader() {}
};