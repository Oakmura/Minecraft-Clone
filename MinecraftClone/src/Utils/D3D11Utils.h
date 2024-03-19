#pragma once

#ifdef _DEBUG
#define DX_CALL(x) \
        if (FAILED(x)) \
        { \
            LOG_ERROR("File: {0}\nLine: {1}\nFunction: {2}", __FILE__, __LINE__, __func__); \
            __debugbreak(); \
        }
#else
#define DX_CALL(x) x
#endif

#define RELEASE_COM(COM) { if(COM) { COM->Release(); COM = nullptr; } }

class D3D11Utils final
{
public:
    static void CheckResourceLeak();

    template<typename T>
    static bool CreateVertexBuffer(ID3D11Device& device, const std::vector<T>& vertices, ID3D11Buffer** outVertexBuffer);
    template<typename T>
    static bool CreateConstantBuffer(ID3D11Device& device, const T& constantBufferData, ID3D11Buffer** outConstantBuffer);
    template<typename T>
    static bool UpdateBuffer(ID3D11DeviceContext& context, const T& bufferData, ID3D11Buffer* buffer);
    static bool CreateIndexBuffer(ID3D11Device& device, const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer);
        
    static bool CreateVertexShaderAndInputLayout(ID3D11Device& device, const wchar_t* filename,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ID3D11VertexShader** outVertexShader, ID3D11InputLayout** outInputLayout);
    static bool CreatePixelShader(ID3D11Device& device, const wchar_t* filename, ID3D11PixelShader** outPixelShader);

    static bool CreateTexture(ID3D11Device& device, const char* filename, ID3D11Texture2D** tex, ID3D11ShaderResourceView** texSRV);
    static bool CreateMipsTexture(ID3D11Device& device, ID3D11DeviceContext& context, const char* filename, ID3D11Texture2D** tex, ID3D11ShaderResourceView** texSRV);
private:
    static bool createStagingTexture(ID3D11Device& device, ID3D11DeviceContext& context, ID3D11Texture2D** outStagingTex, const int width, const int height, const std::vector<uint8_t>& image);
    static void readImage(const char* filename, std::vector<uint8_t>* outImage, int* outWidth, int* outHeight, int* outChannels);
    static size_t getPixelSize(DXGI_FORMAT pixelFormat);
};

template<typename T>
inline bool D3D11Utils::CreateVertexBuffer(ID3D11Device& device, const std::vector<T>& vertices, ID3D11Buffer** outVertexBuffer)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
    bufferDesc.StructureByteStride = sizeof(T);

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = vertices.data();
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    HRESULT hr = S_OK;
    DX_CALL(hr = device.CreateBuffer(&bufferDesc, &vertexBufferData, outVertexBuffer));

    return FAILED(hr) ? false : true;
}

template<typename T>
bool D3D11Utils::CreateConstantBuffer(ID3D11Device& device, const T& constantBufferData, ID3D11Buffer** outConstantBuffer)
{
    ASSERT(outConstantBuffer, "outConstantBuffer is nullptr");
    static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth = sizeof(constantBufferData);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = &constantBufferData;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    HRESULT hr = S_OK;
    DX_CALL(hr = device.CreateBuffer(&desc, &initData, outConstantBuffer));

    return FAILED(hr) ? false : true;
}

template<typename T>
bool D3D11Utils::UpdateBuffer(ID3D11DeviceContext& context, const T& bufferData, ID3D11Buffer* buffer)
{
    ASSERT(buffer, "D3D11Utils::UpdateBuffer : buffer is nullptr");

    D3D11_MAPPED_SUBRESOURCE ms;
    HRESULT hr = S_OK;
    DX_CALL(hr = context.Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
    if (FAILED(hr))
    {
        return false;
    }

    memcpy(ms.pData, &bufferData, sizeof(bufferData));
    context.Unmap(buffer, 0);

    return true;
}