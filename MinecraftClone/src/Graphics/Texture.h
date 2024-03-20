#pragma once

class Texture final
{
public:
    Texture() = default;
    ~Texture() = default;
    Texture(const Texture& rhs) = delete;
    Texture& operator=(const Texture& rhs) = delete;

    void Load(const char* filename);
    void Unload();
    void UseOn(uint8_t slot);
    inline const char* GetFilename() const { return mFilename; }

private:
    enum { MAX_SLOT_COUNT = 128 };
    enum { MAX_FULL_PATH_SIZE = 128 };

    static const char* sBasePath;

    ID3D11Texture2D* mTex;
    ID3D11ShaderResourceView* mTexSRV;

    const char* mFilename;
    char mFullPath[MAX_FULL_PATH_SIZE];
};