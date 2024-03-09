#pragma once

class GraphicsResourceManager final
{    
public:
    static bool CreateInstance(const IntVector2D& screenSize);
    static void DeleteInstance();
    static GraphicsResourceManager& GetInstance();

public:
    inline ID3D11Device& GetDevice() { return *mDevice; }
    inline ID3D11DeviceContext& GetDeviceContext() { return *mContext; }
    inline IDXGISwapChain& GetSwapChain() { return *mSC; }
    inline ID3D11RenderTargetView*& GetBackBufferRTV() { return mBackBufferRTV; }
    inline D3D11_VIEWPORT& GetViewport() { return mVP; }
    inline ID3D11DepthStencilView& GetDepthStencilView() { return *mDSV; }
    inline ID3D11DepthStencilState& GetDepthStencilState() { return *mDSS; }

    void OnScreenResize(const IntVector2D screenSize);

private:
    void setViewport();
    bool setBackBufferRTV();
    bool createDepthBuffers();

private:
    GraphicsResourceManager(const IntVector2D screenSize);
    ~GraphicsResourceManager();
    GraphicsResourceManager(const GraphicsResourceManager& rhs) = delete;
    GraphicsResourceManager& operator=(const GraphicsResourceManager& rhs) = delete;

private:
    static GraphicsResourceManager* sGRM;

    ID3D11Device* mDevice;
    ID3D11DeviceContext* mContext;
    IDXGISwapChain* mSC;
    DXGI_FORMAT mBackBufferFormat;
    ID3D11RenderTargetView* mBackBufferRTV;
    D3D11_VIEWPORT mVP;

    ID3D11Texture2D* mDSB;
    ID3D11DepthStencilView* mDSV;
    ID3D11DepthStencilState* mDSS;

    UINT32 mQualityLevels;
    IntVector2D mScreenSize;

    int mGuiWidth = 0;
};