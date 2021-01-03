#include "GameManager.h"
using namespace PixelGL;

GameManager *GameManager::_instance = nullptr;

GameManager *GameManager::getInstance()
{
    if (!_instance)
        _instance = new GameManager();

    return _instance;
}

void GameManager::Init(Engine *eng, const Vector2<float> &pos)
{
    this->eng = eng;
    this->pos = pos;
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

void GameManager::MapRender()
{
    eng->FillRect({0, 0}, {WIDTH, HEIGHT}, SKY_COLOR);
    MapData mData;
    mData.dim = dim;
    mData.pos = pos;
    mData.tiles = tiles;
    Vector2<float> tilePos{pos.x / BLOCK_DIM, pos.y / BLOCK_DIM};
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

            mData.pos = Vector2<float>(x, y);
            drawable->Draw(WorldToCameraPos(Vector2<float>((x * BLOCK_DIM), (y * BLOCK_DIM))), mData, eng);
        }
    }
}

void GameManager::SetPos(const Vector2<float> &pos)
{
    this->pos = pos;
}

Vector2<float> GameManager::getPos()
{
    return pos;
}

void GameManager::EntityLoop(float dt)
{
    for (IEntity *e : entities)
    {
        IUpdatable *updatable = dynamic_cast<IUpdatable *>(e);
        if (updatable)
            updatable->Update(dt);
    }
}

void GameManager::Loop(float dt)
{
    MapRender();
    EntityLoop(dt);
    FrameEndTaskLoop();
}

void GameManager::FrameEndTaskLoop()
{
    for (auto task : frameEndTasks)
        task();

    frameEndTasks.clear();
}

void GameManager::AddFrameEndTask(std::function<void()> task)
{
    frameEndTasks.push_back(task);
}

void GameManager::AddEntity(IEntity *entity)
{
    AddFrameEndTask([this, entity]() { this->entities.push_back(entity); });
}

Vector2<float> GameManager::WorldToCameraPos(const Vector2<float> &_pos)
{
    return {_pos.x - pos.x, _pos.y - pos.y};
}

bool GameManager::TryGetTileAABB(const Vector2<int>& tilePos, AABB* aabb)
{
    if (tilePos.x < 0 || tilePos.x >= dim.x || tilePos.y < 0 || tilePos.y >= dim.y)
        return false;
        
    ITilemapCollidable* col = dynamic_cast<ITilemapCollidable*>(tiles[tilePos.x + (tilePos.y * dim.x)]);
    if (col) {
        // if (!simulated)
        //     col->OnCollisionEnter(pos);
        *aabb = AABB({tilePos.x * 16.0f, tilePos.y * 16.0f}, {BLOCK_DIM / 2.0f, BLOCK_DIM / 2.0f});
        return true;
    }

    return false;
}
