#ifndef PIXELGL_FUNCTIONS
#define PIXELGL_FUNCTIONS

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "pixel.h"
namespace PixelGL
{
    void PXSleep(long long sleepMs)
    {
#ifdef __linux__
        usleep(sleepMs * 1000); // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _WIN32
        Sleep(sleepMs);
#endif
    }

    void memcpyPixel(Pixel *source, const size_t number, Pixel *destination)
    {
        for (size_t i = 0; i < number; i++)
            destination[i] = source[i];
    }

    void memsetPixel(Pixel *start, const size_t number, Pixel constant)
    {
        for (size_t i = 0; i < number; i++)
            start[i] = constant;
    }
} // namespace PixelGL
#endif