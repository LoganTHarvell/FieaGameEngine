#pragma once

// Standard
#include <cstdint>
#include <string>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <algorithm>
#include <sstream>
#include <istream>
#include <fstream>
#include <chrono>

#include <crtdbg.h>

// GSL Library
#include <gsl/span>

// OpenGL Mathematics
#include <glm/glm.hpp>

#pragma warning(disable : 4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(default : 4201)

// Microsoft Native Unit Testing
#include "CppUnitTest.h"

// Size Literal Definition
inline std::size_t operator""_z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}
