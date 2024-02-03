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