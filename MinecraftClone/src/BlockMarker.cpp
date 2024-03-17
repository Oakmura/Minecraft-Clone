#include "Precompiled.h"

#include "BlockMarker.h"

BlockMarker::BlockMarker()
{
    GraphicsEngine& GRM = GraphicsEngine::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(GRM.GetDevice(), L"src/Shaders/BlockMarkerVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(GRM.GetDevice(), L"src/Shaders/BlockMarkerPS.hlsl", &mPS);

    MeshData box = GeometryGenerator::MakeBox();

    D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), box.Vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), box.Indices, &mIB);
    mIndexCount = UINT(box.Indices.size());

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

    GraphicsEngine& GRM = GraphicsEngine::GetInstance();
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

    GraphicsEngine& GRM = GraphicsEngine::GetInstance();

    GRM.GetDeviceContext().IASetInputLayout(mIL);
    GRM.GetDeviceContext().VSSetShader(mVS, nullptr, 0);
    GRM.GetDeviceContext().PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mModelMatrixCB.GetGPU());
    GRM.GetDeviceContext().VSSetConstantBuffers(2, 1, &mInteractionModeCB.GetGPU());

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}