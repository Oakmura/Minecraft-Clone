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