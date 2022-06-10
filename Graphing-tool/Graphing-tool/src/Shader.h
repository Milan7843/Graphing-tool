#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "AbstractShader.h"

class Shader : public AbstractShader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(std::string& function, const char* vertexPath, const char* fragmentPath, bool throwError);
	~Shader();
};

#endif