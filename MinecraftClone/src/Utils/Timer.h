#pragma once

class Timer final
{
public:
    static void Init();

    Timer();
    ~Timer() = default;
    Timer(const Timer& rhs) = default;
    Timer& operator=(const Timer& rhs) = default;

    float GetDeltaTime();
    float GetGlobalTime(); // does not override previous counter

private:
    static float sSecondsPerCounter;
    static bool sbInitialized;

    LARGE_INTEGER mInitialCounter;
    LARGE_INTEGER mPreviousCounter;
};