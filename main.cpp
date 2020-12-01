#include "pixelgl.h"
#include <iostream>

const PixelGL::Pixel PLAYER_COLOR(255, 0, 0);

class Game : public PixelGL::Engine
{
    int x = 5, y = 5;

public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    virtual void Start() override{};

    virtual void Update(double dt) override
    {
        Clear();
        if (GetKeyState(GLFW_KEY_A) == PixelGL::KeyState::HELD)
            x--;

        if (GetKeyState(GLFW_KEY_D) == PixelGL::KeyState::HELD)
            x++;

        if (GetKeyState(GLFW_KEY_W) == PixelGL::KeyState::HELD)
            y++;

        if (GetKeyState(GLFW_KEY_S) == PixelGL::KeyState::HELD)
            y--;

        if (GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == PixelGL::KeyState::PRESSED)
            std::cout << "Mouse PRESSED"
                      << "\n";

        if (GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == PixelGL::KeyState::HELD)
            std::cout << "Mouse HELD"
                      << "\n";

        if (GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == PixelGL::KeyState::RELEASED)
            std::cout << "Mouse RELEASED"
                      << "\n";

        SetPixel(x, y, PLAYER_COLOR);
    }
};

int main(int argc, char **argv)
{
    Game g = Game(128, 64, 4, 60, false);
    g.Run();
    return 0;
}