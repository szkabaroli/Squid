#pragma once

//standard library
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <set>
#include <functional>

//library specific
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

//export macro
#define SQUID_API extern "C" _declspec(dllexport)