#include "Player.h"

Player::Player(const Vector2<float> &pos)
{
    this->pos = pos;
    inAir = true;
}

void Player::Jump()
{
    velocity.y -= 9;
    inAir = true;
}

void Player::Move(bool left, bool running)
{
    int speed = 1;
    speed = left ? -speed : speed;
    int max = running ? MAX_SPEED : MAX_SPEED / 2;

    velocity.x += speed;
    vclamp(velocity.x, -max, max);
}

AABB Player::GetAABB() {
    return AABB {pos, {BLOCK_DIM / 2.0f, BLOCK_DIM / 2.0f}};
}

void Player::Update()
{
    GameManager *gm = GameManager::getInstance();
    std::vector<Vector2<int>> collisions;

    bool running = gm->eng->GetKeyState(GLFW_KEY_X) == HELD;
    bool jumpHeld = gm->eng->GetKeyState(GLFW_KEY_Z) == HELD;
    bool leftHeld = gm->eng->GetKeyState(GLFW_KEY_LEFT) == HELD;
    bool rightHeld = gm->eng->GetKeyState(GLFW_KEY_RIGHT) == HELD;

    if (leftHeld)
        Move(true, running);
    else if (rightHeld)
        Move(false, running);
    else if (velocity.x != 0)
        velocity.x = velocity.x > 0 ? velocity.x - 1 : velocity.x + 1;
// 
    if (inAir)
        velocity.y = jumpHeld ? velocity.y + 1 : velocity.y + 2;
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
    
    for (int tx = tleft; tx <= tright; tx++) {
        for (int ty = ttop; ty <= tbot; ty++) {
            AABB aabb, playerAABB = GetAABB();//AABB(resolvedPos, Vector2<float>(BLOCK_DIM/2.0f, BLOCK_DIM/2.0f));
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

    for (int tx = tleft; tx <= tright; tx++) {
        for (int ty = ttop; ty <= tbot; ty++) {
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
    gm->eng->Rect(drawPos, Vector2<int>(drawPos.x + BLOCK_DIM, drawPos.y + BLOCK_DIM), {255, 0, 0});
}

Vector2<float> Player::getPos()
{
    return pos;
}

void Player::SetPos(const Vector2<float> &pos)
{
    this->pos = pos;
}