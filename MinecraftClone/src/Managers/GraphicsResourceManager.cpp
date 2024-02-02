#include "Precompiled.h"

#include "GraphicsResourceManager.h"

GraphicsResourceManager* GraphicsResourceManager::sGRM = nullptr;

GraphicsResourceManager::GraphicsResourceManager(HWND hwnd, const IntVector2D screenSize)
    : mWindowHandle(hwnd)
    , mBackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , mScreenSize(screenSize)
    , mQualityLevels(0)
    , mVP(D3D11_VIEWPORT())

    , mDevice(nullptr)
    , mContext(nullptr)
    , mSC(nullptr)
    , mBackBufferRTV(nullptr)
    , mDSB(nullptr)
    , mDSV(nullptr)
    , mDSS(nullptr)
{
}

GraphicsResourceManager::~GraphicsResourceManager()
{
    RELEASE_COM(mDevice);
    RELEASE_COM(mContext);
    RELEASE_COM(mSC);

    RELEASE_COM(mBackBufferRTV);

    RELEASE_COM(mDSB);
    RELEASE_COM(mDSV);
    RELEASE_COM(mDSS);
}

bool GraphicsResourceManager::CreateInstance(HWND hwnd, const IntVector2D& screenSize)
{
    ASSERT(sGRM == nullptr, "GraphicsResourceManager::CreateInstance() : instance already created");

    sGRM = new GraphicsResourceManager(hwnd, IntVector2D(screenSize.mX, screenSize.mY));

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    const D3D_FEATURE_LEVEL featureLevels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_9_3 };
    D3D_FEATURE_LEVEL featureLevel;

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferDesc.Width = sGRM->mScreenSize.mX;
    scd.BufferDesc.Height = sGRM->mScreenSize.mY;
    scd.BufferDesc.Format = sGRM->mBackBufferFormat;
    scd.BufferCount = 2;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = sGRM->mWindowHandle;
    scd.Windowed = TRUE;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.SampleDesc.Count = 1; // _FLIP_은 MSAA 미지원
    scd.SampleDesc.Quality = 0;

    HRESULT hr = S_OK;
    DX_CALL(hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION, &scd, &sGRM->mSC,
        &sGRM->mDevice, &featureLevel, &sGRM->mContext));
    if (FAILED(hr))
    {
        return false;
    }

    DX_CALL(hr = sGRM->mDevice->CheckMultisampleQualityLevels(sGRM->mBackBufferFormat, 4, &sGRM->mQualityLevels));
    if (FAILED(hr))
    {
        return false;
    }

    sGRM->setViewport();
    if (!sGRM->setBackBufferRTV())
    {
        return false;
    }

    if (!sGRM->createDepthBuffers())
    {
        return false;
    }

    return true;
}

void GraphicsResourceManager::DeleteInstance()
{
    ASSERT(sGRM, "GraphicsResourceManager::DeleteInstance() : instance not created");

    if (sGRM)
    {
        delete sGRM;
        sGRM = nullptr;
    }
}

GraphicsResourceManager& GraphicsResourceManager::GetInstance()
{
    ASSERT(sGRM, "GraphicsResourceManager::GetInstance() : instance not created");

    return *sGRM;
}

void GraphicsResourceManager::OnScreenResize(const IntVector2D screenSize)
{
    if (mSC == nullptr)
    {
        return;
    }

    RELEASE_COM(mBackBufferRTV);
    RELEASE_COM(mDSB);
    RELEASE_COM(mDSV);

    mScreenSize = screenSize;
    
    HRESULT hr = S_OK;
    DX_CALL(hr = mSC->ResizeBuffers(0, screenSize.mX, screenSize.mY, DXGI_FORMAT_UNKNOWN, 0));

    setBackBufferRTV();
    createDepthBuffers();
    setViewport();
}

void GraphicsResourceManager::setViewport()
{
    ZeroMemory(&mVP, sizeof(D3D11_VIEWPORT));
    mVP.TopLeftX = static_cast<float>(mGuiWidth);
    mVP.TopLeftY = 0;
    mVP.Width = static_cast<float>(mScreenSize.mX - mGuiWidth);
    mVP.Height = static_cast<float>(mScreenSize.mY);
    mVP.MinDepth = 0.0f;
    mVP.MaxDepth = 1.0f;

    mContext->RSSetViewports(1, &mVP);
}

bool GraphicsResourceManager::setBackBufferRTV()
{
    ID3D11Texture2D* backBuffer;

    HRESULT hr = S_OK;
    DX_CALL(hr = mSC->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
    if (FAILED(hr))
    {
        return false;
    }

    DX_CALL(hr = mDevice->CreateRenderTargetView(backBuffer, nullptr, &mBackBufferRTV));
    if (FAILED(hr))
    {
        return false;
    }

    backBuffer->Release();
    return true;
}

bool GraphicsResourceManager::createDepthBuffers()
{
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = mScreenSize.mX;
    desc.Height = mScreenSize.mY;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    HRESULT hr = S_OK;
    DX_CALL(hr = mDevice->CreateTexture2D(&desc, 0, &mDSB));
    if (FAILED(hr))
    {
        return false;
    }

    DX_CALL(hr = mDevice->CreateDepthStencilView(mDSB, nullptr, &mDSV));
    if (FAILED(hr))
    {
        return false;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    DX_CALL(hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDSS));
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
