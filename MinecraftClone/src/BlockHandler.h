#pragma once

#include "Chunk.h"
#include "Utils/ChunkUtils.h"

class Player;

enum eInteractionMode
{
    Remove,
    Add,
};

class BlockHandler final
{
    friend class ImGuiUI;
public:
    static void Init(World* world);

    BlockHandler() = default;
    ~BlockHandler() = default;
    BlockHandler(const BlockHandler& rhs) = delete;
    BlockHandler& operator=(const BlockHandler& rhs) = delete;

    void Update(Player& player);
    void SwitchInteractionMode();
    void SetBlock();

    const IntVector3D& GetFocusedBlockWorldPos() const { return mFocusedBlockWorldPos; }
    const IntVector3D& GetFocusedBlockNormal() const { return mFocusedBlockNormal; }
    const eInteractionMode& GetInteractionMode() const { return mInteractionMode; }
    const eBlockType& GetBlockType() const { return mFocusedBlockInfo.BlockType; }

private:
    void addBlock();
    void removeBlock();
    void rebuildAdjacentChunk(const IntVector3D& adjacentChunkPosWorld);
    void rebuildAdjacentChunks();

    bool rayCast(Player& player);

private:
    static World* sWorld;

    enum { MAX_RAY_DIST = 6 };

    BlockInfo mFocusedBlockInfo;
    IntVector3D mFocusedBlockWorldPos;
    IntVector3D mFocusedBlockNormal;

    eInteractionMode mInteractionMode = eInteractionMode::Add;
    eBlockType mNewBlockType = eBlockType::Dirt;
};

