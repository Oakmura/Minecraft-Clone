#include "Precompiled.h"

#include "BasicUtils.h"

int BasicUtils::GetSign(const float val)
{
    return (0.0f < val) - (val < 0.0f);
}