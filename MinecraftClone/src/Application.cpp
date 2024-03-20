#include "Precompiled.h"

#include "Utils/Timer.h"
#include "Frustum.h"
#include "Core/WindowManager.h"
#include "Core/GraphicsEngine.h"
#include "Core/ImGuiUI.h"
#include "Core/Renderer.h"
#include "Scene/Water.h"
#include "Scene/Clouds.h"
#include "Scene/Player.h"

int GuardedMain()
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

    { // scope for destroying stack instances
        Camera playerCamera;
        Player player(&playerCamera);
        Frustum::SetCamera(&playerCamera);

        Water water;
        Clouds clouds;
        World* world;
        {
            Timer worldTimer;
            world = new World(); // heap allocation due to size
            LOG_INFO("Size of World: {0}", sizeof(World));
            LOG_INFO("Time took to generate world: {0}s", worldTimer.GetDeltaTime());
        }
        Scene scene(world, &water, &clouds, &player);

        Renderer renderer;
        Timer applicationTimer;
        while (wm.Tick())
        {
            const float dt = applicationTimer.GetDeltaTime();

            imGuiUI.Update(ge, renderer, scene, player, dt);
            {
                player.HandleInput();
                scene.Update(dt);
                renderer.Update(player.GetViewMatrix(), player.GetProjMatrix());
                renderer.Render(scene);

                inputManager.PostUpdate();
            }
            imGuiUI.Render();

            ge.Present();
        }

        delete world;
    }

    ImGuiUI::Destroy();
    GraphicsEngine::Destroy();
    WindowManager::Destroy();

#ifdef _DEBUG
    D3D11Utils::CheckResourceLeak();
#endif

    return EXIT_SUCCESS;
}


#if DIST

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return GuardedMain();
}

#else

int main(int argc, char** argv)
{
    return GuardedMain();
}

#endif // DIST