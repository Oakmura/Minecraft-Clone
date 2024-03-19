#include "Precompiled.h"

#include "Graphics/GraphicsPSO.h"
#include "Core/GraphicsEngine.h"

void GraphicsPSO::SetPipelineState()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.RSSetState(mRS);
    context.OMSetBlendState(mBS, mBlendFactor, 0xffffffff);
    context.OMSetDepthStencilState(mDSS, mStencilRef);
    context.IASetPrimitiveTopology(mPrimitiveTopology);
}

void GraphicsPSO::SetBlendFactor(const float blendFactor[4])
{
    memcpy(mBlendFactor, blendFactor, sizeof(float) * 4);
}
