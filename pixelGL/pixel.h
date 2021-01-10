#ifndef PIXELGL_PIXEL
#define PIXELGL_PIXEL
#include <cstdint>

namespace PixelGL
{
    union Pixel
    {
        uint32_t rgba;
        uint8_t values[4];

        constexpr Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : values{r, g, b, a} {};
        constexpr Pixel() : rgba(0) {};
        constexpr Pixel(uint32_t rgba) : rgba(rgba) {};

        constexpr bool operator==(const Pixel &other)
        {
            return rgba == other.rgba;
        }

        constexpr bool operator!=(const Pixel &other)
        {
            return rgba != other.rgba;
        }
    };
} // namespace PixelGL
#endif