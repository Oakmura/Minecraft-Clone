#pragma once

#include "World.h"
#include "Managers/GraphicsResourceManager.h"

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

enum { TREE_WIDTH = 4, TREE_HEIGHT = 8 };
enum { TREE_HALF_WIDTH = TREE_WIDTH / 2, TREE_HALF_HEIGHT = TREE_HEIGHT / 2 };

class ChunkBuilder final
{
public:
    static void Init(World* world);
    static void BuildChunk(Chunk* outChunk, const IntVector3D& pos);
    static void BuildChunkMesh(Chunk* outChunk, const IntVector3D& pos);

private:
    static bool isEmptyVoxel(const IntVector3D& localPos, const IntVector3D& worldPos);
    static int getChunkIndex(const IntVector3D& worldPos);
    static void addNewIndex(std::vector<uint32_t>& indices, uint32_t* outIndexOffset);
    static void getAmbientOcclusionFactor(const IntVector3D& localPos, const IntVector3D& worldPos, ePlane plane,
        uint8_t* outTopLeft, uint8_t* outTopRight, uint8_t* outBottomRight, uint8_t* outBottomLeft);

    static SimpleMath::Vector2 calculateTexcoord(eVoxelType voxelType, eFaceType faceType, eVertexType vertexType);
    static int generateHeight(int x, int z);
    static void generateVoxelType(Chunk& chunk, const IntVector3D& localPos, const IntVector3D& worldPos, int worldHeight);
    static void placeTree(Chunk& chunk, const IntVector3D& localPos, eVoxelType voxelType);
private:
    static World* mWorld;

    static constexpr float sTreeProbability = 0.02f;
};