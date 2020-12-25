#include "CompileDefines.h"
#include "MapManager.h"
#include "pixelgl.h"
#include <iostream>

using namespace PixelGL;

class Game : public Engine
{
    MapManager mm;
public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    virtual void Start() override{};

    virtual void Update(double dt) override
    {
        if (GetKeyState(GLFW_KEY_LEFT) == HELD)
            mm.Move({-3,0});
        if (GetKeyState(GLFW_KEY_RIGHT) == HELD)
            mm.Move({3,0});
        if (GetKeyState(GLFW_KEY_UP) == HELD)
            mm.Move({0,-3});
        if (GetKeyState(GLFW_KEY_DOWN) == HELD)
            mm.Move({0,3});
        mm.Render(this);
    }
};

int main(int argc, char **argv)
{
    Game g = Game(WIDTH, HEIGHT, PIXEL_SIZE, FPS, false);
    g.Run();
    return 0;
}