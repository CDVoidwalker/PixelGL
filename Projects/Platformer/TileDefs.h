#ifndef TILEDEFS_H
#define TILEDEFS_H
#include "Interfaces.h"

using namespace PixelGL;

namespace TileColors
{
    const Pixel GROUND(67, 0, 0);
    const Pixel BRICK(126, 39, 0);
    const Pixel QM_BLOCK(255, 255, 0);
    const Pixel PIPE(0, 255, 0);
    const Pixel FLAGPOLE(0, 255, 210);
    const Pixel BLOCK(184, 0, 255);
    const Pixel SKY(255, 255, 255);
} // namespace TileColors

class Ground : public ITileDrawable, public ITilemapCollidable
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) override;
};

class Brick : public ITileDrawable, public ITilemapCollidable // todo bump
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) override;
};

class QM_Block : public ITileDrawable, public ITilemapCollidable // todo coin
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) override;
};

class Pipe : public ITileDrawable, public ITilemapCollidable
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) override;
};

class Flagpole : public ITileDrawable, public ITilemapCollidable // todo flagpole
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) override;
};

class Block : public ITileDrawable, public ITilemapCollidable
{
public:
    virtual void Draw(const Vector2<float> &pos, MapData mData, Engine *eng) override;
};
#endif