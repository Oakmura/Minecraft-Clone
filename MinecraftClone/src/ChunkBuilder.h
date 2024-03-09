#pragma once

#include "World.h"
#include "Managers/GraphicsResourceManager.h"

enum class ePlane
{
    X,
    Y,
    Z,
};

class ChunkBuilder
{
public:
    static void Init(World* world);
    static void BuildChunk(Chunk* outChunk, const SimpleMath::Vector3& pos);
    static void BuildChunkMesh(Chunk* outChunk, const SimpleMath::Vector3& pos);

private:
    static bool isEmptyVoxel(const IntVector3D& localPos, const IntVector3D& worldPos);
    static int getChunkIndex(const IntVector3D& worldPos);
    static void addNewIndex(std::vector<uint32_t>& indices, uint32_t* outIndexOffset);
    static void getAmbientOcclusionFactor(const IntVector3D& localPos, const IntVector3D& worldPos, ePlane plane,
        uint8_t* outTopLeft, uint8_t* outTopRight, uint8_t* outBottomRight, uint8_t* outBottomLeft);

private:
    static World* mWorld;
};