#pragma once

#include "Chunk.h"
#include <Managers/GraphicsResourceManager.h>

class ChunkBuilder
{
public:
    static void BuildChunk(GraphicsResourceManager& GRM, Chunk* outChunk, const Vector3& pos);

private:
    static bool isEmptyVoxel(std::vector<eVoxelType>& voxelTypes, const int x, const int y, const int z);
    static void addNewIndex(std::vector<uint32_t>& indices, uint32_t* outIndexOffset);
};