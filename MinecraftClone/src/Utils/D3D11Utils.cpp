#include "Precompiled.h"

#include "D3D11Utils.h"

void D3D11Utils::CheckResourceLeak()
{
    HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
    decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

    IDXGIDebug* debug;
    GetDebugInterface(IID_PPV_ARGS(&debug));

    OutputDebugStringW(L"----------------------------------------------Starting Live Direct3D Object Dump:----------------------------------------------\n");
    debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    OutputDebugStringW(L"----------------------------------------------Completed Live Direct3D Object Dump----------------------------------------------\n");

    debug->Release();
}

bool D3D11Utils::CreateIndexBuffer(ID3D11Device& device, const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size());
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.StructureByteStride = sizeof(uint32_t);

    D3D11_SUBRESOURCE_DATA indexBufferData;
    ZeroMemory(&indexBufferData, sizeof(indexBufferData));
    indexBufferData.pSysMem = indices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    HRESULT hr = S_OK;
    DX_CALL(hr = device.CreateBuffer(&bufferDesc, &indexBufferData, outIndexBuffer));

    return FAILED(hr) ? false : true;
}

bool D3D11Utils::CreateVertexShaderAndInputLayout(ID3D11Device& device, const wchar_t* filename, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ID3D11VertexShader** outVertexShader, ID3D11InputLayout** outInputLayout)
{
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = S_OK;
    DX_CALL(hr = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob));
    if (FAILED(hr))
    {
        RELEASE_COM(shaderBlob)
        return false;
    }

    DX_CALL(hr = device.CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, outVertexShader));
    if (FAILED(hr))
    {
        RELEASE_COM(shaderBlob)
        return false;
    }

    DX_CALL(hr = device.CreateInputLayout(inputElements.data(), static_cast<UINT>(inputElements.size()), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), outInputLayout));
    if (FAILED(hr))
    {
        RELEASE_COM(shaderBlob)
        return false;
    }

    RELEASE_COM(shaderBlob)
    return true;
}

bool D3D11Utils::CreatePixelShader(ID3D11Device& device, const wchar_t* filename, ID3D11PixelShader** outPixelShader)
{
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = S_OK;
    DX_CALL(hr = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob));
    if (FAILED(hr))
    {
        RELEASE_COM(shaderBlob)
        return false;
    }

    DX_CALL(hr = device.CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, outPixelShader));
    if (FAILED(hr))
    {
        RELEASE_COM(shaderBlob)
        return false;
    }

    RELEASE_COM(shaderBlob)
    return true;
}
