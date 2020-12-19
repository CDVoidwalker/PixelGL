#ifndef PIXELGL_IMAGE
#define PIXELGL_IMAGE
#include <png.h>
#include <png++/png.hpp>
#include "pixel.h"
#include "vector2.h"

namespace PixelGL
{
    class Image
    {
        Vector2<int> dimensions;
        Pixel *pixelBuffer;

    public:
        ~Image()
        {
            delete[] pixelBuffer;
        }

        // Constructor for creating images manually
        Image(const Vector2<int> &dimensions)
        {
            this->dimensions = dimensions;
        }

        // Full Image
        Image(const std::string &path)
        {
            png::image<png::rgb_pixel> img = png::image<png::rgb_pixel>(path);
            dimensions.x = img.get_width();
            dimensions.y = img.get_height();

            png::rgb_pixel *tempBuffer = new png::rgb_pixel[dimensions.x * dimensions.y];
            pixelBuffer = (Pixel *)tempBuffer;

            for (int iy = 0; iy < dimensions.y; iy++)
            {
                auto line = img.get_row(iy);
                std::copy(line.begin(), line.end(), tempBuffer + (iy * dimensions.x));
            }
        }

        // Image from slice
        Image(const std::string &path, Vector2<int> origin, Vector2<int> dim)
        {
            png::image<png::rgb_pixel> img = png::image<png::rgb_pixel>(path);
            dimensions.x = dim.x - origin.x;
            dimensions.y = dim.y - origin.y;

            int originalWidth = img.get_width();
            int endDiff = originalWidth - (origin.x + dim.x);

            png::rgb_pixel *tempBuffer = new png::rgb_pixel[dimensions.x * dimensions.y];
            pixelBuffer = (Pixel *)tempBuffer;

            for (int iy = origin.y; iy < (origin.y + dim.y); iy++)
            {
                auto line = img.get_row(iy);
                std::copy(line.begin() + origin.x, line.end() - endDiff, tempBuffer + (iy * dimensions.x));
            }
        }

        const Vector2<int> &getDimensions()
        {
            return dimensions;
        }

        Pixel &getPixel(Vector2<int> pos)
        {
            return pixelBuffer[pos.x + (pos.y * dimensions.x)];
        }
    };
} // namespace PixelGL
#endif