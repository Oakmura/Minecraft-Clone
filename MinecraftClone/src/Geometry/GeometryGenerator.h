#pragma once

#include "Graphics/MeshData.h"

class GeometryGenerator final
{
public:
    static MeshData MakeSquare();
    static MeshData MakeBox();
    static MeshData MakeCylinder(const float radius, float height, int numSlices);
};