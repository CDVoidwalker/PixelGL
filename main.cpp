#include "pixelgl.h"
#include <iostream>

using namespace PixelGL;

const Pixel RED(255, 0, 0);

class Game : public Engine
{
    Vector2<int> start;
    Vector2<int> end;
    bool draw = false;
    int mode = 0;

public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    virtual void Start() override{
        Image i("./assets/testmap.png");
        Vector2<int> dim = i.getDimensions();
        vlogf("Image dimensions are [%i,%i]", dim.x, dim.y);

        for (int ix = 0; ix < i.getDimensions().x; ix++) {
            for (int iy = 0; iy < i.getDimensions().y; iy++) {
                SetPixel({ix, iy}, i.getPixel({ix, iy}));
            }
        }
    };

    virtual void Update(double dt) override
    {
        //Clear();
        if (GetKeyState(GLFW_KEY_1) == PRESSED)
            mode = 0;
        if (GetKeyState(GLFW_KEY_2) == PRESSED)
            mode = 1;
        if (GetKeyState(GLFW_KEY_3) == PRESSED)
            mode = 2;

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