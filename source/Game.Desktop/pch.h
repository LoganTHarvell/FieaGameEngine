#pragma once

// Standard
#include <exception>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <cstdint>
#include <iomanip>
#include <codecvt>
#include <algorithm>
#include <functional>
#include <limits>
#include <filesystem>
#include <tuple>
#include <iterator>
#include <random>
#include <cmath>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// Guidelines Support Library
#include <gsl/gsl>

// Windows
#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

// DirectX
#include <d3d11_3.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/GamePad.h>
#include <DirectXTK/Keyboard.h>
#include <DirectXTK/Mouse.h>

// OpenGL Mathematics
#pragma warning(disable : 26812)
#include <glm/glm.hpp>
#pragma warning(default : 26812)

#pragma warning(disable : 4201)
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default : 4201)