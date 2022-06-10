
#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = readFile(vertexPath);
	std::string fragmentCode = readFile(fragmentPath);
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	/* Compiling the shaders */

	unsigned int vertex, fragment;
	vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
	fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);


	/* Creating the shader program */
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	linkProgram(false);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(std::string &function, const char* vertexPath, const char* fragmentPath, bool throwError)
{
	std::string vertexCode = readFile(vertexPath);
	std::string fragmentCode = readFile(fragmentPath);
	std::cout << replace(vertexCode, "$function", function);
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	/* Compiling the shaders */

	unsigned int vertex, fragment;
	vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
	fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);


	/* Creating the shader program */

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	linkProgram(throwError);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	std::cout << "Shader object destroyed.";
}