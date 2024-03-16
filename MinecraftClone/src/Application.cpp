#include "Precompiled.h"

#include "Managers/WindowManager.h"
#include "Managers/GraphicsResourceManager.h"
#include "Managers/ImGuiUI.h"
#include "Renderer.h"
#include "Player.h"
#include "Water.h"
#include "Clouds.h"

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

    bInitSucceeded &= ImGuiUI::CreateInstance(defaultScreenSize);
    ImGuiUI& imGuiUI = ImGuiUI::GetInstance();

    InputManager& inputManager = InputManager::GetInstance();
    WM.BindInput(inputManager);
    
    Camera* camera = new Camera();
    Player* player = new Player(camera);
    Water* water = new Water();
    Clouds* clouds = new Clouds();

    World* world = new World(camera->GetEyePos());
    Scene* scene = new Scene(world, water, clouds);
    Renderer* renderer = new Renderer();

    if (!bInitSucceeded)
    {
        goto CLEAN_UP;
    }

    while (WM.Tick())
    {
        imGuiUI.Update(*renderer, *world, *player);
        {
            player->HandleInput();

            player->Update(*world, imGuiUI.GetDeltaTime());
            scene->Update(camera->GetEyePos(), player->GetBlockHandler(), imGuiUI.GetDeltaTime());
            renderer->Update(player->GetViewMatrix(), player->GetProjMatrix());

            renderer->Render(*scene, player->GetBlockHandler());

            inputManager.PostUpdate(); // #TODO find out why we need to call this post render?
        }
        imGuiUI.Render();

        GRM.GetSwapChain().Present(1, 0);
    }

CLEAN_UP:
    delete camera;
    delete player;
    delete water;
    delete clouds;

    delete world;
    delete scene;
    delete renderer;
    ImGuiUI::DeleteInstance();
    GraphicsResourceManager::DeleteInstance();
    WindowManager::DeleteInstance();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}