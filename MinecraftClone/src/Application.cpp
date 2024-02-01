#include "Precompiled.h"

#include "Managers/ManagerHeader.h"

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Logger::Init();

    bool bInitSucceeded = true;
    IntVector2D defaultScreenSize(1280, 720);

    bInitSucceeded &= WindowManager::CreateInstance(defaultScreenSize);
    WindowManager& WM = WindowManager::GetInstance();

    bInitSucceeded &= GraphicsResourceManager::CreateInstance(WM.GetWindowHandle(), defaultScreenSize);
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    UserInterface::CreateInstance(WM.GetWindowHandle(), GRM.GetDevice(), GRM.GetDeviceContext(), defaultScreenSize);
    UserInterface& UI = UserInterface::GetInstance();

    if (!bInitSucceeded)
    {
        goto CLEAN_UP;
    }

    WM.Show();
    WM.CenterWindow();

    while (WM.Tick())
    {
        UI.Update(GRM);
        {
        }
        UI.Render();
    }

CLEAN_UP:
    UserInterface::DeleteInstance();
    GraphicsResourceManager::DeleteInstance();
    WindowManager::DeleteInstance();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}