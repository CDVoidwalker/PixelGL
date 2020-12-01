#ifndef PIXELGL_H
#define PIXELGL_H

#include <cstring> // for memset
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

namespace PixelGL
{
#pragma region Macros
#define vlog(x)         \
    do                  \
    {                   \
        printf(x);      \
        printf("\n");   \
        fflush(stdout); \
    } while (0)

#define vlogf(x, args...) \
    do                    \
    {                     \
        printf(x, args);  \
        printf("\n");     \
        fflush(stdout);   \
    } while (0)

#define vclamp(val, min, max) \
    do                        \
    {                         \
        if (val < min)        \
            val = min;        \
        if (val > max)        \
            val = max;        \
    } while (0)

#define vthrow(error)           \
    do                          \
    {                           \
        fprintf(stderr, error); \
        exit(-1);               \
    } while (0)

#pragma endregion Macros

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

        bool operator==(const Vector2<T> &other)
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vector2<T> &other)
        {
            return !(*this == other);
        }

        T length()
        {
            return sqrt(x * x + y * y);
        }
    };

    struct Pixel
    {
        unsigned char r, g, b;
        Pixel(unsigned char r, unsigned char g, unsigned char b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        bool operator==(const Pixel &other)
        {
            return r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(const Pixel &other)
        {
            return !(*this == other);
        }
    };

    enum KeyState
    {
        NO_STATE = -1,
        RELEASED = GLFW_RELEASE,
        PRESSED = GLFW_PRESS,
        HELD,
    };

    void PXSleep(long long sleepMs)
    {
#ifdef __linux__
        usleep(sleepMs * 1000); // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _WIN32
        Sleep(sleepMs);
#endif
    }
    // note: action has corresponding values to keystate
    void _key_fun(GLFWwindow *win, int key, int scancode, int action, int modifier_bits);
    // note: action has corresponding values to keystate
    void _mouse_fun(GLFWwindow *window, int button, int action, int mods);
    void _update_keypresses();
    KeyState *_keystates;
    const int KEYSTATES_SIZE = GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST;

    class Engine
    {
        static bool INPUT_MANAGED;
        // TODO: possibly add proper shaders to sample texture
        const char *vertexShaderSource = "void main() { gl_Position = gl_Vertex; }"; /*"#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";*/

        const char *fragment_shader = "#version 400\n"
                                      "out vec4 frag_colour;"
                                      "void main() {"
                                      "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
                                      "}";

        int _width, _height, _pixelsize, _fullscreen;
        char *_title;
        GLFWwindow *_win;
        Pixel *_pixels;

        /// Size in Pixel
        int _pixels_size = -1;
        GLuint _framebuffer = INT32_MAX;

        GLuint _shader_program;
        double _fps = 2;
        long double _s_delay;


        inline int _pixelindex(int x, int y);
        void _refresh_framebuffer();
        void _create_framebuffer();

        // default update function
        void _randomize_pixels(double dt);

        virtual void Start();
        virtual void Update(double dt);

    public:
        // TODO: support width and height other than powers of 2
        Engine(int width, int height, int pixelsize, double fps, int fullscreen);
        ~Engine();
        void Run();

        KeyState GetKeyState(int glfw_key);
        KeyState GetMouseButtonState(int glfw_mousebutton);

        Vector2<double> MousePosition();
        Vector2<unsigned int> MousePixelPosition();

        inline void SetPixel(int x, int y, Pixel col);
        inline void SetPixelUnsafe(int x, int y, Pixel col);
        inline Pixel GetPixel(int x, int y);
        inline Pixel GetPixelUnsafe(int x, int y);
        void Clear();
    };

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

    Vector2<unsigned int> Engine::MousePixelPosition()
    {
        Vector2<double> mpos = MousePosition();

        int x = mpos.x / _pixelsize;
        int y = ((_height * _pixelsize) - mpos.y) / _pixelsize;
        vclamp(x, 0, _width - 1);
        vclamp(y, 0, _height - 1);

        Vector2<unsigned int> r(x, y);
        return r;
    }

    inline int Engine::_pixelindex(int x, int y)
    {
        return x + (y * _width);
    }

    void Engine::SetPixelUnsafe(int x, int y, Pixel col)
    {
        _pixels[_pixelindex(x, y)] = col;
    }

    inline void Engine::SetPixel(int x, int y, Pixel col)
    {
        if (x >= 0 && x < _width && y >= 0 && y < _height)
            SetPixelUnsafe(x, y, col);
    }

    inline Pixel Engine::GetPixel(int x, int y)
    {
        if (x >= 0 && x < _width && y >= 0 && y < _height)
            return GetPixelUnsafe(x, y);
        else
        {
            vthrow("getpixel called with out of bounds coordinates!");
        }
    }

    inline Pixel Engine::GetPixelUnsafe(int x, int y)
    {
        return _pixels[_pixelindex(x, y)];
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

    Engine::Engine(int width, int height, int pixelsize, double fps, int fullscreen)
    {
        this->_width = width;
        this->_height = height;
        this->_pixelsize = pixelsize;
        this->_fullscreen = fullscreen;
        this->_fps = fps;
        this->_s_delay = 1.0 / _fps;
        this->_title = "PixelGL Window";

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
        _win = glfwCreateWindow(_width * pixelsize, _height * pixelsize, _title, NULL, NULL); // (TODO: FIX FULLSCREEN) not fullscreen , not sharing resources
        glfwMakeContextCurrent(_win);
        glfwSwapInterval(1);

        if (_win != NULL)
        {
            vlogf("Successfully initalized glfw ver(%s) window with width=%i height=%i title=%s fullscreen=%i", glfwGetVersionString(), _width, _height, _title, _fullscreen);
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

        // setup projection
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, _width, 0, _height, -1, 1); // world size
        glMatrixMode(GL_MODELVIEW);

        // Adjust viewport to pixel size
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
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.0f, _height, 0.0f);

            // top right
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(_width, _height, 0.0f);

            // bot right
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(_width, 0.0f, 0.0f);

            // bot left
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
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
        }
    }

    Engine::~Engine()
    {
        glfwTerminate();
    }

    void Engine::Update(double dt) { printf("Base update");}
    void Engine::Start() { printf("Base start");}
} // namespace PixelGL
#endif