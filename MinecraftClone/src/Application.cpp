#include "Precompiled.h"

#include "Managers/WindowManager.h"
#include "Managers/GraphicsResourceManager.h"
#include "Managers/UserInterface.h"
#include "Renderer.h"

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

    bInitSucceeded &= GraphicsResourceManager::CreateInstance(defaultScreenSize);
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    bInitSucceeded &= UserInterface::CreateInstance(defaultScreenSize);
    UserInterface& UI = UserInterface::GetInstance();
    
    Camera* camera = new Camera();
    Scene* scene = new Scene(camera);
    Renderer* renderer = new Renderer();
    
    WM.BindMouseButtonDownFunc(&Player::OnMouseButtonDown);
    WM.BindMouseMoveFunc(&Player::OnMouseMove);
    WM.BindKeyboardPressFunc(&Player::OnKeyboardPress);
    WM.BindKeyboardReleaseFunc(&Player::OnKeyboardRelease);
    WM.BindPlayer(&scene->GetPlayer());

    if (!bInitSucceeded)
    {
        goto CLEAN_UP;
    }

    WM.Show();
    WM.CenterWindow();

    while (WM.Tick())
    {
        UI.Update(*renderer, scene->GetPlayer());
        {
            renderer->Update(*scene, UI.GetDeltaTime());
            renderer->Render(*scene);
        }
        UI.Render();

        GRM.GetSwapChain().Present(1, 0);
    }

CLEAN_UP:
    delete camera;
    delete scene;
    delete renderer;
    UserInterface::DeleteInstance();
    GraphicsResourceManager::DeleteInstance();
    WindowManager::DeleteInstance();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}