/*
 This is an example where OOP is a bad design choice. Doing this without any kind of polymorphism
 and pointer management would make it much easier and perform way better. Thinking back, better
 approach would be achieving delta compression via storing particle functions as lambdas with
 some sort of structure that keeps them with their parameters, it would be faster but even
 harder to implement and make code even less readable.
*/
#include "pixelgl.h"
#include <iostream>
#include <typeinfo>
#include <map>
#include <vector>

const int WIDTH = 32, HEIGHT = 32, PIXELSIZE = 16;
using namespace PixelGL;

struct IParticle
{
    bool needsUpdate = true;
    virtual const Pixel &GetColor() = 0;
    virtual void SimulationStep(Vector2<int> pos, Engine *engine) = 0;
};

struct Simulation
{
    std::map<Vector2<int>, IParticle *> currentGen;
    std::map<Vector2<int>, IParticle *> nextGen;
    bool running = false;

    inline bool Empty(const Vector2<int> &pos) { return currentGen[pos] == nullptr; }
    inline bool UnReserved(const Vector2<int> &pos) { return nextGen[pos] == nullptr; }
};

Simulation simulation;

#pragma region ParticleDeclarations
struct Sand : public IParticle
{
    static Pixel color;
    virtual void SimulationStep(Vector2<int> pos, Engine *engine) override;
    virtual const Pixel &GetColor() override;
};

struct Water : public IParticle
{
    static Pixel color;
    virtual void SimulationStep(Vector2<int> pos, Engine *engine) override;
    virtual const Pixel &GetColor() override;
};
#pragma endregion ParticleDeclarations

#pragma region ParticleDefinitions
void Sand::SimulationStep(Vector2<int> pos, Engine *engine)
{
    // offsets in order : bottom, bottom left, bottom right
    const Vector2<int> offset[] = {{0, -1}, {-1, -1}, {1, -1}};
    for (int i = 0; i < 3; i++)
    {
        auto p = pos + offset[i];

        // if 'p' is not in bounds, skip iteration
        if (!engine->isInBounds(p))
            continue;

        // if there is something at the 'p' position in current gen
        // or if there is something occupying next gen of that pos
        if (simulation.Empty(p) && simulation.UnReserved(p))
        {
            // place Sand in the 'p' position of next generation and return
            simulation.nextGen[p] = new Sand();
            return;
        }
        // if !empty & water & (unreserved | reserved & water)
        else if (
            !simulation.Empty(p) &&
            typeid(*simulation.currentGen[p]) == typeid(Water) &&
            (simulation.UnReserved(p) ||
             (!simulation.UnReserved(p) &&
              typeid(*simulation.nextGen[p]) == typeid(Water))))
        {
            // Set water to pre offsetted one and sand to the 'p'
            simulation.nextGen[p - offset[i]] = new Water();
            simulation.nextGen[p] = new Sand();

            // set water at that spot not to have update
            simulation.currentGen[p]->needsUpdate = false;
            vlogf("Swapping with water at [%i,%i]", p.x, p.y);

            return;
        }
    }

    // if it could not find any position during the 'for' loop then
    // place sand in current position at next generation
    simulation.nextGen[pos] = new Sand();
}

const Pixel &Sand::GetColor()
{
    return color;
}

void Water::SimulationStep(Vector2<int> pos, Engine *engine)
{
    // bottom, bottom left, bottom right, left, right
    const Vector2<int> offset[] = {{0, -1}, {-1, -1}, {1, -1}, {-1, 0}, {1, 0}};
    for (int i = 0; i < 3; i++)
    {
        auto p = pos + offset[i];
        if (!engine->isInBounds(p))
            continue;

        if (!simulation.Empty(p))
            continue;

        if (!simulation.UnReserved(p))
            continue;

        simulation.nextGen[p] = new Water();
        return;
    }

    // randomly pick between left and right check first
    if (rand() % 2 == 0)
    {
        if (engine->isInBounds(pos + offset[3]) &&
            simulation.Empty(pos + offset[3]) &&    // current position empty
            simulation.UnReserved(pos + offset[3])) // no other particle occupied that
        {
            simulation.nextGen[pos + offset[3]] = new Water();
            return;
        }
    }
    else if (engine->isInBounds(pos + offset[4]) &&
             simulation.Empty(pos + offset[4]) &&    // current position empty
             simulation.UnReserved(pos + offset[4])) // no other particle occupied that
    {
        simulation.nextGen[pos + offset[4]] = new Water();
        return;
    }
    simulation.nextGen[pos] = new Water();
}

const Pixel &Water::GetColor()
{
    return color;
}
#pragma endregion ParticleDefinitions

#pragma region ParticleColors
Pixel Sand::color = {255, 255, 0};
Pixel Water::color = {102, 152, 255};
#pragma endregion ParticleColors

class Game : public Engine
{
public:
    Game(int width, int height, int pixelsize, double fps, int fullscreen) : Engine(width, height, pixelsize, fps, fullscreen){};
    int particlePicker = 0;
    virtual void Start() override{};

    virtual void Update(double dt) override
    {
        PixelGL::Vector2<int> pos = MousePixelPosition();
        if (GetKeyState(GLFW_KEY_1) == KeyState::PRESSED)
            particlePicker = 0;
        if (GetKeyState(GLFW_KEY_2) == KeyState::PRESSED)
            particlePicker = 1;

        if (GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == KeyState::HELD)
        {
            if (simulation.currentGen[pos] != nullptr)
            {
                delete simulation.currentGen[pos];
                simulation.currentGen[pos] = nullptr;
                SetPixel(pos, {0, 0, 0});
            }

            IParticle *pptr;
            switch (particlePicker)
            {
            case 0:
                pptr = new Sand();
                break;
            case 1:
                pptr = new Water();
                break;
            }

            simulation.currentGen[pos] = pptr;
            SetPixel(pos, pptr->GetColor());
        }

        else if (GetMouseButtonState(GLFW_MOUSE_BUTTON_2) == KeyState::HELD)
        {
            if (simulation.currentGen[pos] != nullptr)
            {
                delete simulation.currentGen[pos];
                simulation.currentGen[pos] = nullptr;
                SetPixel(pos, {0, 0, 0});
            }
        }

        if (GetKeyState(GLFW_KEY_SPACE) == KeyState::PRESSED)
            simulation.running = !simulation.running;

        if (!simulation.running)
            return;

        // Clear drawing area
        Clear();

        // Create new state out of current sim.simulation and draw current
        for (auto cg : simulation.currentGen)
        {
            if (cg.second == nullptr)
                continue;

            if (cg.second->needsUpdate)
            {
                cg.second->SimulationStep(cg.first, this);
                SetPixel(cg.first, cg.second->GetColor());
            }
        }

        // clear sim.simulation
        for (auto cg : simulation.currentGen)
        {
            if (cg.second != nullptr)
                delete cg.second;
        }

        simulation.currentGen.clear();

        // Push next generation into current and clear next generation
        for (auto ng : simulation.nextGen)
        {
            if (ng.second != nullptr) 
                simulation.currentGen[ng.first] = ng.second; // there is no memory leak here, we pass pointers to first gen
        }

        // Clear out next state
        // note that all data under pointers is being handled in sim.simulation loops
        simulation.nextGen.clear();
    }
};

int main(int argc, char **argv)
{
    Game g = Game(WIDTH, HEIGHT, PIXELSIZE, 25, false);
    g.Run();
    return 0;
}