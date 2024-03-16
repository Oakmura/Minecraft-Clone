#include "Precompiled.h"

#include "BlockMarker.h"

BlockMarker::BlockMarker()
    : mIL(nullptr)
    , mVS(nullptr)
    , mPS(nullptr)
    , mVB(nullptr)
    , mIB(nullptr)
    , mIndexCount(0)
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(GRM.GetDevice(), L"src/Shaders/BlockMarkerVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(GRM.GetDevice(), L"src/Shaders/BlockMarkerPS.hlsl", &mPS);

    // cubemesh ����
    std::vector<IntVector3D> positions;
    std::vector<SimpleMath::Vector2> texcoords;

    const static int scale = 1;

    // ����
    positions.push_back(IntVector3D(0, 1, 0) * scale);
    positions.push_back(IntVector3D(0, 1, 1) * scale);
    positions.push_back(IntVector3D(1, 1, 1) * scale);
    positions.push_back(IntVector3D(1, 1, 0) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // �Ʒ���
    positions.push_back(IntVector3D(0, 0, 0) * scale);
    positions.push_back(IntVector3D(1, 0, 0) * scale);
    positions.push_back(IntVector3D(1, 0, 1) * scale);
    positions.push_back(IntVector3D(0, 0, 1) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // �ո�
    positions.push_back(IntVector3D(0, 0, 0) * scale);
    positions.push_back(IntVector3D(0, 1, 0) * scale);
    positions.push_back(IntVector3D(1, 1, 0) * scale);
    positions.push_back(IntVector3D(1, 0, 0) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // �޸�
    positions.push_back(IntVector3D(0, 0, 1) * scale);
    positions.push_back(IntVector3D(1, 0, 1) * scale);
    positions.push_back(IntVector3D(1, 1, 1) * scale);
    positions.push_back(IntVector3D(0, 1, 1) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ����
    positions.push_back(IntVector3D(0, 0, 1) * scale);
    positions.push_back(IntVector3D(0, 1, 1) * scale);
    positions.push_back(IntVector3D(0, 1, 0) * scale);
    positions.push_back(IntVector3D(0, 0, 0) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    // ������
    positions.push_back(IntVector3D(1, 0, 1) * scale);
    positions.push_back(IntVector3D(1, 0, 0) * scale);
    positions.push_back(IntVector3D(1, 1, 0) * scale);
    positions.push_back(IntVector3D(1, 1, 1) * scale);
    texcoords.push_back(SimpleMath::Vector2(0.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 0.0f));
    texcoords.push_back(SimpleMath::Vector2(1.0f, 1.0f));
    texcoords.push_back(SimpleMath::Vector2(0.0f, 1.0f));

    std::vector<BlockMarkerVertex> vertices;
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        BlockMarkerVertex v;
        v.Position = positions[i];
        v.Texcoord = texcoords[i];
        vertices.push_back(v);
    }

    std::vector<uint32_t> indices = 
    {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23,  // ������
    };

    D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), indices, &mIB);
    mIndexCount = UINT(indices.size());

    mModelMatrixCB.GetCPU().Model = SimpleMath::Matrix().Transpose();
    mInteractionModeCB.GetCPU().InteractionMode = eInteractionMode::Add;
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mModelMatrixCB.GetCPU(), &mModelMatrixCB.GetGPU());
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mInteractionModeCB.GetCPU(), &mInteractionModeCB.GetGPU());
}

BlockMarker::~BlockMarker()
{
    RELEASE_COM(mIL);
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
}

void BlockMarker::Update(const BlockHandler& blockHandler)
{
    if (blockHandler.GetBlockType() == eBlockType::Empty)
    {
        return;
    }

    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();
    if (mInteractionModeCB.GetCPU().InteractionMode != blockHandler.GetInteractionMode())
    {
        mInteractionModeCB.GetCPU().InteractionMode = blockHandler.GetInteractionMode();
        D3D11Utils::UpdateBuffer(GRM.GetDeviceContext(), mInteractionModeCB.GetCPU(), mInteractionModeCB.GetGPU());
    }
    
    IntVector3D posInt;
    if (mInteractionModeCB.GetCPU().InteractionMode == eInteractionMode::Add)
    {
        posInt = blockHandler.GetFocusedBlockWorldPos() + blockHandler.GetFocusedBlockNormal();
    }
    else
    {
        posInt = blockHandler.GetFocusedBlockWorldPos();
    }

    SimpleMath::Vector3 posFloat = { static_cast<float>(posInt.mX), static_cast<float>(posInt.mY), static_cast<float>(posInt.mZ) };
    mModelMatrixCB.GetCPU().Model = SimpleMath::Matrix::CreateTranslation(posFloat).Transpose();
    D3D11Utils::UpdateBuffer(GRM.GetDeviceContext(), mModelMatrixCB.GetCPU(), mModelMatrixCB.GetGPU());
}

void BlockMarker::Render(const BlockHandler& blockHandler)
{
    if (blockHandler.GetBlockType() == eBlockType::Empty)
    {
        return;
    }

    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    GRM.GetDeviceContext().IASetInputLayout(mIL);
    GRM.GetDeviceContext().VSSetShader(mVS, nullptr, 0);
    GRM.GetDeviceContext().PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(BlockMarkerVertex);

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mModelMatrixCB.GetGPU());
    GRM.GetDeviceContext().VSSetConstantBuffers(2, 1, &mInteractionModeCB.GetGPU());

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}