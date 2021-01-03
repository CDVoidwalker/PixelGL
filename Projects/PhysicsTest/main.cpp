#include "pixelgl.h"
#include "physics.h"
#include <iostream>

using namespace PixelGL;

const Pixel RED(255, 0, 0), GREEN(0,255,0), BLUE(0,0,255);

class Game : public Engine
{
    Vector2<float> middle = {320.0f, 240.0f}, point1;
    Vector2<float> halfdims = {32.0f, 32.0f};
public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};

    virtual void Start() override{
        middle = {320.0f, 240.0f};
    };

    virtual void Update(double dt) override
    {
        Clear();
        float elapsed = GetElapsedTime();
        point1 = {middle.x + (sin(elapsed) * 50.0f), middle.y + (cos(elapsed * 2) * 50.0f)};

        Rect(middle - halfdims, middle + halfdims, BLUE);
        AABB aabb = AABB(middle, halfdims), aabb2(point1, halfdims);

        HitInfo out;
        if (aabb.Intersect(aabb2, out)) 
            Rect(point1 - halfdims + out.delta, point1 + halfdims + out.delta, RED);

        Rect(point1 - halfdims, point1 + halfdims, GREEN);
    }
};

int main(int argc, char **argv)
{
    Game g = Game(640, 480, 1, 60, false);
    g.Run();
    return 0;
}