#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Interfaces.h"
#include "CompileDefines.h"
#include "vector2.h"
#include "image.h"
#include "engine.h"
#include "TileDefs.h"

using namespace PixelGL;

class GameManager : public IPositionable
{
    Vector2<int> pos, dim;
    ITileDrawable** tiles;

public:
    GameManager(const Vector2<int>& pos = {0,BLOCK_DIM});
    void Render(Engine *eng);
    void Move(Vector2<int> pixels);
    virtual Vector2<int> getPos() override;
};
#endif