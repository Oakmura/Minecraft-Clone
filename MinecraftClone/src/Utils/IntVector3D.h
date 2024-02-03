#pragma once

class IntVector3D final
{
public:
    IntVector3D() = default;
    IntVector3D(int x, int y, int z) : mX(x), mY(y), mZ(z) { }

    IntVector3D operator-(const IntVector3D& InPoint) const;
    IntVector3D operator+(const IntVector3D& InPoint) const;

public:
    int mX = 0;
    int mY = 0;
    int mZ = 0;
};

