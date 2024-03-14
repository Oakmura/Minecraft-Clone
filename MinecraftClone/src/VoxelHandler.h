#pragma once

#include "Chunk.h"
#include "World.h"

class Player;

struct VoxelInfo
{
    eVoxelType VoxelType = eVoxelType::Empty;
    uint32_t VoxelIndex;
    IntVector3D VoxelLocalPos;
    Chunk* Chunk;
};

enum eInteractionMode
{
    Remove,
    Add,
};

class VoxelHandler final
{
    friend class ImGuiUI;
public:
    static void Init(World* world);

    VoxelHandler() = default;
    ~VoxelHandler() = default;
    VoxelHandler(const VoxelHandler& rhs) = delete;
    VoxelHandler& operator=(const VoxelHandler& rhs) = delete;

    void Update(Player& player);
    void SwitchInteractionMode();
    void SetVoxel();

    const IntVector3D& GetFocusedVoxelWorldPos() const { return mFocusedVoxelWorldPos; }
    const IntVector3D& GetFocusedVoxelNormal() const { return mFocusedVoxelNormal; }
    const eInteractionMode& GetInteractionMode() const { return mInteractionMode; }
    const eVoxelType& GetVoxelType() const { return mFocusedVoxelInfo.VoxelType; }

private:
    int getSign(const float val);

    void addVoxel();
    void removeVoxel();
    void rebuildAdjacentChunk(const IntVector3D& adjacentChunkPosWorld);
    void rebuildAdjacentChunks();

    bool rayCast(Player& player);
    bool getVoxelInfo(VoxelInfo* outVoxelInfo, const IntVector3D& voxelWorldPos);

private:
    static World* sWorld;

    enum { MAX_RAY_DIST = 6 };

    VoxelInfo mFocusedVoxelInfo;
    IntVector3D mFocusedVoxelWorldPos;
    IntVector3D mFocusedVoxelNormal;

    eInteractionMode mInteractionMode = eInteractionMode::Add;
    eVoxelType mNewVoxelType = eVoxelType::Dirt;
};

