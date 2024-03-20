#pragma once

#include "Settings.h"
#include "Graphics/GraphicsResourceLibrary.h"
#include "Graphics/GraphicsCommon.h"

class GraphicsEngine final
{    
    friend class ImGuiUI;
public:
    static void Init();
    static void Destroy();
    static GraphicsEngine& GetInstance();

public:
    inline ID3D11Device& GetDevice() { return *mDevice; }
    inline ID3D11DeviceContext& GetDeviceContext() { return *mContext; }
    inline IDXGISwapChain& GetSwapChain() { return *mSC; }
    inline ID3D11RenderTargetView*& GetBackBufferRTV() { return mBackBufferRTV; }
    inline D3D11_VIEWPORT& GetViewport() { return mVP; }
    inline ID3D11DepthStencilView& GetDepthStencilView() { return *mDSV; }

    inline std::vector<ID3D11SamplerState*>& GetSamplers() { return GraphicsCommon::sSampleStates; }
    inline GraphicsResourceLibrary& GetResourceLibrary() { return mGraphicsResourceLibrary; }

    inline void Present() const { mSC->Present(mbVSync ? 1 : 0, 0); }
    void OnScreenResize(const IntVector2D screenSize);

private:
    void setViewport();
    void setBackBufferRTV();
    void createDepthBuffers();

    void addInputLayouts();
    void addShaders();
    void addTextures();
    void addPSOs();

private:
    GraphicsEngine() = delete;
    GraphicsEngine(const IntVector2D& screenSize);
    GraphicsEngine(const GraphicsEngine& rhs) = delete;
    ~GraphicsEngine();
    GraphicsEngine& operator=(const GraphicsEngine& rhs) = delete;

private:
    static GraphicsEngine* sGRM;

    ID3D11Device* mDevice;
    ID3D11DeviceContext* mContext;
    IDXGISwapChain* mSC;
    DXGI_FORMAT mBackBufferFormat;
    ID3D11RenderTargetView* mBackBufferRTV;
    D3D11_VIEWPORT mVP;

    ID3D11Texture2D* mDSB;
    ID3D11DepthStencilView* mDSV;

    UINT32 mQualityLevels;
    IntVector2D mScreenSize;

    bool mbVSync;

    GraphicsResourceLibrary mGraphicsResourceLibrary;
};