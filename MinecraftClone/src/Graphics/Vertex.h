#pragma once

enum class eBlockType : uint8_t;
enum class eFaceType : uint8_t;

struct BlockVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
    eBlockType BlockType;
    eFaceType FaceType;
    uint8_t AmbientOcclusionFactor;
};

struct WaterVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
};

struct BlockMarkerVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
};

struct CloudsVertex
{
    IntVector3D Position;
};