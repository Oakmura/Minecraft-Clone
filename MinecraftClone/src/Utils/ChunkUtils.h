#pragma once

#include "Settings.h"

class ChunkUtils final
{
public:
    ChunkUtils() = delete;
    ~ChunkUtils() = delete;
    ChunkUtils(const ChunkUtils& rhs) = delete;
    ChunkUtils operator=(const ChunkUtils& rhs) = delete;

    static int GetVoxelIndex(const IntVector3D& localPos);
    static int GetChunkIndexLocal(const IntVector3D& localPos);
    static int GetChunkIndexWorld(const IntVector3D& worldPos);

    static bool ChunkUtils::GetChunkXYZ(IntVector3D* outPos, const IntVector3D& worldPos);
};