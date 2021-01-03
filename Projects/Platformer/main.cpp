#include "pixelgl.h"
#include "CompileDefines.h"
#include "GameManager.h"
#include "Player.h"
#include <iostream>

using namespace PixelGL;

class Game : public Engine
{
public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    virtual void Start() override{
        GameManager::getInstance()->Init(this);
        GameManager::getInstance()->AddEntity(new Player({BLOCK_DIM * 4,BLOCK_DIM * 13 + 8}));
    };

    virtual void Update(double dt) override
    {
        GameManager::getInstance()->Loop();
    }
};

int main(int argc, char **argv)
{
    Game g = Game(WIDTH, HEIGHT, PIXEL_SIZE, FPS, false);
    g.Run();
    return 0;
}