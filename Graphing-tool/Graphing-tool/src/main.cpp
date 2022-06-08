#pragma once

#include "Application.h"

/* CONTROLS */
/*
 - W to move forward
 - S to move backward
 - A to move left
 - D to move right
 - E to move up
 - Q to move down
 - F to enable/disable settings and info menu
 - Mouse to look around
*/

// Window size
const int WIDTH = 1200, HEIGHT = 900;

// The input function, can use variables x, z and regular glsl functions
std::string function(
	"sin(x*z)/1.4 + cos((x+z)/2)"
);


// The sensitivity of the graph rescaling
const float scrollSensitivity = 1.0f;

int main()
{
	Application application(WIDTH, HEIGHT, function, scrollSensitivity);
	return application.Start();
}