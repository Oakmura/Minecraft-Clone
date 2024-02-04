#include "Precompiled.h"

#include "D3D11Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

bool D3D11Utils::CreateTexture(ID3D11Device& device, const char* filename, ID3D11Texture2D** tex, ID3D11ShaderResourceView** texSRV)
{
    int width, height, channels;
    std::vector<uint8_t> image;
    readImage(filename, &image, &width, &height, &channels);

    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = 1;
    txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_DEFAULT;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = image.data();
    InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * channels;
    InitData.SysMemSlicePitch = 0;

    HRESULT hr = S_OK;
    DX_CALL(hr = device.CreateTexture2D(&txtDesc, &InitData, tex));
    if (FAILED(hr))
    {
        return false;
    }

    DX_CALL(hr = device.CreateShaderResourceView(*tex, nullptr, texSRV));
    if (FAILED(hr))
    {
        return false;
    }
    
    return true;
}

bool D3D11Utils::CreateMipsTexture(ID3D11Device& device, ID3D11DeviceContext& context, const char* filename, 
    ID3D11Texture2D** tex, ID3D11ShaderResourceView** texSRV)
{
    int width, height, channels;
    std::vector<uint8_t> image;
    readImage(filename, &image, &width, &height, &channels);

    ID3D11Texture2D* stagingTex;
    createStagingTexture(device, context, &stagingTex, width, height, image);

    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = 0;
    txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_DEFAULT;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    txtDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    txtDesc.CPUAccessFlags = 0;

    HRESULT hr = S_OK;
    DX_CALL(hr = device.CreateTexture2D(&txtDesc, nullptr, tex));
    context.CopySubresourceRegion(*tex, 0, 0, 0, 0, stagingTex, 0, nullptr);
    DX_CALL(device.CreateShaderResourceView(*tex, 0, texSRV));
    context.GenerateMips(*texSRV);

    RELEASE_COM(stagingTex);

    return true;
}

bool D3D11Utils::createStagingTexture(ID3D11Device& device, ID3D11DeviceContext& context, ID3D11Texture2D** outStagingTex, 
    const int width, const int height, const std::vector<uint8_t>& image)
{
    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = 1;
    txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_STAGING;
    txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // WRITE | READ

    HRESULT hr = S_OK;
    DX_CALL(hr = device.CreateTexture2D(&txtDesc, nullptr, outStagingTex));
    if (FAILED(hr))
    {
        return false;
    }

    D3D11_MAPPED_SUBRESOURCE ms;
    DX_CALL(hr = context.Map(*outStagingTex, 0, D3D11_MAP_WRITE, 0, &ms));
    if (FAILED(hr))
    {
        return false;
    }

    size_t pixelSize = getPixelSize(txtDesc.Format);
    uint8_t* pData = (uint8_t*)ms.pData;
    for (UINT h = 0; h < UINT(height); ++h)
    {
        memcpy(&pData[h * ms.RowPitch], &image[h * width * pixelSize], width * pixelSize);
    }
    context.Unmap(*outStagingTex, 0);

    return true;
}

void D3D11Utils::readImage(const char* filename, std::vector<uint8_t>* outImage, int* outWidth, int* outHeight, int* outChannels)
{
    unsigned char* img = stbi_load(filename, outWidth, outHeight, outChannels, 0);

    const int imageSize = (*outWidth) * (*outHeight);
    const int channels = (*outChannels);
    outImage->resize(imageSize * 4); // 4채널로 만들어서 복사 
    
    switch (channels)
    {
    case 1:
        for (size_t i = 0; i < imageSize; ++i)
        {
            uint8_t g = img[i * channels + 0];
            for (size_t c = 0; c < 4; ++c)
            {
                (*outImage)[4 * i + c] = g;
            }
        }
        break;
    case 2:
        for (size_t i = 0; i < imageSize; ++i)
        {
            for (size_t c = 0; c < 2; ++c)
            {
                (*outImage)[4 * i + c] = img[i * channels + c];
            }
            (*outImage)[4 * i + 2] = 255;
            (*outImage)[4 * i + 3] = 255;
        }
        break;
    case 3:
        for (size_t i = 0; i < imageSize; ++i)
        {
            for (size_t c = 0; c < 3; ++c)
            {
                (*outImage)[4 * i + c] = img[i * channels + c];
            }
            (*outImage)[4 * i + 3] = 255;
        }
        break;
    case 4:
        memcpy(outImage->data(), img, imageSize * channels);
        //for (size_t i = 0; i < imageSize; ++i)
        //{
        //    for (size_t c = 0; c < 4; c++)
        //    {
        //        (*outImage)[4 * i + c] = img[i * channels + c];
        //    }
        //}
        break;
    default:
        ASSERT(false, "undefined number of channels for image");
    }

    LOG_INFO("filename: {0}, numPixels: {1}, width: {2}, height: {3}, channels: {4}", filename, outImage->size(), *outWidth, *outHeight, channels);

    delete[] img;
}

size_t D3D11Utils::getPixelSize(DXGI_FORMAT pixelFormat)
{
    switch (pixelFormat) 
    {
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            return sizeof(uint16_t) * 4;
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            return sizeof(uint32_t) * 4;
        case DXGI_FORMAT_R32_FLOAT:
            return sizeof(uint32_t) * 1;
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            return sizeof(uint8_t) * 4;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return sizeof(uint8_t) * 4;
        case DXGI_FORMAT_R32_SINT:
            return sizeof(int32_t) * 1;
        case DXGI_FORMAT_R16_FLOAT:
            return sizeof(uint16_t) * 1;
        default:
            ASSERT(false, "PixelFormat not implemented");
    }

    return sizeof(uint8_t) * 4;
}
