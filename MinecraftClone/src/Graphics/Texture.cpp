#include "Precompiled.h"

#include "Texture.h"
#include "Core/GraphicsEngine.h"

const char* Texture::sBasePath = "../Resources/";

void Texture::Load(const char* filename)
{
    ASSERT(filename != nullptr, "cannot load empty filename");
    mFilename = filename;

    size_t basePathLen = strlen(sBasePath);
    size_t filenameLen = strlen(filename);
    ASSERT(basePathLen + filenameLen < MAX_FULL_PATH_SIZE, "filename too long");

    memcpy(mFullPath, sBasePath, basePathLen);
    memcpy(mFullPath + basePathLen, filename, filenameLen);
    mFullPath[basePathLen + filenameLen] = '\0';

    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    D3D11Utils::CreateMipsTexture(ge.GetDevice(), ge.GetDeviceContext(), mFullPath, &mTex, &mTexSRV);
}

void Texture::Unload()
{
    RELEASE_COM(mTex);
    RELEASE_COM(mTexSRV);
}

void Texture::UseOn(uint8_t slot)
{
    ASSERT(slot < MAX_SLOT_COUNT, "greater than max slot count");

    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ge.GetDeviceContext().PSSetShaderResources(slot, 1, &mTexSRV);
}