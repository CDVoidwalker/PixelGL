#ifndef INTERFACES_H
#define INTERFACES_H
#include "vector2.h"
#include "engine.h"

using namespace PixelGL;
class ITileDrawable;

struct MapData {
    Vector2<int> pos, dim;
    ITileDrawable** tiles;
};

class ITileDrawable
{
public:
    virtual void Draw(const Vector2<int>& pos,MapData mData, Engine* eng) = 0;
};

#endif