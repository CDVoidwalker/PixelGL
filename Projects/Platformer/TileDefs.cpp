#include "TileDefs.h"
#include "AssetManager.h"
#include "engine.h"
#include "vector2.h"

void Ground::Draw(const Vector2<int> &pos, MapData mData, Engine *eng)
{
    AssetManager::getInstance()->tileImages.Ground.Draw(pos, eng);
}

void Brick::Draw(const Vector2<int> &pos, MapData mData, Engine *eng)
{
    AssetManager::getInstance()->tileImages.Brick.Draw(pos, eng);
}

void QM_Block::Draw(const Vector2<int> &pos, MapData mData, Engine *eng)
{
    int anim = eng->GetFrameCount() / 10 % 5;
    switch (anim)
    {
    case 0:
    case 4:
        AssetManager::getInstance()->tileImages.QM_Block.Draw(pos, eng);
        break;
    case 1:
    case 3:
        AssetManager::getInstance()->tileImages.QM_Block2.Draw(pos, eng);
        break;
    case 2:
        AssetManager::getInstance()->tileImages.QM_Block3.Draw(pos, eng);
        break;
    }
}

void Flagpole::Draw(const Vector2<int> &pos, MapData mData, Engine *eng)
{
    if (dynamic_cast<Flagpole *>(mData.tiles[mData.pos.x + ((mData.pos.y - 1) * mData.dim.x)]))
        AssetManager::getInstance()->tileImages.Flagpole.Draw(pos, eng);
    else
        AssetManager::getInstance()->tileImages.Flagpole_top.Draw(pos, eng);
}

void Pipe::Draw(const Vector2<int> &pos, MapData mData, Engine *eng)
{
    // TODO cleanup, typeid should be faster
    // and it is kinda ugly either way
    const Vector2<int> offsets[] = {{0, -1}, {-1, 0}};

    Vector2<int> p = mData.pos + offsets[0];
    int idx = p.x + (p.y * mData.dim.x);
    bool free_left = p.x < 0 || dynamic_cast<Pipe *>(mData.tiles[idx]);

    p = mData.pos + offsets[1];
    idx = p.x + (p.y * mData.dim.x);
    bool free_top = p.y < 0 || dynamic_cast<Pipe *>(mData.tiles[idx]);

    if (free_left)
    {
        if (free_top)
            AssetManager::getInstance()->tileImages.Pipe_r.Draw(pos, eng);
        else
            AssetManager::getInstance()->tileImages.Pipe_l.Draw(pos, eng);
    }
    else
    {
        if (free_top)
            AssetManager::getInstance()->tileImages.Pipe_tr.Draw(pos, eng);
        else
            AssetManager::getInstance()->tileImages.Pipe_tl.Draw(pos, eng);
    }
}

void Block::Draw(const Vector2<int> &pos, MapData mData, Engine *eng)
{
    AssetManager::getInstance()->tileImages.Block.Draw(pos, eng);
}
