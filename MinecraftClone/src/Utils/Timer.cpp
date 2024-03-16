#include "Precompiled.h"

#include "Timer.h"

float Timer::sSecondsPerCounter;

void Timer::Init()
{
    LARGE_INTEGER counterPerSecond;
    QueryPerformanceFrequency(&counterPerSecond);

    sSecondsPerCounter = 1.0f / (float)counterPerSecond.QuadPart;
}

Timer::Timer()
{  
    QueryPerformanceCounter(&mInitialCounter);
    mPreviousCounter = mInitialCounter;
}

float Timer::GetDeltaTime()
{
    LARGE_INTEGER currentCounter;
    QueryPerformanceCounter(&currentCounter);

    float deltaTime = static_cast<float>(currentCounter.QuadPart - mPreviousCounter.QuadPart) * sSecondsPerCounter;
    mPreviousCounter = currentCounter;

    return deltaTime;
}

float Timer::GetGlobalTime()
{
    LARGE_INTEGER currentCounter;
    QueryPerformanceCounter(&currentCounter);

    float deltaTime = static_cast<float>(currentCounter.QuadPart - mInitialCounter.QuadPart) * sSecondsPerCounter;

    return deltaTime;
}