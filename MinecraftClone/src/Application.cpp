#include "Precompiled.h"

#include "Managers/ManagerHeader.h"

int main()
{
    Logger::Init();
    LOG_WARN("Initialized Log!");

    int a = 5;
    LOG_INFO("Hello! Var={0}", a);

    IntVector2D defaultScreenSize(1280, 720);

    WindowManager::CreateInstance(defaultScreenSize);
    WindowManager& WM = WindowManager::GetInstance();

    WM.Show();
    WM.CenterWindow();

    while (WM.Tick())
    {
    }

    WindowManager::DeleteInstance();

    return EXIT_SUCCESS;
}