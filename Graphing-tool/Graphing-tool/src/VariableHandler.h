#pragma once

#include "AbstractShader.h"

class VariableHandler
{
public:
	// Use ImGui to draw a list of inputs for the variables
	void drawVariableList();

	// Set a function. Uses the function to enable and disable variables.
	void setFunction(std::string& function);

	// Get whether any variable has changed
	bool variableChanged();

	// Apply the variables as uniforms in the given shader
	void setVariables(AbstractShader* shader);

	float variableValues[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

private:

	std::string functions[14] = {
		"asin",
		"acos",
		"atan",
		"sin",
		"cos",
		"tan",
		"exp",
		"pow",
		"sqrt",
		"abs",
		"floor",
		"ceil",
		"min",
		"max"
	};

	// Removes all function definitions from the given string
	void removeFunctions(std::string& function);

	// All variables and whether they are included
	std::string variableNames[6] = { "a", "b", "c", "d", "e", "f" };
	float prevVariableValues[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	bool variableIncluded[6] = { false, false, false, false, false, false };
};

