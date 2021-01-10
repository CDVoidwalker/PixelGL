#ifndef PIXELGL_VECTOR2
#define PIXELGL_VECTOR2
#include <math.h>

namespace PixelGL
{
    template <typename T>
    struct Vector2
    {
        T x, y;

        Vector2() {this->x = T(), this->y = T()};
        constexpr Vector2() : x(T()), y(T()) {};
        constexpr Vector2(T x, T y) : x(x), y(y) {};

        constexpr Vector2<T> operator+(const Vector2<T> &other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        constexpr Vector2<T> operator-(const Vector2<T> &other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        constexpr Vector2<T> operator*(const T &scale) const
        {
            return Vector2(x * scale, y * scale);
        }

        constexpr Vector2<T> &operator+=(const Vector2<T> &other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        constexpr Vector2<T> &operator-=(const Vector2<T> &other)
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        constexpr bool operator==(const Vector2<T> &other) const
        {
            return x == other.x && y == other.y;
        }

        constexpr bool operator!=(const Vector2<T> &other) const
        {
            return !(*this == other);
        }

        // for sorting
        constexpr bool operator<(const Vector2<T> &other) const
        {
            return x < other.x || (x == other.x && y < other.y);
        }

        constexpr T length() const
        {
            return std::sqrt(x * x + y * y);
        }

        constexpr void normalize() {
            T lenreverse = 1.0 / length();
            this->x *= lenreverse;
            this->y *= lenreverse;
        }

        constexpr void rotate(float rad) {
            float s = sin(rad), c = cos(rad);
            this->x = x * c - y * s;
            this->y = x * s + y * c;
        }

        constexpr operator Vector2<int>() const
        {
            return Vector2<int>((int)x, (int)y);
        }

        constexpr operator Vector2<float>() const
        {
            return Vector2<float>((float)x, (float)y);
        }

        constexpr operator Vector2<double>() const
        {
            return Vector2<double>((float)x, (float)y);
        }

        constexpr operator Vector2<long>() const
        {
            return Vector2<long>((long)x, (long)y);
        }
    };
} // namespace PixelGL
#endif