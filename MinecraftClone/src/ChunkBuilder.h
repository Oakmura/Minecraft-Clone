#pragma once

#include "World.h"
#include <Managers/GraphicsResourceManager.h>

enum class ePlane
{
    X,
    Y,
    Z,
};

class ChunkBuilder
{
public:
    static void BuildChunk(Chunk* outChunk, const Vector3& pos);
    static void BuildChunkMesh(GraphicsResourceManager& GRM, World& world, Chunk* outChunk, const Vector3& pos);

private:
    static bool isEmptyVoxel(World& world, const IntVector3D& localPos, const IntVector3D& worldPos);
    static int getChunkIndex(const IntVector3D& worldPos);
    static void addNewIndex(std::vector<uint32_t>& indices, uint32_t* outIndexOffset);
    static void getAmbientOcclusionFactor(World& world, const IntVector3D& localPos, const IntVector3D& worldPos, ePlane plane,
        uint8_t* outTopLeft, uint8_t* outTopRight, uint8_t* outBottomRight, uint8_t* outBottomLeft);
};