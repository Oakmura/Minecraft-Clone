#pragma once

class GraphicsResourceManager final
{
    friend class Chunk;
    friend class ChunkBuilder;
    friend class World;
    friend class Renderer;
    
public:
    static bool CreateInstance(const IntVector2D& screenSize);
    static void DeleteInstance();
    static GraphicsResourceManager& GetInstance();

public:
    inline ID3D11Device* GetDevice() const { return mDevice; }
    inline ID3D11DeviceContext* GetDeviceContext() const { return mContext; }
    inline IDXGISwapChain* GetSwapChain() const { return mSC; }

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

    HWND mWindowHandle;
    UINT32 mQualityLevels;
    IntVector2D mScreenSize;

    int mGuiWidth = 0;
};