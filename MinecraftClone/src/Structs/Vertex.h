#pragma once

enum class eBlockType : uint8_t;
enum class eFaceType : uint8_t;

struct Vertex
{
    SimpleMath::Vector3 Position;
    SimpleMath::Vector2 Texcoord;
};

struct BlockVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
    eBlockType BlockType;
    eFaceType FaceType;
    uint8_t AmbientOcclusionFactor;
};

struct CloudsVertex
{
    IntVector3D Position;
};