#ifndef PIXELGL_H
#define PIXELGL_H

#include <cstring> // for memset
#include <string>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "engine.h"
#include "image.h"
#include "macros.h"
#include "vector2.h"
#include "pixel.h"
#include "functions.h"

namespace PixelGL
{
    // note: action has corresponding values to keystate
    void _key_fun(GLFWwindow *win, int key, int scancode, int action, int modifier_bits);
    // note: action has corresponding values to keystate
    void _mouse_fun(GLFWwindow *window, int button, int action, int mods);
    void _update_keypresses();
    KeyState *_keystates;
    const int KEYSTATES_SIZE = GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ // Function Implementation

    bool Engine::INPUT_MANAGED = false;

    void Engine::Clear()
    {
        memset(_pixels, 0, _pixels_size * sizeof(Pixel));
    }

    KeyState Engine::GetKeyState(int glfw_key)
    {
        return _keystates[glfw_key];
    }

    KeyState Engine::GetMouseButtonState(int glfw_mousebutton)
    {
        return _keystates[glfw_mousebutton + GLFW_KEY_LAST];
    }

    Vector2<double> Engine::MousePosition()
    {
        Vector2<double> v;
        glfwGetCursorPos(_win, &v.x, &v.y);
        return v;
    }

    Vector2<int> Engine::MousePixelPosition()
    {
        Vector2<double> mpos = MousePosition();

        int x = mpos.x / _pixelsize;
        int y = mpos.y / _pixelsize;
        vclamp(x, 0, _width - 1);
        vclamp(y, 0, _height - 1);

        return {x, y};
    }

    inline int Engine::_pixelindex(int x, int y)
    {
        return x + (y * _width);
    }

    inline bool Engine::SetPixel(const Vector2<int> &pos, Pixel col)
    {
        if (isInBounds(pos))
        {
            SetPixelUnsafe(pos, col);
            return true;
        }
        else
            vlog("Engine::SetPixel called with out of bounds coordinates!");

        return false;
    }

    inline void Engine::SetPixelUnsafe(const Vector2<int> &pos, Pixel col)
    {
        _pixels[_pixelindex(pos.x, pos.y)] = col;
    }

    inline Pixel Engine::GetPixel(const Vector2<int> &pos)
    {
        if (isInBounds(pos))
            return GetPixelUnsafe(pos);
        else
            vlog("Engine::GetPixel called with out of bounds coordinates!");
    }

    inline Pixel Engine::GetPixelUnsafe(const Vector2<int> &pos)
    {
        return _pixels[_pixelindex(pos.x, pos.y)];
    }

    void Engine::FastHorizontalLine(Vector2<int> from, Vector2<int> to, const PixelGL::Pixel &col)
    {
        int start = std::min(from.x, to.x), end = std::max(from.x, to.x);
#ifdef PIXELGL_DEBUG
        if (from.y != to.y)
            vthrow("Engine::FastHorizontalLines Y coordinates are not equal!");
#endif
        // assumptions:
        // end > start

        // if end is lower than 0, means that start also is and they are both out of bounds
        if (end < 0)
            return;

        // if start is higher than width, means that end also is
        if (start >= _width)
            return;

        // assuming that both are equal, only one check suffices
        if (from.y >= _height || from.y < 0)
            return;

        // ensure bounds
        if (end >= _width)
            end = _width - 1;

        if (start < 0)
            start = 0;

        size_t pixnum = end - start;
        memsetPixel(&_pixels[_pixelindex(start, from.y)], pixnum + 1, col);
    }

    void Engine::CopyImageLine(Vector2<int> origin, size_t pixnum, PixelGL::Pixel *source)
    {
        int end = pixnum + origin.x;
        bool negativex = false;

        // oob on Y axis
        if (origin.y >= _height || origin.y < 0)
            return;

        // oob on positive x
        if (origin.x > _width)
            return;

        // oob on negative x
        if (end < 0)
        {
            return;
            // only start point is oob on positive x
        }
        else if (origin.x < 0)
        {
            pixnum -= std::abs(origin.x);
            source += -origin.x;
            origin.x = 0;
        }

        // only end point is oob on positive x
        if (end >= _width)
        {
            pixnum -= end - _width;
            end = _width - 1;
        }

        memcpyPixel(source, pixnum, &_pixels[_pixelindex(origin.x, origin.y)]);
    }

    void Engine::Line(Vector2<int> from, Vector2<int> to, const PixelGL::Pixel &col)
    {
        if (from == to)
        {
            SetPixel(from, col);
            return;
        }

        Vector2<float> ab = to - from;
        float dx = ab.x / ab.length(), dy = ab.y / ab.length();
        for (int i = 0; i <= std::round(ab.length()); i++)
        {
            SetPixel({std::round(from.x + (dx * i)), std::round(from.y + (dy * i))}, col);
        }
    }

    void Engine::Rect(const Vector2<int> &topLeft, const Vector2<int> &botRight, const PixelGL::Pixel &col)
    {
        FastHorizontalLine({topLeft.x, topLeft.y}, {botRight.x, topLeft.y}, col);
        Line({botRight.x, topLeft.y}, {botRight.x, botRight.y}, col);
        FastHorizontalLine({botRight.x, botRight.y}, {topLeft.x, botRight.y}, col);
        Line({topLeft.x, botRight.y}, {topLeft.x, topLeft.y}, col);
    }

