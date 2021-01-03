#ifndef INTERFACES_H
#define INTERFACES_H
#include "vector2.h"
#include "engine.h"

using namespace PixelGL;
class ITileDrawable;
class GameManager;

struct MapData
{
    Vector2<float> pos;
    Vector2<int> dim;
    ITileDrawable **tiles;
};

class IEntity
{
public:
    IEntity(){};
    virtual ~IEntity(){};
};

class ITileDrawable
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) = 0;
};

class IPositionable : virtual public IEntity
{
public:
    virtual Vector2<float> getPos() = 0;
    virtual void SetPos(const Vector2<float> &pos) = 0;
};

class ITilemapCollidable
{
public:
    virtual void OnCollisionEnter(const Vector2<float> &pos){};
};

class IUpdatable : virtual public IEntity
{
public:
    virtual void Update() = 0;
};

#endif