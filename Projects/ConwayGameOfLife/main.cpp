#include "pixelgl.h"
#include <iostream>
#include <vector>

using namespace PixelGL;
const Pixel LIVING_CELL(0, 255, 0);
const Pixel DEAD_CELL(0,0,0);
const int WIDTH = 64, HEIGHT = 32, PIXELSIZE = 24;

struct Change {
    Vector2<int> pos;
    bool alive;
};


class Game : public Engine
{
public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    std::vector<Change> simulationDelta;
    bool simulate = false;

    int livingNeighbours(const Vector2<int>& pos) {
        Vector2<int> offsets[] = { {-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1} };
        int livingCells = 0;
        for (auto offset : offsets) {
            Vector2<int> sum = offset + pos;
            if (!isInBounds(sum))
                continue;

            if (GetPixel(sum) == LIVING_CELL)
                livingCells++;
        }
        
        return livingCells;
    }

    virtual void Start() override{};

    virtual void Update(double dt) override
    {
        if (GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == KeyState::HELD){
            Vector2<int> pos = MousePixelPosition();
            SetPixel(pos, LIVING_CELL);
        } else if (GetMouseButtonState(GLFW_MOUSE_BUTTON_2) == KeyState::HELD) {
            Vector2<int> pos = MousePixelPosition();
            SetPixel(pos, DEAD_CELL);
        }

        if (GetKeyState(GLFW_KEY_SPACE) == KeyState::PRESSED)
            simulate = !simulate;

        if (!simulate)
            return;

        for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++) {
            Pixel celltype = GetPixelUnsafe({x,y});
            int aliveNeighbours = livingNeighbours({x,y});

            if (celltype == DEAD_CELL && aliveNeighbours == 3)
                simulationDelta.push_back(Change{.pos = {x,y}, .alive = true});
            else if (celltype == LIVING_CELL && !(aliveNeighbours == 2 || aliveNeighbours == 3)) 
                simulationDelta.push_back(Change{.pos = {x,y}, .alive = false});
        }

        for (auto change : simulationDelta) {
            SetPixel(change.pos, change.alive ? LIVING_CELL : DEAD_CELL);
        }

        simulationDelta.clear();
    }
};

int main(int argc, char **argv)
{
    Game g = Game(WIDTH, HEIGHT, PIXELSIZE, 15, false);
    g.Run();
    return 0;
}