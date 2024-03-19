#include "Precompiled.h"

#include "GraphicsCommon.h"
#include "Core/GraphicsEngine.h"

void GraphicsCommon::initCommonStates()
{
    initSamplers();
    initRasterizerState();
    initBlendStates();
    initDepthStencilState();
    initPipelineStates();
}

void GraphicsCommon::destroyCommonStates()
{
    for (auto& sampler : sSampleStates)
    {
        RELEASE_COM(sampler);
    }

    RELEASE_COM(sSolidRS);
    RELEASE_COM(sWireRS);
    RELEASE_COM(sSolidBothRS);
    RELEASE_COM(sWireBothRS);

    RELEASE_COM(sDrawDSS);

    RELEASE_COM(sDefaultBS);
    RELEASE_COM(sAlphaBS);
}

void GraphicsCommon::initSamplers()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &sLinearWrapSS));

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &sPointWrapSS));

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &sLinearClampSS));

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &sPointClampSS));

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &sLinearMirrorSS));

    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &sAnisoWrapSS));

    // must match slots with "Common.hlsli"
    sSampleStates.reserve(6);
    sSampleStates.push_back(sLinearWrapSS);    // s0
    sSampleStates.push_back(sLinearClampSS);   // s1
    sSampleStates.push_back(sPointWrapSS);     // s2
    sSampleStates.push_back(sPointClampSS);    // s3
    sSampleStates.push_back(sLinearMirrorSS);  // s4
    sSampleStates.push_back(sAnisoWrapSS);     // s5
}

void GraphicsCommon::initRasterizerState()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.DepthClipEnable = true;
    // rasterDesc.MultisampleEnable = true; #TODO enable MSAA support
    DX_CALL(ge.GetDevice().CreateRasterizerState(&rasterDesc, &sSolidRS));

    rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    DX_CALL(ge.GetDevice().CreateRasterizerState(&rasterDesc, &sWireRS));

    rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    DX_CALL(ge.GetDevice().CreateRasterizerState(&rasterDesc, &sSolidBothRS));

    rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    DX_CALL(ge.GetDevice().CreateRasterizerState(&rasterDesc, &sWireBothRS));
}

void GraphicsCommon::initBlendStates()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    D3D11_BLEND_DESC defaultBlendDesc;
    ZeroMemory(&defaultBlendDesc, sizeof(defaultBlendDesc));
    defaultBlendDesc.AlphaToCoverageEnable = false; // MSAA
    defaultBlendDesc.IndependentBlendEnable = false;
    defaultBlendDesc.RenderTarget[0].BlendEnable = false;
    defaultBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    defaultBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    defaultBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    defaultBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    defaultBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    defaultBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    defaultBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    DX_CALL(ge.GetDevice().CreateBlendState(&defaultBlendDesc, &sDefaultBS));

    D3D11_BLEND_DESC alphaBlendDesc;
    ZeroMemory(&alphaBlendDesc, sizeof(alphaBlendDesc));
    alphaBlendDesc.AlphaToCoverageEnable = false; // MSAA
    alphaBlendDesc.IndependentBlendEnable = false;
    alphaBlendDesc.RenderTarget[0].BlendEnable = true;
    alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    DX_CALL(ge.GetDevice().CreateBlendState(&alphaBlendDesc, &sAlphaBS));
}

void GraphicsCommon::initDepthStencilState()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    DX_CALL(ge.GetDevice().CreateDepthStencilState(&depthStencilDesc, &sDrawDSS));
}

void GraphicsCommon::initPipelineStates()
{
    sDefaultSolidPSO.SetDepthStencilState(sDrawDSS);
    sDefaultSolidPSO.SetBlendState(sDefaultBS);
    sDefaultSolidPSO.SetRasterizerState(sSolidRS);

    sDefaultWirePSO = sDefaultSolidPSO;
    sDefaultWirePSO.SetRasterizerState(sWireRS);

    sBothSolidPSO = sDefaultSolidPSO;
    sBothSolidPSO.SetRasterizerState(sSolidBothRS);

    sBothWirePSO = sDefaultSolidPSO;
    sBothWirePSO.SetRasterizerState(sWireBothRS);

    sDefaultSolidAlphaPSO = sDefaultSolidPSO;
    sDefaultSolidAlphaPSO.SetBlendState(sAlphaBS);

    sDefaultWireAlphaPSO = sDefaultSolidAlphaPSO;
    sDefaultWireAlphaPSO.SetRasterizerState(sWireRS);

    sBothSolidAlphaPSO = sDefaultSolidAlphaPSO;
    sBothSolidAlphaPSO.SetRasterizerState(sSolidBothRS);

    sBothWireAlphaPSO = sDefaultSolidAlphaPSO;
    sBothWireAlphaPSO.SetRasterizerState(sWireBothRS);
}

ID3D11SamplerState* GraphicsCommon::sLinearWrapSS;
ID3D11SamplerState* GraphicsCommon::sLinearClampSS;
ID3D11SamplerState* GraphicsCommon::sPointWrapSS;
ID3D11SamplerState* GraphicsCommon::sPointClampSS;
ID3D11SamplerState* GraphicsCommon::sLinearMirrorSS;
ID3D11SamplerState* GraphicsCommon::sAnisoWrapSS;
std::vector<ID3D11SamplerState*> GraphicsCommon::sSampleStates;

ID3D11RasterizerState* GraphicsCommon::sSolidRS;
ID3D11RasterizerState* GraphicsCommon::sWireRS;
ID3D11RasterizerState* GraphicsCommon::sSolidBothRS;
ID3D11RasterizerState* GraphicsCommon::sWireBothRS;

ID3D11DepthStencilState* GraphicsCommon::sDrawDSS;

ID3D11BlendState* GraphicsCommon::sDefaultBS;
ID3D11BlendState* GraphicsCommon::sAlphaBS;

GraphicsPSO GraphicsCommon::sDefaultSolidPSO;
GraphicsPSO GraphicsCommon::sDefaultWirePSO;
GraphicsPSO GraphicsCommon::sBothSolidPSO;
GraphicsPSO GraphicsCommon::sBothWirePSO;
GraphicsPSO GraphicsCommon::sDefaultSolidAlphaPSO;
GraphicsPSO GraphicsCommon::sDefaultWireAlphaPSO;
GraphicsPSO GraphicsCommon::sBothSolidAlphaPSO;
GraphicsPSO GraphicsCommon::sBothWireAlphaPSO;