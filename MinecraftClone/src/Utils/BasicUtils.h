#pragma once

#ifdef _DEBUG
#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#endif

class BasicUtils final
{
public:
    static int GetSign(const float val);
};