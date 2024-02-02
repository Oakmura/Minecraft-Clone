#include "Precompiled.h"

#include "Managers/ManagerHeader.h"
#include "Renderer.h"
#include "Player.h"

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

    bInitSucceeded &= UserInterface::CreateInstance(WM.GetWindowHandle(), GRM.GetDevice(), GRM.GetDeviceContext(), defaultScreenSize);
    UserInterface& UI = UserInterface::GetInstance();

    Player player;
    Renderer* renderer = new Renderer(GRM);
    
    WM.BindMouseMoveFunc(&Player::OnMouseMove);
    WM.BindKeyboardPressFunc(&Player::OnKeyboardPress);
    WM.BindKeyboardReleaseFunc(&Player::OnKeyboardRelease);
    WM.BindPlayer(&player);

    if (!bInitSucceeded)
    {
        goto CLEAN_UP;
    }

    WM.Show();
    WM.CenterWindow();

    while (WM.Tick())
    {
        UI.Update(GRM, *renderer);
        {
            renderer->Update(GRM, player, UI.GetDeltaTime());
            renderer->Render(GRM);
        }
        UI.Render();

        GRM.GetSwapChain()->Present(1, 0);
    }

CLEAN_UP:
    delete renderer;
    UserInterface::DeleteInstance();
    GraphicsResourceManager::DeleteInstance();
    WindowManager::DeleteInstance();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}