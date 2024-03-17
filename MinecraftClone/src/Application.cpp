#include "Precompiled.h"

#include "Utils/Timer.h"
#include "Frustum.h"
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
    Timer::Init();
    WindowManager::Init();
    GraphicsEngine::Init();
    ImGuiUI::Init();

    WindowManager& wm = WindowManager::GetInstance();
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ImGuiUI& imGuiUI = ImGuiUI::GetInstance();

    InputManager& inputManager = InputManager::GetInstance();
    wm.BindInput(inputManager);

    Camera* playerCamera = new Camera();
    Player* player = new Player(playerCamera);
    Frustum::SetCamera(playerCamera);

    Water* water = new Water();
    Clouds* clouds = new Clouds();

    World* world = new World(playerCamera->GetEyePos());
    Scene* scene = new Scene(world, water, clouds);
    Renderer* renderer = new Renderer();

    Timer applicationTimer;
    while (wm.Tick())
    {
        const float dt = applicationTimer.GetDeltaTime();

        imGuiUI.Update(*renderer, *world, *player, dt);
        {
            player->HandleInput();
            player->Update(*world, dt);
            scene->Update(playerCamera->GetEyePos(), player->GetBlockHandler(), dt);
            renderer->Update(player->GetViewMatrix(), player->GetProjMatrix());

            renderer->Render(*scene, player->GetBlockHandler());

            inputManager.PostUpdate(); // #TODO find out why we need to call this post render?
        }
        imGuiUI.Render();

        ge.GetSwapChain().Present(1, 0);
    }

    // clean up
    delete playerCamera;
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