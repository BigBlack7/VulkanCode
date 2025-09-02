#pragma once

#pragma once
// cpp lib
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <span>
#include <memory>
#include <functional>
#include <concepts>
#include <format>
#include <chrono>
#include <numeric>
#include <numbers>

// GLM
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // opengl from -1 to 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Vulkan
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>