    void Engine::FillRect(const Vector2<int> &p1, const Vector2<int> &p2, const PixelGL::Pixel &col)
    {
        int start = std::min(p1.y, p2.y), end = std::max(p1.y, p2.y);
        for (; start <= end; start++)
            FastHorizontalLine({p1.x, start}, {p2.x, start}, col);
    }

    inline bool Engine::isInBounds(const PixelGL::Vector2<int> &pos)
    {
        return !(pos.x < 0 || pos.x >= _width || pos.y < 0 || pos.y >= _height);
    }

    void Engine::_create_framebuffer()
    {
        // allocate texture id for framebuffer
        glGenTextures(1, &_framebuffer);
        glBindTexture(GL_TEXTURE_2D, _framebuffer);

        // mipmapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // provide with valid framebuffer of pixels
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Engine::_refresh_framebuffer()
    {
        glDeleteTextures(1, &_framebuffer);
        this->_create_framebuffer();
    }

    void _key_fun(GLFWwindow *win, int key, int scancode, int action, int modifier_bits)
    {
        if (action == GLFW_PRESS || action == GLFW_RELEASE)
            _keystates[key] = (KeyState)action;
    }

    void _mouse_fun(GLFWwindow *window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS || action == GLFW_RELEASE)
            _keystates[button + GLFW_KEY_LAST] = (KeyState)action;
    }

    void _update_keypresses()
    {
        for (int i = 0; i < KEYSTATES_SIZE; i++)
        {
            switch (_keystates[i])
            {
            case PRESSED:
                _keystates[i] = HELD;
                break;
            case RELEASED:
                _keystates[i] = NO_STATE;
                break;
            case HELD:
            case NO_STATE:
            default:
                break;
            }
        }
    }

    Engine::Engine(int width, int height, int pixelsize, double fps, int fullscreen, const std::string &title)
    {
        this->_width = width;
        this->_height = height;
        this->_pixelsize = pixelsize;
        this->_fullscreen = fullscreen;
        this->_fps = fps;
        this->_s_delay = 1.0 / _fps;
        this->_title = title;

        if (!glfwInit())
        {
            vlog("Failed to initalize initalized glfw!");
        }
        else
        {
            vlog("Succesfully initalized glfw!");
        }

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // not resizable
        glfwWindowHint(GLFW_SAMPLES, 1);          // multisampling set to 1
        GLFWmonitor *monitor = _fullscreen ? glfwGetPrimaryMonitor() : NULL;
        _win = glfwCreateWindow(_width * pixelsize, _height * pixelsize, _title.c_str(), NULL, NULL); // (TODO: FIX FULLSCREEN) not fullscreen , not sharing resources
        glfwMakeContextCurrent(_win);
        glfwSwapInterval(1);

        if (_win != NULL)
        {
            vlogf("Successfully initalized glfw ver(%s) window with width=%i height=%i title=%s fullscreen=%i", glfwGetVersionString(), _width, _height, _title.c_str(), _fullscreen);
        }
        else
        {
            vlog("Failed to initalize glfw window!");
        }

        GLenum code = glewInit();
        if (!code)
        {
            vlog("Succesfully initalized glew!");
        }
        else
        {
            vlogf("Failed to initalize glew! Error code: %u", code);
        }

        _pixels_size = _width * _height;
        _pixels = (Pixel *)calloc(sizeof(Pixel), _pixels_size);

        _create_framebuffer();
        glOrtho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0); // world size
        glViewport(0, 0, _width * _pixelsize, _height * _pixelsize);

        // Input Management
        if (!INPUT_MANAGED)
        {
            glfwSetKeyCallback(_win, _key_fun);
            glfwSetMouseButtonCallback(_win, _mouse_fun);
            _keystates = (KeyState *)malloc(sizeof(KeyState) * KEYSTATES_SIZE);
            memset(_keystates, NO_STATE, sizeof(KeyState) * KEYSTATES_SIZE);
            INPUT_MANAGED = true;
        }

        // TODO: implement!
        // shader setup
        // GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        // glShaderSource(vs, 1, &vertexShaderSource, NULL);
        // glCompileShader(vs);
        // GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        // glShaderSource(fs, 1, &fragment_shader, NULL);
        // glCompileShader(fs);

        // shader program
        // _shader_program = glCreateProgram();
        // glAttachShader(_shader_program, fs);
        // glAttachShader(_shader_program, vs);
        // glLinkProgram(_shader_program);
    }

    void Engine::Run()
    {
        double pre, post, diff;
        GLenum code;

        Start();
        while (!glfwWindowShouldClose(_win))
        {
            pre = glfwGetTime();
            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            //glUseProgram(_shader_program);

            glBindTexture(GL_TEXTURE_2D, _framebuffer);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);

            // top left
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);

            // top right
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, 0.0f);

            // bot right
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, 0.0f);

            // bot left
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glEnd();

            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFlush();

            // buffer swap
            glfwSwapBuffers(_win);
            glfwPollEvents();

            // update and draw
            Update(diff);
            _update_keypresses();
            _refresh_framebuffer();

            post = glfwGetTime();
            diff = post - pre;
            if (diff < _s_delay)
            {
                PXSleep((_s_delay - diff) * 1e3);
            }
            tickCounter++;
        }
    }

    Engine::~Engine()
    {
        glfwTerminate();
    }

    void Engine::Update(double dt) {}
    void Engine::Start() {}
    void Engine::SetTitle(const std::string &title)
    {
        _title = title;
        glfwSetWindowTitle(this->_win, _title.c_str());
    }

    uint64_t Engine::GetFrameCount()
    {
        return tickCounter;
    }

} // namespace PixelGL
#endif