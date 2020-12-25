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

        operator Vector2<int>()
        {
            return Vector2<int>((int)x, (int)y);
        }

        operator Vector2<float>()
        {
            return Vector2<float>((float)x, (float)y);
        }

        operator Vector2<double>()
        {
            return Vector2<double>((float)x, (float)y);
        }

        operator Vector2<long>()
        {
            return Vector2<long>((long)x, (long)y);
        }
    };
} // namespace PixelGL
#endif