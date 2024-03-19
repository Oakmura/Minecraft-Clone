#include "Precompiled.h"

#include "GeometryGenerator.h"

MeshData GeometryGenerator::MakeSquare()
{
    constexpr uint32_t squareVertexCount = 4;
    constexpr uint32_t squareIndexCount = 6;

    std::vector<SimpleMath::Vector3> positions;
    positions.reserve(squareVertexCount);
    positions.push_back({ 0.0f, 0.0f, 0.0f });
    positions.push_back({ 0.0f, 0.0f, 1.0f });
    positions.push_back({ 1.0f, 0.0f, 1.0f });
    positions.push_back({ 1.0f, 0.0f, 0.0f });

    std::vector<SimpleMath::Vector2> texcoords;
    texcoords.reserve(squareVertexCount);
    texcoords.push_back({ 0.0f, 0.0f });
    texcoords.push_back({ 0.0f, 1.0f });
    texcoords.push_back({ 1.0f, 1.0f });
    texcoords.push_back({ 1.0f, 0.0f });

    MeshData meshData;
    meshData.Vertices.reserve(squareVertexCount);
    meshData.Indices.reserve(squareIndexCount);

    for (size_t i = 0; i < positions.size(); ++i)
    {
        Vertex v;
        v.Position = positions[i];
        v.Texcoord = texcoords[i];
        meshData.Vertices.push_back(v);
    }

    meshData.Indices =
    {
        0, 1, 2,
        1, 2, 3,
    };

    return meshData;
}

MeshData GeometryGenerator::MakeBox()
{
    constexpr uint32_t boxVertexCount = 4 * 6;
    constexpr uint32_t boxIndexCount = 6 * 6;

    std::vector<SimpleMath::Vector3> positions;
    std::vector<SimpleMath::Vector2> texcoords;
    positions.reserve(boxVertexCount);
    texcoords.reserve(boxVertexCount);

    // À­¸é
    positions.push_back(SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 1.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¾Æ·§¸é
    positions.push_back(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 0.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 0.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¾Õ¸é
    positions.push_back(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 1.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // µÞ¸é
    positions.push_back(SimpleMath::Vector3(0.0f, 0.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 0.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¿ÞÂÊ
    positions.push_back(SimpleMath::Vector3(0.0f, 0.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 1.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¿À¸¥ÂÊ
    positions.push_back(SimpleMath::Vector3(1.0f, 0.0f, 1.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 0.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 1.0f, 0.0f));
    positions.push_back(SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    MeshData meshData;
    meshData.Vertices.reserve(boxVertexCount);
    meshData.Indices.reserve(boxIndexCount);

    for (size_t i = 0; i < positions.size(); ++i)
    {
        Vertex v;
        v.Position = positions[i];
        v.Texcoord = texcoords[i];
        meshData.Vertices.push_back(v);
    }

    meshData.Indices =
    {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23,  // ¿À¸¥ÂÊ
    };

    return meshData;
}

MeshData GeometryGenerator::MakeCylinder(const float radius, float height, int numSlices)
{
    const float dTheta = -XM_2PI / static_cast<float>(numSlices);
    const int cylinderVertexCount = numSlices * 2 + 2;
    const int cylinderIndexCount = numSlices * 6;

    MeshData meshData;

    std::vector<Vertex>& vertices = meshData.Vertices;
    vertices.reserve(cylinderVertexCount);
    for (int i = 0; i <= numSlices; ++i) // bottom
    {
        Vertex v;
        v.Position = SimpleMath::Vector3::Transform(SimpleMath::Vector3(radius, -0.5f * height, 0.0f), SimpleMath::Matrix::CreateRotationY(dTheta * float(i)));
        v.Texcoord = SimpleMath::Vector2(float(i) / numSlices, 1.0f);

        vertices.push_back(v);
    }

    for (int i = 0; i <= numSlices; ++i) // top
    {
        Vertex v;
        v.Position = SimpleMath::Vector3::Transform(SimpleMath::Vector3(radius, 0.5f * height, 0.0f), SimpleMath::Matrix::CreateRotationY(dTheta * float(i)));
        v.Texcoord = SimpleMath::Vector2(float(i) / numSlices, 0.0f);

        vertices.push_back(v);
    }

    std::vector<uint32_t>& indices = meshData.Indices;
    indices.reserve(cylinderIndexCount);
    for (int i = 0; i < numSlices; ++i) 
    {
        indices.push_back(i);
        indices.push_back(i + numSlices + 1);
        indices.push_back(i + 1 + numSlices + 1);

        indices.push_back(i);
        indices.push_back(i + 1 + numSlices + 1);
        indices.push_back(i + 1);
    }

    return meshData;
}
