#pragma once

#include "Settings.h"
#include "Structs/Vertex.h"
#include "Graphics/ConstantBuffer.h"
#include "Core/GraphicsEngine.h"
#include "Scene/World.h"

class Clouds final
{
public:
    Clouds();
    ~Clouds();
    Clouds(const Clouds& rhs) = delete;
    Clouds& operator=(const Clouds& rhs) = delete;

    void Update(const float dt);
    void Render();

private:
    void buildVertices(const std::vector<uint8_t>& cloudData);

private:
    std::vector<SimpleMath::Vector3> mVertices;
    std::vector<uint32_t> mIndices;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ConstantBuffer<CloudsCB> mCloudsCB;
};