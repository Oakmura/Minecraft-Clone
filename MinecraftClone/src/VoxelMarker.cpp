#include "Precompiled.h"

#include "VoxelMarker.h"

VoxelMarker::VoxelMarker()
    : mIL(nullptr)
    , mVS(nullptr)
    , mPS(nullptr)
    , mVB(nullptr)
    , mIB(nullptr)
    , mModelCPU(SimpleMath::Matrix().Transpose())
    , mModelGPU(nullptr)
    , mInteractionModeGPU(nullptr)
    , mIndexCount(0)
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(GRM.GetDevice(), L"src/Shaders/VoxelMarkerVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(GRM.GetDevice(), L"src/Shaders/VoxelMarkerPS.hlsl", &mPS);

    // cubemesh Á¤ÀÇ
    std::vector<IntVector3D> positions;
    std::vector<SimpleMath::Vector2> texcoords;

    const static int scale = 1;

    // À­¸é
    positions.push_back(IntVector3D(0, 1, 0) * scale);
    positions.push_back(IntVector3D(0, 1, 1) * scale);
    positions.push_back(IntVector3D(1, 1, 1) * scale);
    positions.push_back(IntVector3D(1, 1, 0) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¾Æ·§¸é
    positions.push_back(IntVector3D(0, 0, 0) * scale);
    positions.push_back(IntVector3D(1, 0, 0) * scale);
    positions.push_back(IntVector3D(1, 0, 1) * scale);
    positions.push_back(IntVector3D(0, 0, 1) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¾Õ¸é
    positions.push_back(IntVector3D(0, 0, 0) * scale);
    positions.push_back(IntVector3D(0, 1, 0) * scale);
    positions.push_back(IntVector3D(1, 1, 0) * scale);
    positions.push_back(IntVector3D(1, 0, 0) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // µÞ¸é
    positions.push_back(IntVector3D(0, 0, 1) * scale);
    positions.push_back(IntVector3D(1, 0, 1) * scale);
    positions.push_back(IntVector3D(1, 1, 1) * scale);
    positions.push_back(IntVector3D(0, 1, 1) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¿ÞÂÊ
    positions.push_back(IntVector3D(0, 0, 1) * scale);
    positions.push_back(IntVector3D(0, 1, 1) * scale);
    positions.push_back(IntVector3D(0, 1, 0) * scale);
    positions.push_back(IntVector3D(0, 0, 0) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ¿À¸¥ÂÊ
    positions.push_back(IntVector3D(1, 0, 1) * scale);
    positions.push_back(IntVector3D(1, 0, 0) * scale);
    positions.push_back(IntVector3D(1, 1, 0) * scale);
    positions.push_back(IntVector3D(1, 1, 1) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    std::vector<VoxelMarkerVertex> vertices;
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        VoxelMarkerVertex v;
        v.Position = positions[i];
        v.Texcoord = texcoords[i];
        vertices.push_back(v);
    }

    std::vector<uint32_t> indices = 
    {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23,  // ¿À¸¥ÂÊ
    };

    D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), indices, &mIB);

    mIndexCount = UINT(indices.size());

    mInteractionModeCPU.InteractionMode = eInteractionMode::Add;
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mModelCPU, &mModelGPU);
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mInteractionModeCPU, &mInteractionModeGPU);
}

VoxelMarker::~VoxelMarker()
{
    RELEASE_COM(mIL);
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);

    RELEASE_COM(mModelGPU);
    RELEASE_COM(mInteractionModeGPU);
}

void VoxelMarker::Update(const VoxelHandler& voxelHandler)
{
    if (voxelHandler.GetVoxelType() == eVoxelType::Empty)
    {
        return;
    }
    
    LOG_TRACE("Found voxel");

    mInteractionModeCPU.InteractionMode = voxelHandler.GetInteractionMode();
    IntVector3D posInt;
    if (mInteractionModeCPU.InteractionMode == eInteractionMode::Add)
    {
        posInt = voxelHandler.GetFocusedVoxelWorldPos() + voxelHandler.GetFocusedVoxelNormal();
    }
    else
    {
        posInt = voxelHandler.GetFocusedVoxelWorldPos();
    }

    SimpleMath::Vector3 posFloat = { static_cast<float>(posInt.mX), static_cast<float>(posInt.mY), static_cast<float>(posInt.mZ) };
    mModelCPU = SimpleMath::Matrix::CreateTranslation(posFloat).Transpose();
}

void VoxelMarker::Render(const VoxelHandler& voxelHandler)
{
    if (voxelHandler.GetVoxelType() == eVoxelType::Empty)
    {
        return;
    }

    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    D3D11Utils::UpdateBuffer(GRM.GetDeviceContext(), mModelCPU, mModelGPU);
    D3D11Utils::UpdateBuffer(GRM.GetDeviceContext(), mInteractionModeCPU, mInteractionModeGPU);

    GRM.GetDeviceContext().IASetInputLayout(mIL);
    GRM.GetDeviceContext().VSSetShader(mVS, nullptr, 0);
    GRM.GetDeviceContext().PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(VoxelMarkerVertex);

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mModelGPU);
    GRM.GetDeviceContext().VSSetConstantBuffers(2, 1, &mInteractionModeGPU);

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}