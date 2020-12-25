#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include "Interfaces.h"
#include "CompileDefines.h"
#include "vector2.h"
#include "image.h"
#include "engine.h"
#include "TileDefs.h"

using namespace PixelGL;

class MapManager
{
    Vector2<int> pos, dim;
    ITileDrawable** tiles;

public:
    MapManager();
    void Render(Engine *eng);
    void Move(Vector2<int> pixels);
};
#endif