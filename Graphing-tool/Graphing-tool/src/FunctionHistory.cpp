#include "FunctionHistory.h"

// For removing duplicate elements
#include <algorithm>

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "imgui/imgui_stdlib.h"

void FunctionHistory::addFunction(std::string& function)
{
	// Removing any copies of the new function
	history.erase(std::remove(history.begin(), history.end(), function), history.end());

	// Adding the new function to the array
	history.push_back(function);
}

void FunctionHistory::drawHistory()
{
	// No need to draw anything if there is no history
	if (history.size() == 0)
	{
		return;
	}

	// Otherwise draw each used function in backwards order
	for (int i = history.size()-1; i >= 0; i--)
	{
		if (ImGui::Button(history[i].c_str()))
		{
			*actualFunction = history[i];
		}
	}
}
