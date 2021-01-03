#ifndef PIXELGL_IMAGE
#define PIXELGL_IMAGE
#include <png.h>
#include <png++/png.hpp>

#include "pixel.h"
#include "vector2.h"
#include "macros.h"
#include "engine.h"

namespace PixelGL
{
    class Image
    {
        Vector2<int> dimensions;
        Pixel *pixelBuffer = nullptr;

    public:
        ~Image()
        {
            if (pixelBuffer != nullptr)
                delete[] pixelBuffer;
        }

        // Empty constructor for uninitalized image
        Image() {}

        // Copy constructor
        Image(Image const& rhs)
        {
            dimensions = rhs.dimensions;
            pixelBuffer = new Pixel[dimensions.x * dimensions.y];
            std::copy(rhs.pixelBuffer, rhs.pixelBuffer + (dimensions.x * dimensions.y), pixelBuffer);
        }

        // Constructor for creating images manually
        Image(const Vector2<int> &dimensions)
        {
            this->dimensions = dimensions;
            pixelBuffer = new Pixel[dimensions.x * dimensions.y];
        }

        // Full Image
        Image(const std::string &path)
        {
            png::image<png::rgb_pixel> img = png::image<png::rgb_pixel>(path);
            dimensions.x = img.get_width();
            dimensions.y = img.get_height();

            png::rgb_pixel *tempBuffer = new png::rgb_pixel[dimensions.x * dimensions.y];

            for (int iy = 0; iy < dimensions.y; iy++)
            {
                auto line = img.get_row(iy);
                std::copy(line.begin(), line.end(), tempBuffer + (iy * dimensions.x));
            }

            pixelBuffer = (Pixel *)tempBuffer;
        }

        // Image from slice
        Image(const std::string &path, Vector2<int> origin, Vector2<int> dim)
        {
            png::image<png::rgb_pixel> img = png::image<png::rgb_pixel>(path);
            Vector2<unsigned int> originalDim{img.get_width(), img.get_height()};

            dimensions.x = dim.x;
            dimensions.y = dim.y;

            png::rgb_pixel *tempBuffer = new png::rgb_pixel[dimensions.x * dimensions.y];

#ifdef PIXELGL_DEBUG
            auto check = origin + dim;
            if (check.x > originalDim.x || check.y > originalDim.y || check.x < 0 || check.y < 0)
                vthrowf("PixelGL::Image() Invalid dimensions for subimage origin[%i, %i] dimensions[%i,%i] excess bound of original image [%i,%i]", origin.x, origin.y, dim.x, dim.y, originalDim.x, originalDim.y);
#endif

            for (int iy = 0; iy < dimensions.y; iy++)
            {
                auto line = img.get_row(iy + origin.y);
                std::copy(line.begin() + origin.x, line.begin() + origin.x + dim.x, tempBuffer + (iy * dimensions.x));
            }

            pixelBuffer = (Pixel *)tempBuffer;
        }

        const Vector2<int> &getDimensions()
        {
            return dimensions;
        }

        Pixel &getPixel(Vector2<int> pos)
        {
            return pixelBuffer[pos.x + (pos.y * dimensions.x)];
        }

        template <typename T>
        void Draw(Vector2<T> coordinates, Engine *engine)
        {
            for (int iy = 0; iy < dimensions.y; iy++)
                engine->CopyImageLine(Vector2<int>(coordinates.x, coordinates.y + iy), dimensions.x, pixelBuffer + (iy * dimensions.x));
        }

        void Swap(Image& rhs) {
            Pixel* pxbuf = rhs.pixelBuffer;
            Vector2<int> dim = rhs.dimensions;

            rhs.dimensions = this->dimensions;
            rhs.pixelBuffer = this->pixelBuffer;

            this->pixelBuffer = pxbuf;
            this->dimensions = dim;
        }

        Image& operator=(Image const& other) {
            Image copy(other);
            Swap(copy);
            return *this;
        }
    };
} // namespace PixelGL
#endif