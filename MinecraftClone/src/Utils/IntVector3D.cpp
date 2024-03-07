#include "Precompiled.h"

#include "IntVector3D.h"

IntVector3D IntVector3D::operator-(const IntVector3D& rhs) const
{
    return IntVector3D(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ);
}

IntVector3D IntVector3D::operator+(const IntVector3D& rhs) const
{
    return IntVector3D(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ);
}

IntVector3D IntVector3D::operator*(const IntVector3D& rhs) const
{
    return IntVector3D(mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ);
}

IntVector3D IntVector3D::operator/(const IntVector3D& rhs) const
{
    ASSERT(rhs.mX != 0 && rhs.mY != 0 && rhs.mZ != 0, "cannot divide by 0");

    return IntVector3D(mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ);
}

IntVector3D IntVector3D::operator*(const int scalar) const
{
    return IntVector3D(mX * scalar, mY * scalar, mZ * scalar);
}