#include "Precompiled.h"

#include "TextureLibrary.h"

TextureLibrary::~TextureLibrary()
{
    for (auto& texture : mTextures)
    {
        delete texture.second;
    }
}

void TextureLibrary::Add(Texture* texture, const uint32_t key)
{
    ASSERT(mTextures.find(key) == mTextures.end(), "Trying to add already existing texture filename");

    mTextures.insert({ key, texture });
}

Texture& TextureLibrary::Get(const uint32_t key)
{
    ASSERT(mTextures.find(key) != mTextures.end(), "Trying to get unidentified texture");

    return *mTextures[key];
}