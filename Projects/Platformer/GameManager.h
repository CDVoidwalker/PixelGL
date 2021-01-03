#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "Interfaces.h"
#include "CompileDefines.h"
#include "TileDefs.h"

#include "physics.h"
#include "vector2.h"
#include "image.h"
#include "engine.h"

#include <vector>
#include <functional>

using namespace PixelGL;

class GameManager : public IPositionable
{
    static GameManager* _instance;
    Vector2<float> pos;
    Vector2<int> dim;
    ITileDrawable** tiles;
    std::vector<IEntity*> entities;
    std::vector<std::function<void()>> frameEndTasks;
    void EntityLoop();
    void MapRender();
    void FrameEndTaskLoop();

public:
    Engine* eng = nullptr;
    static GameManager* getInstance();
    void Init(Engine *eng, const Vector2<float>& pos = {0,BLOCK_DIM});
    void Loop();
    void AddFrameEndTask(std::function<void()> task);
    void AddEntity(IEntity* entity);

    Vector2<float> WorldToCameraPos(const Vector2<float>& pos);

    bool TryGetTileAABB(const Vector2<int>& tilePos, AABB* aabb);

    virtual Vector2<float> getPos() override;
    virtual void SetPos(const Vector2<float>& pos) override;
};
#endif