#pragma once

#include "Graphics/MeshData.h"

class GeometryGenerator final
{
public:
    GeometryGenerator() = delete;
    ~GeometryGenerator() = delete;
    GeometryGenerator(const GeometryGenerator& rhs) = delete;
    GeometryGenerator& operator=(const GeometryGenerator& rhs) = delete;

public:
    static MeshData MakeSquare();
    static MeshData MakeBox();
    static MeshData MakeCylinder(const float radius, float height, int numSlices);
};