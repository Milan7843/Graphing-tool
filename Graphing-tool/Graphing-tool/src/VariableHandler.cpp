#include "VariableHandler.h"

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "imgui/imgui_stdlib.h"


void VariableHandler::drawVariableList()
{
	// Saving prvious values to other array for comparing to detect change
	for (unsigned int i = 0; i < 6; i++)
	{
		prevVariableValues[i] = variableValues[i];
	}

	for (unsigned int i = 0; i < 6; i++)
	{
		// Check if the variable is included in the current graph
		if (variableIncluded[i])
		{
			// Draw a dragfloat area for the included variable
			ImGui::DragFloat(variableNames[i].c_str(), &(variableValues[i]), 0.001f);
		}
	}
}

void VariableHandler::setFunction(std::string& function)
{
	// Removing all function as they may contain variable letters
	std::string functionCopy(function);
	removeFunctions(functionCopy);

	// Found it? Then it must be included
	for (unsigned int i = 0; i < 6; i++)
	{
		// Check and set whether the variable is included in the current graph
		variableIncluded[i] = (functionCopy.find(variableNames[i].c_str()) != std::string::npos);
	}
}

bool VariableHandler::variableChanged()
{
	for (unsigned int i = 0; i < 6; i++)
	{
		// If any changed, return true
		if (prevVariableValues[i] != variableValues[i])
		{
			return true;
		}
	}
	// If none changed, return false
	return false;
}

void VariableHandler::setVariables(AbstractShader* shader)
{
	// Activate shader program before updating uniforms
	shader->use();

	for (unsigned int i = 0; i < 6; i++)
	{
		// Set each variable in the shader
		shader->setFloat(variableNames[i].c_str(), variableValues[i]);
	}
}

void VariableHandler::removeFunctions(std::string& function)
{
	// Removing all function definitions
	for (unsigned int i = 0; i < 14; i++)
	{
		size_t start_pos = function.find(functions[i]);
		while (start_pos != std::string::npos)
		{
			function.replace(start_pos, functions[i].length(), "");
			start_pos = function.find(functions[i]);
		}
	}
}
