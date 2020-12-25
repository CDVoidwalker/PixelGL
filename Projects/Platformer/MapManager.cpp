#include "MapManager.h"
using namespace PixelGL;

MapManager::MapManager()
{
    Image map = Image("./assets/platformer/1-1.png");
    dim = map.getDimensions();
    tiles = new ITileDrawable *[dim.x * dim.y];
    vlogf("Map dimensions [%i, %i]", dim.x, dim.y);

    for (int x = 0; x < dim.x; x++)
        for (int y = 0; y < dim.y; y++)
        {
            auto pos = x + (y * dim.x);
            auto col = map.getPixel({x, y});
            if (col == TileColors::GROUND)
                tiles[pos] = new Ground();
            else if (col == TileColors::BRICK)
                tiles[pos] = new Brick();
            else if (col == TileColors::FLAGPOLE)
                tiles[pos] = new Flagpole();
            else if (col == TileColors::BLOCK)
                tiles[pos] = new Block();
            else if (col == TileColors::PIPE)
                tiles[pos] = new Pipe();
            else if (col == TileColors::QM_BLOCK)
                tiles[pos] = new QM_Block();
            else
                tiles[pos] = nullptr;
        }
}

void MapManager::Render(Engine *eng)
{
    eng->FillRect({0, 0}, {WIDTH, HEIGHT}, SKY_COLOR);
    MapData mData;
    mData.dim = dim;
    mData.pos = pos;
    mData.tiles = tiles;
    Vector2<int> tilePos{pos.x / BLOCK_DIM, pos.y / BLOCK_DIM};
    for (int x = tilePos.x; x <= tilePos.x + (WIDTH / BLOCK_DIM); x++)
    {
        for (int y = tilePos.y; y <= tilePos.y + (HEIGHT / BLOCK_DIM); y++)
        {
            if (x < 0 || x >= dim.x)
                continue;

            if (y < 0 || y >= dim.y)
                continue;

            ITileDrawable *drawable = tiles[x + (y * dim.x)];
            if (drawable == nullptr)
                continue;

            mData.pos = {x,y};
            drawable->Draw({(x * BLOCK_DIM) - pos.x, (y * BLOCK_DIM) - pos.y},mData, eng);
        }
    }
}

void MapManager::Move(Vector2<int> pixels)
{
    pos += pixels;
}
