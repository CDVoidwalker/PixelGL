#ifndef PIXELGL_PHYSICS
#define PIXELGL_PHYSICS
#include "vector2.h"
#include "macros.h"
#include "functions.h"

namespace PixelGL
{
    constexpr float EPSILON = 1e-8;

    struct HitInfo
    {
        Vector2<float> position, normal, delta;
        float time;
    };

    struct Sweep
    {
        HitInfo hit;
        Vector2<float> position;
        float time;
    };

    class AABB
    {
        Vector2<float> position, halfDim;

    public:
        AABB(const Vector2<float> &position, const Vector2<float> &dimensions)
        {
            this->position = position;
            this->halfDim = dimensions;
        }

        AABB() {}
        bool Intersect(const AABB &other, HitInfo &hit)
        {
            float dx = other.position.x - this->position.x;
            float px = (other.halfDim.x + this->halfDim.x) - abs(dx);
            if (px <= 0)
            {
                return false;
            }

            float dy = other.position.y - this->position.y;
            float py = (other.halfDim.y + this->halfDim.y) - abs(dy);
            if (py <= 0)
            {
                return false;
            }

            if (px < py)
            {
                float sx = std::copysignf(1.0f, dx);
                hit.delta.x = px * sx;
                hit.normal.x = sx;
                hit.position.x = this->position.x + (this->halfDim.x * sx);
                hit.position.y = other.position.y;
            }
            else
            {
                float sy = std::copysignf(1.0f, dy);
                hit.delta.y = py * sy;
                hit.normal.y = sy;
                hit.position.x = other.position.x;
                hit.position.y = this->position.y + (this->halfDim.y * sy);
            }
            return true;
        }
        bool IntersectPoint(const Vector2<float> &point, HitInfo &hit)
        {
            float dx = point.x - this->position.x;
            float px = this->halfDim.x - abs(dx);
            if (px <= 0)
            {
                return false;
            }

            float dy = point.y - this->position.y;
            float py = this->halfDim.y - abs(dy);
            if (py <= 0)
            {
                return false;
            }

            if (px < py)
            {
                float sx = std::copysignf(1.0f, dx);
                hit.delta.x = px * sx;
                hit.normal.x = sx;
                hit.position.x = this->position.x + (this->halfDim.x * sx);
                hit.position.y = point.y;
            }
            else
            {
                float sy = std::copysignf(1.0f, dy);
                hit.delta.y = py * sy;
                hit.normal.y = sy;
                hit.position.x = point.x;
                hit.position.y = this->position.y + (this->halfDim.y * sy);
            }
            return true;
        }
        bool IntersectSegment(const Vector2<float> &pos, const Vector2<float> &delta, HitInfo &hit, float paddingX = 0,
                              float paddingY = 0)
        {
            float scaleX = 1.0f / delta.x;
            float scaleY = 1.0f / delta.y;
            float signX = std::copysignf(1.0f, scaleX);
            float signY = std::copysignf(1.0f, scaleY);
            float nearTimeX = (this->position.x - signX * (this->halfDim.x + paddingX) - pos.x) * scaleX;
            float nearTimeY = (this->position.y - signY * (this->halfDim.y + paddingY) - pos.y) * scaleY;
            float farTimeX = (this->position.x + signX * (this->halfDim.x + paddingX) - pos.x) * scaleX;
            float farTimeY = (this->position.y + signY * (this->halfDim.y + paddingY) - pos.y) * scaleY;
            if (nearTimeX > farTimeY || nearTimeY > farTimeX)
            {
                return false;
            }
            float nearTime = nearTimeX > nearTimeY ? nearTimeX : nearTimeY;
            float farTime = farTimeX < farTimeY ? farTimeX : farTimeY;
            if (nearTime >= 1 || farTime <= 0)
            {
                return false;
            }
            hit.time = Clamp(nearTime, 0.0f, 1.0f);
            if (nearTimeX > nearTimeY)
            {
                hit.normal.x = -signX;
                hit.normal.y = 0;
            }
            else
            {
                hit.normal.x = 0;
                hit.normal.y = -signY;
            }
            hit.delta.x = (1.0 - hit.time) * -delta.x;
            hit.delta.y = (1.0 - hit.time) * -delta.y;
            hit.position.x = pos.x + delta.x * hit.time;
            hit.position.y = pos.y + delta.y * hit.time;
            return true;
        }
        bool SweepAABB(const AABB &box, const Vector2<float> &delta, Sweep &sweep)
        {
            if (delta.x == 0 && delta.y == 0)
            {
                sweep.position.x = box.position.x;
                sweep.position.y = box.position.y;
                bool swphit = this->Intersect(box, sweep.hit);
                sweep.time = swphit ? (sweep.hit.time = 0) : 1;
                return false;
            }
            bool swphit2 = this->IntersectSegment(box.position, delta, sweep.hit, box.halfDim.x, box.halfDim.y);
            if (swphit2)
            {
                sweep.time = Clamp(sweep.hit.time - EPSILON, 0.0f, 1.0f);
                sweep.position.x = box.position.x + delta.x * sweep.time;
                sweep.position.y = box.position.y + delta.y * sweep.time;
                Vector2<float> direction = delta;
                direction.normalize();
                sweep.hit.position.x = Clamp(
                    sweep.hit.position.x + direction.x * box.halfDim.x,
                    this->position.x - this->halfDim.x, this->position.x + this->halfDim.x);
                sweep.hit.position.y = Clamp(
                    sweep.hit.position.y + direction.y * box.halfDim.y,
                    this->position.y - this->halfDim.y, this->position.y + this->halfDim.y);
            }
            else
            {
                sweep.position.x = box.position.x + delta.x;
                sweep.position.y = box.position.y + delta.y;
                sweep.time = 1;
            }
            return true;
        }
    };
} // namespace PixelGL
#endif