#pragma once

class GraphicsPSO final
{
public:
    GraphicsPSO() = default;
    ~GraphicsPSO() = default;
    GraphicsPSO(const GraphicsPSO& rhs) = default;
    GraphicsPSO& operator=(const GraphicsPSO& rhs) = default;

    void SetPipelineState();

    inline void SetBlendState(ID3D11BlendState* bs) { mBS = bs; }
    inline void SetRasterizerState(ID3D11RasterizerState* rs) { mRS = rs; }
    inline void SetDepthStencilState(ID3D11DepthStencilState* dss) { mDSS = dss; }

    inline void SetStencilRef(UINT stencilRef) { mStencilRef = stencilRef; }
    inline void SetBlendFactor(const float blendFactor[4]);
    inline void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology) { mPrimitiveTopology = primitiveTopology; }

private:
    ID3D11BlendState* mBS;
    ID3D11RasterizerState* mRS;
    ID3D11DepthStencilState* mDSS;

    UINT mStencilRef = 0;
    float mBlendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    D3D11_PRIMITIVE_TOPOLOGY mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};