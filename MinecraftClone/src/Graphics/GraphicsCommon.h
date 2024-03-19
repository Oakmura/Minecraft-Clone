#pragma once

#include "Graphics/GraphicsPSO.h"

class GraphicsCommon final
{
    friend class GraphicsEngine;

public:
    GraphicsCommon() = delete;
    ~GraphicsCommon() = delete;
    GraphicsCommon(const GraphicsCommon& rhs) = delete;
    GraphicsCommon& operator=(const GraphicsCommon& rhs) = delete;

private:
    static void initCommonStates();
    static void destroyCommonStates();

    static void initSamplers();
    static void initRasterizerState();
    static void initBlendStates();
    static void initDepthStencilState();
    static void initPipelineStates();

private:
    // SS
    static ID3D11SamplerState* sLinearWrapSS;
    static ID3D11SamplerState* sLinearClampSS;
    static ID3D11SamplerState* sPointWrapSS;
    static ID3D11SamplerState* sPointClampSS;
    static ID3D11SamplerState* sLinearMirrorSS;
    static ID3D11SamplerState* sAnisoWrapSS;
    static std::vector<ID3D11SamplerState*> sSampleStates;

    // RS
    static ID3D11RasterizerState* sSolidRS; // front only
    static ID3D11RasterizerState* sWireRS;
    static ID3D11RasterizerState* sSolidBothRS; // front and back
    static ID3D11RasterizerState* sWireBothRS;

    // DSS
    static ID3D11DepthStencilState* sDrawDSS; // default 

    // BS
    static ID3D11BlendState* sDefaultBS;
    static ID3D11BlendState* sAlphaBS;

    // PSO
    static GraphicsPSO sDefaultSolidPSO;
    static GraphicsPSO sDefaultWirePSO;
    static GraphicsPSO sBothSolidPSO;
    static GraphicsPSO sBothWirePSO;
    static GraphicsPSO sDefaultSolidAlphaPSO;
    static GraphicsPSO sDefaultWireAlphaPSO;
    static GraphicsPSO sBothSolidAlphaPSO;
    static GraphicsPSO sBothWireAlphaPSO;
};

