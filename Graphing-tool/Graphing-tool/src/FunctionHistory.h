#pragma once

#include <vector>
#include <string>

class FunctionHistory
{
public:
	// Add a function to the history of functions
	void addFunction(std::string& function);

	// Draw the list of previous functions
	void drawHistory();

	std::string* actualFunction;

private:

	std::vector<std::string> history;
};

