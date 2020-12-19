#ifndef PIXELGL_PIXEL
#define PIXELGL_PIXEL
namespace PixelGL
{
    struct Pixel
    {
        unsigned char r, g, b;
        Pixel(unsigned char r, unsigned char g, unsigned char b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        Pixel(){};

        bool operator==(const Pixel &other)
        {
            return r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(const Pixel &other)
        {
            return !(*this == other);
        }
    };
} // namespace PixelGL
#endif