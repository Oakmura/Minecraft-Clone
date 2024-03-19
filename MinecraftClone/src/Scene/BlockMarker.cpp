#include "Precompiled.h"

#include "BlockMarker.h"

BlockMarker::BlockMarker()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11Device& device = ge.GetDevice();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(device, L"src/Shaders/BlockMarkerVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(device, L"src/Shaders/BlockMarkerPS.hlsl", &mPS);

    MeshData box = GeometryGenerator::MakeBox();

    D3D11Utils::CreateVertexBuffer(device, box.Vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(device, box.Indices, &mIB);
    mIndexCount = UINT(box.Indices.size());

    mModelMatrixCB.GetCPU().Model = SimpleMath::Matrix().Transpose();
    mInteractionModeCB.GetCPU().InteractionMode = eInteractionMode::Add;
    D3D11Utils::CreateConstantBuffer(device, mModelMatrixCB.GetCPU(), &mModelMatrixCB.GetGPU());
    D3D11Utils::CreateConstantBuffer(device, mInteractionModeCB.GetCPU(), &mInteractionModeCB.GetGPU());
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

    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    if (mInteractionModeCB.GetCPU().InteractionMode != blockHandler.GetInteractionMode())
    {
        mInteractionModeCB.GetCPU().InteractionMode = blockHandler.GetInteractionMode();
        D3D11Utils::UpdateBuffer(ge.GetDeviceContext(), mInteractionModeCB.GetCPU(), mInteractionModeCB.GetGPU());
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
    D3D11Utils::UpdateBuffer(ge.GetDeviceContext(), mModelMatrixCB.GetCPU(), mModelMatrixCB.GetGPU());
}

void BlockMarker::Render(const BlockHandler& blockHandler)
{
    if (blockHandler.GetBlockType() == eBlockType::Empty)
    {
        return;
    }

    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(mIL);
    context.VSSetShader(mVS, nullptr, 0);
    context.PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    context.IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    context.IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    mModelMatrixCB.UseOn(eShader::Vertex, 0);
    mInteractionModeCB.UseOn(eShader::Vertex, 5);

    context.DrawIndexed(mIndexCount, 0, 0);
}