#pragma once

#include "Chunk.h"
#include "World.h"

class Player;

struct VoxelInfo
{
    eVoxelType VoxelType;
    uint32_t VoxelIndex;
    IntVector3D VoxelLocalPos;
    Chunk* Chunk;
};

enum eInteractionMode
{
    ADD,
    REMOVE,
};

class VoxelHandler final
{
public:
    static void Init(World* world);

    VoxelHandler();
    ~VoxelHandler();

    void Update(Player& player);
    void ToggleInteractionMode();
    void SetVoxel();

private:
    void addVoxel();
    void removeVoxel();

    bool rayCast(Player& player);
    bool getVoxelInfo(VoxelInfo* outVoxelInfo, IntVector3D& voxelWorldPos);

private:
    static World* mWorld;

    enum { MAX_RAY_DIST = 6 };

    VoxelInfo mFocusedVoxelInfo;
    IntVector3D mFocusedVoxelWorldPos;
    IntVector3D mFocusedVoxelNormal;

    eInteractionMode mInteractionMode;
    eVoxelType mNewVoxelType;
    bool mbOnFocus;
};

