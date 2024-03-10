#pragma once

class IntVector2D final
{
public:
    IntVector2D() = default;
    IntVector2D(int x, int y) : mX(x), mY(y) { }
    IntVector2D(const IntVector2D& rhs) = default;
    IntVector2D& operator=(const IntVector2D& rhs) = default;

    inline float GetAspectRatio() const { return mX / static_cast<float>(mY); } // Y축 기준으로 측정
    inline bool HasZero() const { return (mX == 0 || mY == 0); }

    IntVector2D operator-(const IntVector2D& InPoint) const;
    IntVector2D operator+(const IntVector2D& InPoint) const;

public:
    int mX = 0;
    int mY = 0;
};