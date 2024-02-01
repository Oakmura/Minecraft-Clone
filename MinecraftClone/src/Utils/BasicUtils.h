#pragma once

#ifdef _DEBUG
#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#endif

#ifdef _DEBUG
#define DX_CALL(x) \
        if (FAILED(x)) \
        { \
            LOG_ERROR("File: {0}\nLine: {1}\nFunction: {2}", __FILE__, __LINE__, __func__); \
            __debugbreak(); \
        }
#else
#define DX_CALL(x) x
#endif

#define RELEASE_COM(COM) { if(COM) { COM->Release(); COM = nullptr; } }

class BasicUtils final
{
};