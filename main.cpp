#include "pixelgl.h"
#include <iostream>

using namespace PixelGL;

const Pixel RED(255, 0, 0);

class Game : public Engine
{
    Vector2<int> start, end, bgpos;
    bool draw = false;
    int mode = 0;

public:
    Image i;
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    virtual void Start() override{
        // Notice that without pointer this would not work
        // becasue i would go out of scope and have garbage
        // data at update.
        i = Image("./assets/testmap.png", {0,0}, {32,32});
        Vector2<int> dim = i.getDimensions();
        vlogf("Image dimensions are [%i,%i]", dim.x, dim.y);
    };

    virtual void Update(double dt) override
    {
        Clear();
        i.Draw(bgpos, this);
        if (GetKeyState(GLFW_KEY_1) == PRESSED)
            mode = 0;
        if (GetKeyState(GLFW_KEY_2) == PRESSED)
            mode = 1;
        if (GetKeyState(GLFW_KEY_3) == PRESSED)
            mode = 2;

        if (GetKeyState(GLFW_KEY_LEFT) == PRESSED)
            bgpos.x--;
        if (GetKeyState(GLFW_KEY_RIGHT) == PRESSED)
            bgpos.x++;
        if (GetKeyState(GLFW_KEY_UP) == PRESSED)
            bgpos.y--;
        if (GetKeyState(GLFW_KEY_DOWN) == PRESSED)
            bgpos.y++;

        switch (GetMouseButtonState(GLFW_MOUSE_BUTTON_1))
        {
        case PRESSED:
            start = MousePixelPosition();
            end = start;
            draw = true;
            break;
        case HELD:
            end = MousePixelPosition();
            break;
        case RELEASED:
            draw = false;
            break;
        }

        if (draw)
        {
            switch (mode)
            {
            case 0:
                Line(start, end, RED);
                break;
            case 1:
                Rect(start, end, RED);
                break;
            case 2:
                FillRect(start, end, RED);
                break;
            }
        }
    }
};

int main(int argc, char **argv)
{
    Game g = Game(32, 32, 16, 60, false);
    g.Run();
    return 0;
}