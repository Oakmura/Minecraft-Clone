#pragma once

#include "Scene/World.h"
#include "Utils/ChunkUtils.h"
#include "Core/GraphicsEngine.h"

enum class ePlane
{
    X,
    Y,
    Z,
};

enum eTerrainLevel
{
    Sand = 7,
    Grass = 8,
    Dirt = 40,
    Stone = 49,
    Snow = 54,
};

class ChunkGenerator final
{
public:
    ChunkGenerator() = delete;
    ~ChunkGenerator() = delete;
    ChunkGenerator(const ChunkGenerator& rhs) = delete;
    ChunkGenerator& operator=(const ChunkGenerator& rhs) = delete;

public:
    static void Init(World* world);
    static void BuildChunk(Chunk* outChunk, const IntVector3D& pos);
    static void BuildChunkMesh(Chunk* outChunk, const IntVector3D& pos);

private:
    static bool isEmptyBlock(const IntVector3D& localPos, const IntVector3D& worldPos);
    static void addNewIndex(std::vector<uint32_t>& indices, uint32_t* outIndexOffset);
    static void getAmbientOcclusionFactor(const IntVector3D& localPos, const IntVector3D& worldPos, ePlane plane,
        uint8_t* outTopLeft, uint8_t* outTopRight, uint8_t* outBottomRight, uint8_t* outBottomLeft);

    static SimpleMath::Vector2 calculateTexcoord(eBlockType blockType, eFaceType faceType, eVertexType vertexType);
    static int generateHeight(int x, int z);
    static void generateBlockType(Chunk& chunk, const IntVector3D& localPos, const IntVector3D& worldPos, int worldHeight);
    static void placeTree(Chunk& chunk, const IntVector3D& localPos, eBlockType blockType);
private:
    static World* mWorld;
};