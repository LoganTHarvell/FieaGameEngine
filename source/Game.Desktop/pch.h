#pragma once

// Standard

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <streambuf>
#include <sstream>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// OpenGL
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

// GLM
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning( pop )