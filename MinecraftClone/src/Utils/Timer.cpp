#include "Precompiled.h"

#include "Timer.h"

float Timer::sSecondsPerCounter;
bool Timer::sbInitialized = false;

void Timer::Init()
{
    LARGE_INTEGER counterPerSecond;
    QueryPerformanceFrequency(&counterPerSecond);
    sSecondsPerCounter = 1.0f / (float)counterPerSecond.QuadPart;

    sbInitialized = true;
}

Timer::Timer()
{  
    ASSERT(sbInitialized, "Timer not initialized");

    QueryPerformanceCounter(&mInitialCounter);
    mPreviousCounter = mInitialCounter;
}

float Timer::GetDeltaTime()
{
    ASSERT(sbInitialized, "Timer not initialized");

    LARGE_INTEGER currentCounter;
    QueryPerformanceCounter(&currentCounter);

    float deltaTime = static_cast<float>(currentCounter.QuadPart - mPreviousCounter.QuadPart) * sSecondsPerCounter;
    mPreviousCounter = currentCounter;

    return deltaTime;
}

float Timer::GetGlobalTime()
{
    ASSERT(sbInitialized, "Timer not initialized");

    LARGE_INTEGER currentCounter;
    QueryPerformanceCounter(&currentCounter);

    float deltaTime = static_cast<float>(currentCounter.QuadPart - mInitialCounter.QuadPart) * sSecondsPerCounter;

    return deltaTime;
}