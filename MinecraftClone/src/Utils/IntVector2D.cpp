#include "Precompiled.h"

#include "IntVector2D.h"

IntVector2D IntVector2D::operator-(const IntVector2D& rhs) const
{
    return IntVector2D(mX - rhs.mX, mY - rhs.mY);
}

IntVector2D IntVector2D::operator+(const IntVector2D& rhs) const
{
    return IntVector2D(mX + rhs.mX, mY + rhs.mY);
}