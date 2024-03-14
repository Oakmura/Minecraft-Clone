#include "Precompiled.h"

#include "InputManager.h"
#include "WindowManager.h"

void InputManager::OnMouseMove(const int mouseX, const int mouseY)
{
    static IntVector2D previousMouseCoord = { mouseX, mouseY };
    static IntVector2D center = WindowManager::GetInstance().GetRelativeScreenCenter();
    static const IntVector2D& windowScreenSize = WindowManager::GetInstance().GetScreenSize();
    static const IntVector2D& windowAbsoluteCenter = WindowManager::GetInstance().GetAbsoluteScreenCenter();

    if (mbLockInput || mbJustLocked)
    {
        mbJustLocked = false;
        return;
    }

    mbMouseMoved = true;
    
    IntVector2D curMouseCoord = { mouseX, mouseY };
    mMouseRelativeChange = curMouseCoord - previousMouseCoord;

    IntVector2D screenSize = WindowManager::GetInstance().GetScreenSize();

    int maxRadius = screenSize.mY / 3;
    IntVector2D distanceFromCenter = previousMouseCoord - center;
    float len = sqrt(static_cast<float>(distanceFromCenter.mX * distanceFromCenter.mX + distanceFromCenter.mY * distanceFromCenter.mY));
    if (len > maxRadius)
    {
        SetCursorPos(windowAbsoluteCenter.mX, windowAbsoluteCenter.mY);
        previousMouseCoord = center;
    }
    else
    {
        previousMouseCoord = curMouseCoord;
    }
}

bool InputManager::IsPressed(eInputButton InInputButton) const
{
    ASSERT(InInputButton < eInputButton::LastButton, "InputManager::IsPressed() : invalid input button");

    size_t targetIndex = static_cast<size_t>(InInputButton);
    return (!PrevButtonStatus[targetIndex] && PressedButtonMap[targetIndex]());
}

bool InputManager::IsPressing(eInputButton InInputButton) const
{
    ASSERT(InInputButton < eInputButton::LastButton, "InputManager::IsPressed() : invalid input button");

    size_t targetIndex = static_cast<size_t>(InInputButton);
    return (PrevButtonStatus[targetIndex] && PressedButtonMap[targetIndex]());
}

bool InputManager::IsReleased(eInputButton InInputButton) const
{
    ASSERT(InInputButton < eInputButton::LastButton, "InputManager::IsPressed() : invalid input button");

    size_t targetIndex = static_cast<size_t>(InInputButton);
    return (PrevButtonStatus[targetIndex] && !PressedButtonMap[targetIndex]());
}

void InputManager::UpdateInput()
{
    size_t lastIndex = static_cast<size_t>(eInputButton::LastButton);
    for (size_t bi = 0; bi < lastIndex; ++bi)
    {
        PrevButtonStatus[bi] = PressedButtonMap[bi]();
    }

    mbMouseMoved = false;
    mMouseRelativeChange = { 0, 0 };
}

void InputManager::SetInputButton(eInputButton InInputButton, std::function<bool()> InPressedFn)
{
    ASSERT(InInputButton < eInputButton::LastButton, "InputManager::IsPressed() : invalid input button");

    PressedButtonMap[static_cast<size_t>(InInputButton)] = InPressedFn;
}

void InputManager::ToggleInputLock()
{
    mbLockInput = !mbLockInput;

    if (mbLockInput)
    {
        mbJustLocked = true;
    }
}
