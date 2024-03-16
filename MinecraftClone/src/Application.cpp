#include "Precompiled.h"

#include "Managers/WindowManager.h"
#include "Managers/GraphicsEngine.h"
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
    WindowManager::Init();
    GraphicsEngine::Init();
    ImGuiUI::Init();

    WindowManager& WM = WindowManager::GetInstance();
    GraphicsEngine& GE = GraphicsEngine::GetInstance();
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

        GE.GetSwapChain().Present(1, 0);
    }

    // clean up
    delete camera;
    delete player;
    delete water;
    delete clouds;

    delete world;
    delete scene;
    delete renderer;

    ImGuiUI::Destroy();
    GraphicsEngine::Destroy();
    WindowManager::Destroy();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}