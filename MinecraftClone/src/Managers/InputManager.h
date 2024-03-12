#pragma once

enum class eInputButton : UINT32
{
    Space = 0,
    W,
    A,
    S,
    D,
    Q,
    E,
    F,
    MOUSE_L,
    MOUSE_R,
    LastButton
};

class InputManager final // #TODO implement mouse scroll to zoom in/out
{
public:
    static InputManager& GetInstance()
    {
        static InputManager inputManager;
        return inputManager;
    }

    inline const IntVector2D& GetMouseRelativeChange() const { return mMouseRelativeChange; };
    void OnMouseMove(const int mouseX, const int mouseY);

    bool IsPressed(eInputButton IneInputButton) const; // #TODO Fix clicking multiple times
    bool IsPressing(eInputButton IneInputButton) const;
    bool IsReleased(eInputButton IneInputButton) const;
    inline bool IsInputLock() const { return mbLockInput; }

    void UpdateInput();
    void SetInputButton(eInputButton IneInputButton, std::function<bool()> InPressedFn);
    inline void ToggleInputLock() { mbLockInput = !mbLockInput; }

private:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager& rhs) = delete;
    InputManager operator=(const InputManager& rhs) = delete; 

    bool mbMouseMoved = false;
    bool mbLockInput = true;
    IntVector2D mMouseRelativeChange;

    std::array<std::function<bool()>, static_cast<size_t>(eInputButton::LastButton)> PressedButtonMap = { 0, };
    std::array<bool, static_cast<size_t>(eInputButton::LastButton)> PrevButtonStatus = { 0, };
};