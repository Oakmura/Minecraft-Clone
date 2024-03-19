#include "Precompiled.h"

#include "Utils/Timer.h"
#include "Frustum.h"
#include "Core/WindowManager.h"
#include "Core/GraphicsEngine.h"
#include "Core/ImGuiUI.h"
#include "Core/Renderer.h"
#include "Scene/Water.h"
#include "Scene/Clouds.h"
#include "Player.h"

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

    Camera playerCamera;
    Player player(&playerCamera);
    Frustum::SetCamera(&playerCamera);

    Water water;
    Clouds clouds;
    World* world = new World(playerCamera.GetEyePos()); // heap allocation due to size
    Scene scene(world, &water, &clouds);

    Renderer renderer;
    Timer applicationTimer;
    while (wm.Tick())
    {
        const float dt = applicationTimer.GetDeltaTime();

        imGuiUI.Update(renderer, *world, player, dt);
        {
            player.HandleInput();
            player.Update(*world, dt);
            scene.Update(playerCamera.GetEyePos(), player.GetBlockHandler(), dt);
            renderer.Update(player.GetViewMatrix(), player.GetProjMatrix());

            renderer.Render(scene, player.GetBlockHandler());

            inputManager.PostUpdate(); // #TODO find out why we need to call this post render?
        }
        imGuiUI.Render();

        ge.GetSwapChain().Present(1, 0);
    }

    delete world;

    ImGuiUI::Destroy();
    GraphicsEngine::Destroy();
    WindowManager::Destroy();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}