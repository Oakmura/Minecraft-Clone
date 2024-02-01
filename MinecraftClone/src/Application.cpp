#include "Precompiled.h"

#include "Managers/ManagerHeader.h"

int main()
{
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