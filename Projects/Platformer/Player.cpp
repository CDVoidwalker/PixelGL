#include "Player.h"

Player::Player(const Vector2<float> &pos)
{
    this->pos = pos;
    inAir = true;
}

void Player::Jump()
{
    float force = -JUMPFORCE - (abs(velocity.x) / MAX_RUN_SPEED) * JUMPFORCE_ADD;
    force = std::max(-JUMPFORCE - JUMPFORCE_ADD, force);
    velocity.y = force;
    inAir = true;
}

void Player::Move(bool left, bool running, float dt)
{
    float a = running ? RUN_ACCEL : WALK_ACCEL;
    float max = running ? MAX_RUN_SPEED : MAX_WALK_SPEED;

    if (left)
        a = -a;

    bool dirMatch = std::signbit(velocity.x) == std::signbit(a);

    // if velocity and acceleration dir do not match
    if (!dirMatch)
    {
        a *= AIRSLIDE_FACTOR;
        a *= inAir ? FRICTION_AIR : FRICTION_GROUND;
    }

    velocity.x = Clamp(velocity.x + a * dt, -max, max);
}

AABB Player::GetAABB()
{
    return AABB{pos, {BLOCK_DIM / 2.0f, BLOCK_DIM / 2.0f}};
}

void Player::Update(float dt)
{
    GameManager *gm = GameManager::getInstance();
    std::vector<Vector2<int>> collisions;

    bool running = gm->eng->GetKeyState(GLFW_KEY_X) == HELD;
    bool jumpHeld = gm->eng->GetKeyState(GLFW_KEY_Z) == HELD;
    bool leftHeld = gm->eng->GetKeyState(GLFW_KEY_LEFT) == HELD;
    bool rightHeld = gm->eng->GetKeyState(GLFW_KEY_RIGHT) == HELD;

    if (leftHeld)
        Move(true, running, dt);
    else if (rightHeld)
        Move(false, running, dt);
    else if (velocity.x != 0)
    {
        float friction = inAir ? FRICTION_AIR : FRICTION_GROUND;
        velocity.x -= std::copysignf(1.0f, velocity.x) * friction * dt;

        if (abs(velocity.x) <= MIN_SPEED)
            velocity.x = 0;
    }
    //
    if (inAir)
        velocity.y += jumpHeld ? JUMP_GRAV * dt : GRAV * dt;
    else if (jumpHeld)
        Jump();

    // tile positions
    int ttop = (pos.y / BLOCK_DIM) - 1;
    int tbot = ttop + 2;
    int tleft = (pos.x / BLOCK_DIM) - 1;
    int tright = tleft + 2;

    vclamp(velocity.y, -7, 7);

    // X axis collision resolution
    pos.x += velocity.x;

    for (int tx = tleft; tx <= tright; tx++)
    {
        for (int ty = ttop; ty <= tbot; ty++)
        {
            AABB aabb, playerAABB = GetAABB(); //AABB(resolvedPos, Vector2<float>(BLOCK_DIM/2.0f, BLOCK_DIM/2.0f));
            HitInfo hinfo;
            if (!gm->TryGetTileAABB({tx, ty}, &aabb))
                continue;

            if (!aabb.Intersect(playerAABB, hinfo))
                continue;

            pos.x += hinfo.delta.x;
        }
    }

    // Y axis collision resolution
    pos.y += velocity.y;

    for (int tx = tleft; tx <= tright; tx++)
    {
        for (int ty = ttop; ty <= tbot; ty++)
        {
            AABB aabb, playerAABB = GetAABB();
            HitInfo hinfo;
            if (!gm->TryGetTileAABB({tx, ty}, &aabb))
                continue;

            if (!aabb.Intersect(playerAABB, hinfo))
                continue;

            pos.y += hinfo.delta.y;
            if (hinfo.delta.y < 0)
                inAir = false;
        }
    }

    // move camera centered on player by x axis
    gm->SetPos({pos.x - (WIDTH / 2) + (BLOCK_DIM / 2), gm->getPos().y});

    Vector2<float> drawPos = gm->WorldToCameraPos(pos);
    gm->eng->Rect(drawPos, Vector2<int>(drawPos.x + BLOCK_DIM, drawPos.y + BLOCK_DIM), {255, 0, 0, 255});
}

Vector2<float> Player::getPos()
{
    return pos;
}

void Player::SetPos(const Vector2<float> &pos)
{
    this->pos = pos;
}