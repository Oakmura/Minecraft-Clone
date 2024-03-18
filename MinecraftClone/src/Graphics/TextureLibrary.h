#pragma once

#include "Graphics/Texture.h"

class TextureLibrary final
{
public:
    TextureLibrary() = default;
    ~TextureLibrary();
    TextureLibrary(const TextureLibrary& rhs) = delete;
    TextureLibrary& operator=(const TextureLibrary& rhs) = delete;
    
    void Add(Texture* texture, const uint32_t key);
    Texture& Get(const uint32_t key);

private:
    std::unordered_map<uint32_t, Texture*> mTextures;
};