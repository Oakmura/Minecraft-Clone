#include "Precompiled.h"

#include "Managers/WindowManager.h"
#include "Managers/GraphicsResourceManager.h"
#include "Managers/UserInterface.h"
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

    bInitSucceeded &= GraphicsResourceManager::CreateInstance(defaultScreenSize);
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    bInitSucceeded &= UserInterface::CreateInstance(defaultScreenSize);
    UserInterface& UI = UserInterface::GetInstance();

    InputManager& inputManager = InputManager::GetInstance();
    WM.BindInput(inputManager);
    
    Camera* camera = new Camera();
    Player* player = new Player(camera);
    World* world = new World(camera->GetEyePos());
    Scene* scene = new Scene(world);
    Renderer* renderer = new Renderer();

    if (!bInitSucceeded)
    {
        goto CLEAN_UP;
    }

    while (WM.Tick())
    {
        UI.Update(*renderer, *world, *player);
        {
            player->HandleInput();
            player->Update(*world, UI.GetDeltaTime());
            scene->Update(player->GetVoxelHandler(), UI.GetDeltaTime());
            renderer->Render(*scene, player->GetViewMatrix(), player->GetProjMatrix(), player->GetVoxelHandler());

            inputManager.UpdateInput(); // #TODO find out why we need to call this post render?
        }
        UI.Render();

        GRM.GetSwapChain().Present(1, 0);
    }

CLEAN_UP:
    delete camera;
    delete player;
    delete world;
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