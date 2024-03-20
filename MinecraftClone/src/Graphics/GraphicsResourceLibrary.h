#pragma once

#include "Graphics/Texture.h"
#include "Graphics/GraphicsPSO.h"

class GraphicsResourceLibrary final
{
public:
    GraphicsResourceLibrary() = default;
    ~GraphicsResourceLibrary() = default;
    GraphicsResourceLibrary(const GraphicsResourceLibrary& rhs) = delete;
    GraphicsResourceLibrary& operator=(const GraphicsResourceLibrary& rhs) = delete;

    void AddIL(ID3D11InputLayout* il, const uint32_t key);
    void AddVS(ID3D11VertexShader* vs, const uint32_t key);
    void AddPS(ID3D11PixelShader* ps, const uint32_t key);
    void AddTex(Texture* tex, const uint32_t key);
    void AddPSO(GraphicsPSO* pso, const uint32_t key);

    ID3D11InputLayout& GetIL(const uint32_t key);
    ID3D11VertexShader& GetVS(const uint32_t key);
    ID3D11PixelShader& GetPS(const uint32_t key);
    Texture& GetTex(const uint32_t key);
    GraphicsPSO& GetPSO(const uint32_t key);

private:
    std::unordered_map<uint32_t, ID3D11InputLayout*> mILs;
    std::unordered_map<uint32_t, ID3D11VertexShader*> mVSs;
    std::unordered_map<uint32_t, ID3D11PixelShader*> mPSs;
    std::unordered_map<uint32_t, Texture*> mTexs;
    std::unordered_map<uint32_t, GraphicsPSO*> mPSOs;
};