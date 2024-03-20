#include "Precompiled.h"
#include "GraphicsResourceLibrary.h"

void GraphicsResourceLibrary::AddIL(ID3D11InputLayout* il, const uint32_t key)
{
    ASSERT(mILs.find(key) == mILs.end(), "Trying to add already existing input layout");

    mILs.insert({ key, il });
}

void GraphicsResourceLibrary::AddVS(ID3D11VertexShader* vs, const uint32_t key)
{
    ASSERT(mVSs.find(key) == mVSs.end(), "Trying to add already existing shader");

    mVSs.insert({ key, vs });
}

void GraphicsResourceLibrary::AddPS(ID3D11PixelShader* ps, const uint32_t key)
{
    ASSERT(mPSs.find(key) == mPSs.end(), "Trying to add already existing shader");

    mPSs.insert({ key, ps });
}

void GraphicsResourceLibrary::AddTex(Texture* tex, const uint32_t key)
{
    ASSERT(mTexs.find(key) == mTexs.end(), "Trying to add already existing texture filename");

    mTexs.insert({ key, tex });
}

void GraphicsResourceLibrary::AddPSO(GraphicsPSO* pso, const uint32_t key)
{
    ASSERT(mPSOs.find(key) == mPSOs.end(), "Trying to add already existing PSO");

    mPSOs.insert({ key, pso });
}

ID3D11InputLayout& GraphicsResourceLibrary::GetIL(const uint32_t key)
{
    ASSERT(mILs.find(key) != mILs.end(), "Trying to get unidentified input layout");

    return *mILs[key];
}

ID3D11VertexShader& GraphicsResourceLibrary::GetVS(const uint32_t key)
{
    ASSERT(mVSs.find(key) != mVSs.end(), "Trying to get unidentified shader");

    return *mVSs[key];
}

ID3D11PixelShader& GraphicsResourceLibrary::GetPS(const uint32_t key)
{
    ASSERT(mPSs.find(key) != mPSs.end(), "Trying to get unidentified shader");

    return *mPSs[key];
}

Texture& GraphicsResourceLibrary::GetTex(const uint32_t key)
{
    ASSERT(mTexs.find(key) != mTexs.end(), "Trying to get unidentified texture");

    return *mTexs[key];
}

GraphicsPSO& GraphicsResourceLibrary::GetPSO(const uint32_t key)
{
    ASSERT(mPSOs.find(key) != mPSOs.end(), "Trying to get unidentified PSOs");

    return *mPSOs[key];
}