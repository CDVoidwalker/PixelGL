#ifndef PLAYER_H
#define PLAYER_H
#include "Interfaces.h"
#include "CompileDefines.h"
#include "GameManager.h"
#include "vector2.h"
#include "physics.h"
#include "engine.h"
#include "macros.h"
#include <vector>

class Player : virtual public IUpdatable, virtual public IPositionable {
    private:
    Vector2<float> pos, velocity;
    bool inAir = false;
    void Move(bool left, bool running, float dt);
    void Jump();
    AABB GetAABB();

    public:
    Player(const Vector2<float>& pos);
    virtual void Update(float dt) override;
    virtual Vector2<float> getPos() override;
    virtual void SetPos(const Vector2<float>& pos) override;
};

#endif