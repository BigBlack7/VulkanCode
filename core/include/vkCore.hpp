#pragma once
// EasyVulkanStart
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

template <typename T>
class ArrayRef
{
    T *const mPArray = nullptr;
    size_t mCount = 0;

public:
    // construct from null para, count=0
    ArrayRef() = default;
    // construct from single object, count=1
    ArrayRef(T &data) : mPArray(&data), mCount(1) {}
    // construct from top level array
    template <size_t ElementCount>
    ArrayRef(T (&data)[ElementCount]) : mPArray(data), mCount(ElementCount) {}
    // construct from pointer and element count
    ArrayRef(T *pData, size_t elementCount) : mPArray(pData), mCount(elementCount) {}
    // construct from const arrayRef, if T is const
    ArrayRef(const ArrayRef<std::remove_const_t<T>> &other) : mPArray(other.Pointer()), mCount(other.Count()) {}
    // Getter
    T *Pointer() const { return mPArray; }
    size_t Count() const { return mCount; }
    // Const Function
    T &operator[](size_t index) const { return mPArray[index]; }
    T *begin() const { return mPArray; }
    T *end() const { return mPArray + mCount; }
    // Non-const Function
    // disable copy/move assignment (ArrayRef is intended to simulate "reference to array", its purpose is just passing parameter, so make it same as C++ reference's underlying address, to prevent being modified after initialization)
    ArrayRef &operator=(const ArrayRef &) = delete;
};

#define ExecuteOnce(...)              \
    {                                 \
        static bool executed = false; \
        if (executed)                 \
            return __VA_ARGS__;       \
        executed = true;              \
    }