#include "ComputeShader.h"

ComputeShader::ComputeShader(const char* shaderPath)
{
	std::string shaderCode = readFile(shaderPath);
	const char* shaderCodeChars = shaderCode.c_str();


	/* Compiling the shaders */

	unsigned int shader;
	shader = compileShader(GL_COMPUTE_SHADER, shaderCodeChars);


	/* Creating the shader program */

	ID = glCreateProgram();
	glAttachShader(ID, shader);
	linkProgram(false);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(shader);
}

ComputeShader::ComputeShader(std::string& function, const char* shaderPath, bool throwError)
{
	std::string shaderCode = readFile(shaderPath);
	std::cout << replace(shaderCode, "$function", function);
	const char* shaderCodeChars = shaderCode.c_str();

	/* Compiling the shaders */

	unsigned int shader;
	shader = compileShader(GL_COMPUTE_SHADER, shaderCodeChars);


	/* Creating the shader program */

	ID = glCreateProgram();
	glAttachShader(ID, shader);
	linkProgram(throwError);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(shader);
}

ComputeShader::~ComputeShader()
{
	std::cout << "Compute shader destroyed." << std::endl;
}