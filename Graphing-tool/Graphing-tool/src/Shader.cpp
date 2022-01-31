
#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	/* Retrieving the shader data from the files */

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Ensuring ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Opening files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// Reading file into streams
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Closing file handlers
		vShaderFile.close();
		fShaderFile.close();

		// Converting the streams into strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Error: shader file not correctly read." << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	/* Compiling the shaders */

	unsigned int vertex, fragment;
	vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
	fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);


	/* Creating the shader program */
	int success;
	char infoLog[512];

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// Print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Error: shader program linking failed.\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	std::cout << "Shader object destroyed.";
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVector3(const std::string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}
void Shader::setMat4(const std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

unsigned int Shader::compileShader(GLenum type, const char* code)
{
	unsigned int id;
	int success;
	char infoLog[512];

	id = glCreateShader(type);
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	// Printing errors:
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Error: shader compilation failed." << type << std::endl;
	}
	return id;
}