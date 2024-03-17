#pragma once

#include "Graphics/Vertex.h"

struct MeshData
{
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;
};