#pragma once

class IntVector3D final
{
public:
    constexpr IntVector3D() = default;
    constexpr IntVector3D(int x, int y, int z) : mX(x), mY(y), mZ(z) { }
    constexpr IntVector3D(const IntVector3D& rhs) = default;
    constexpr IntVector3D& operator=(const IntVector3D& rhs) = default;

    IntVector3D operator-(const IntVector3D& rhs) const;
    IntVector3D operator+(const IntVector3D& rhs) const;
    IntVector3D operator*(const IntVector3D& rhs) const;
    IntVector3D operator/(const IntVector3D& rhs) const;

    IntVector3D operator*(const int scalar) const;

public:
    int mX = 0;
    int mY = 0;
    int mZ = 0;
};

