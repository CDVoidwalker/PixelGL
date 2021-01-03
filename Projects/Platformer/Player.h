#ifndef PLAYER_H
#define PLAYER_H
#include "Interfaces.h"
#include "vector2.h"
#include "physics.h"
#include "CompileDefines.h"
#include "engine.h"
#include "macros.h"
#include "GameManager.h"
#include <vector>

class Player : virtual public IUpdatable, virtual public IPositionable {
    private:
    Vector2<float> pos, velocity;
    bool inAir = false;
    void Move(bool left, bool running);
    void Jump();
    AABB GetAABB();

    public:
    Player(const Vector2<float>& pos);
    virtual void Update() override;
    virtual Vector2<float> getPos() override;
    virtual void SetPos(const Vector2<float>& pos) override;
};

#endif