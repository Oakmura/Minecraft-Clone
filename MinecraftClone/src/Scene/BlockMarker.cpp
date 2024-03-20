#include "Precompiled.h"

#include "BlockMarker.h"
#include "Utils/Hasher.h"

BlockMarker::BlockMarker()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11Device& device = ge.GetDevice();

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
    GraphicsResourceLibrary& grl = ge.GetResourceLibrary();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(&grl.GetIL(Hasher::Hash("blockMarker")));
    context.VSSetShader(&grl.GetVS(Hasher::Hash("blockMarker")), nullptr, 0);
    context.PSSetShader(&grl.GetPS(Hasher::Hash("blockMarker")), nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    context.IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    context.IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    mModelMatrixCB.UseOn(eShader::Vertex, 0);
    mInteractionModeCB.UseOn(eShader::Vertex, 5);

    context.DrawIndexed(mIndexCount, 0, 0);
}