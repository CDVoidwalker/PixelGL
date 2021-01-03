#ifndef PIXELGL_VECTOR2
#define PIXELGL_VECTOR2
#include <math.h>

namespace PixelGL
{
    template <typename T>
    struct Vector2
    {
        T x, y;
        Vector2()
        {
            x = T();
            y = T();
        }

        Vector2(T x, T y)
        {
            this->x = x;
            this->y = y;
        }

        Vector2<T> operator+(const Vector2<T> &other)
        {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2<T> operator-(const Vector2<T> &other)
        {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2<T> operator*(const T &scale)
        {
            return Vector2(x * scale, y * scale);
        }

        Vector2<T> &operator+=(const Vector2<T> &other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        Vector2<T> &operator-=(const Vector2<T> &other)
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        bool operator==(const Vector2<T> &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vector2<T> &other) const
        {
            return !(*this == other);
        }

        // for sorting
        bool operator<(const Vector2<T> &other) const
        {
            return x < other.x || (x == other.x && y < other.y);
        }

        T length() const
        {
            return std::sqrt(x * x + y * y);
        }

        void normalize() {
            T lenreverse = 1.0 / length();
            this->x *= lenreverse;
            this->y *= lenreverse;
        }

        void rotate(float rad) {
            float s = sin(rad), c = cos(rad);
            this->x = x * c - y * s;
            this->y = x * s + y * c;
        }

        operator Vector2<int>() const
        {
            return Vector2<int>((int)x, (int)y);
        }

        operator Vector2<float>() const
        {
            return Vector2<float>((float)x, (float)y);
        }

        operator Vector2<double>() const
        {
            return Vector2<double>((float)x, (float)y);
        }

        operator Vector2<long>() const
        {
            return Vector2<long>((long)x, (long)y);
        }
    };
} // namespace PixelGL
#endif