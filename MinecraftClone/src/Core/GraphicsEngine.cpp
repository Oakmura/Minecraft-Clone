#include "Precompiled.h"

#include "GraphicsEngine.h"
#include "WindowManager.h"

GraphicsEngine* GraphicsEngine::sGRM = nullptr;

GraphicsEngine::GraphicsEngine(const IntVector2D screenSize)
    : mBackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , mScreenSize(screenSize)
{
}

GraphicsEngine::~GraphicsEngine()
{
    RELEASE_COM(mDevice);
    RELEASE_COM(mContext);
    RELEASE_COM(mSC);

    RELEASE_COM(mBackBufferRTV);

    RELEASE_COM(mDSB);
    RELEASE_COM(mDSV);
    RELEASE_COM(mDSS);
}

void GraphicsEngine::Init()
{
    ASSERT(sGRM == nullptr, "GraphicsResourceManager::CreateInstance() : instance already created");

    sGRM = new GraphicsEngine(def::g_SCREEN_SIZE);

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
    scd.OutputWindow = WindowManager::GetInstance().GetWindowHandle();
    scd.Windowed = TRUE;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.SampleDesc.Count = 1; // _FLIP_은 MSAA 미지원
    scd.SampleDesc.Quality = 0;

    DX_CALL(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION, &scd, &sGRM->mSC,
        &sGRM->mDevice, &featureLevel, &sGRM->mContext));

    DX_CALL(sGRM->mDevice->CheckMultisampleQualityLevels(sGRM->mBackBufferFormat, 4, &sGRM->mQualityLevels));

    sGRM->setViewport();
    sGRM->setBackBufferRTV();
    sGRM->createDepthBuffers();
}

void GraphicsEngine::Destroy()
{
    ASSERT(sGRM, "GraphicsResourceManager::DeleteInstance() : instance not created");

    delete sGRM;
    sGRM = nullptr;
}

GraphicsEngine& GraphicsEngine::GetInstance()
{
    ASSERT(sGRM, "GraphicsResourceManager::GetInstance() : instance not created");

    return *sGRM;
}

void GraphicsEngine::OnScreenResize(const IntVector2D screenSize)
{
    if (mSC == nullptr)
    {
        return;
    }

    RELEASE_COM(mBackBufferRTV);
    RELEASE_COM(mDSB);
    RELEASE_COM(mDSV);

    mScreenSize = screenSize;
    
    DX_CALL(mSC->ResizeBuffers(0, screenSize.mX, screenSize.mY, DXGI_FORMAT_UNKNOWN, 0));

    setBackBufferRTV();
    createDepthBuffers();
    setViewport();
}

void GraphicsEngine::setViewport()
{
    ZeroMemory(&mVP, sizeof(D3D11_VIEWPORT));
    mVP.TopLeftX = 0;
    mVP.TopLeftY = 0;
    mVP.Width = static_cast<float>(mScreenSize.mX);
    mVP.Height = static_cast<float>(mScreenSize.mY);
    mVP.MinDepth = 0.0f;
    mVP.MaxDepth = 1.0f;

    mContext->RSSetViewports(1, &mVP);
}

void GraphicsEngine::setBackBufferRTV()
{
    ID3D11Texture2D* backBuffer;

    DX_CALL(mSC->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
    DX_CALL(mDevice->CreateRenderTargetView(backBuffer, nullptr, &mBackBufferRTV));
    backBuffer->Release();
}

void GraphicsEngine::createDepthBuffers()
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

    DX_CALL(mDevice->CreateTexture2D(&desc, 0, &mDSB));
    DX_CALL(mDevice->CreateDepthStencilView(mDSB, nullptr, &mDSV));

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    DX_CALL(mDevice->CreateDepthStencilState(&depthStencilDesc, &mDSS));
}
