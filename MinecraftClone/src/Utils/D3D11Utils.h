#pragma once

class D3D11Utils final
{
public:
    static void CheckResourceLeak();

    template<typename T>
    static bool CreateVertexBuffer(ID3D11Device& device, const std::vector<T>& vertices, ID3D11Buffer** outVertexBuffer);
    static bool CreateIndexBuffer(ID3D11Device& device, const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer);

    static bool CreateVertexShaderAndInputLayout(ID3D11Device& device, const wchar_t* filename,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ID3D11VertexShader** outVertexShader, ID3D11InputLayout** outInputLayout);
    static bool CreatePixelShader(ID3D11Device& device, const wchar_t* filename, ID3D11PixelShader** outPixelShader);
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
