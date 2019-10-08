#pragma once

//standard library
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <assert.h>
#include <sstream>
#include <random>

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
Scope<T> MakeScope(Args&& ... args)
{
	return Scope<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(args...);
}


#define SquidAssert assert