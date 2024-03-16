#pragma once

enum class eVoxelType : uint8_t;
enum class eFaceType : uint8_t;
struct VoxelVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
    eVoxelType VoxelType;
    eFaceType FaceType;
    uint8_t AmbientOcclusionFactor;
};

struct WaterVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
};

struct VoxelMarkerVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
};

struct CloudsVertex
{
    IntVector3D Position;
};