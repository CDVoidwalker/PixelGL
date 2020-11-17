#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#pragma region Macros

// __HIDDEN__ restricts access to these to current file
#define __HIDDEN__ __attribute__((visibility("hidden")))
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
#pragma region StructsAndEnums

typedef struct pixel_t
{
    unsigned char r, g, b;
} pixel_t;

typedef struct vector2d_t
{
    double x, y;
} vector2d_t;

typedef struct vector2i_t
{
    int x, y;
} vector2i_t;

typedef enum pxgl_keystate_t
{
    PXGLKEY_NO_STATE = -1,
    PXGLKEY_RELEASED = GLFW_RELEASE,
    PXGLKEY_PRESSED = GLFW_PRESS,
    PXGLKEY_HELD,
} pxgl_keystate_t;

#pragma endregion StructsAndEnums
#pragma region Variables

// TODO: possibly add proper shaders to sample texture
__HIDDEN__ const char *vertexShaderSource = "void main() { gl_Position = gl_Vertex; }"; /*"#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";*/

__HIDDEN__ const char *fragment_shader = "#version 400\n"
                                         "out vec4 frag_colour;"
                                         "void main() {"
                                         "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
                                         "}";

__HIDDEN__ int _width = 800, _height = 600, _pixelsize = 4;
__HIDDEN__ char *_title = "PixelGL Window";
__HIDDEN__ int _fullscreen = GL_FALSE;
__HIDDEN__ GLFWwindow *_win;

__HIDDEN__ pixel_t *_pxgl_pixels;
/// Size in pixel_t
__HIDDEN__ int _pxgl_pixels_size = -1;
__HIDDEN__ GLuint _pxgl_framebuffer = INT32_MAX;

__HIDDEN__ GLuint _shader_program;
__HIDDEN__ double _fps = 2;
__HIDDEN__ long double _s_delay;

__HIDDEN__ void (/*@null@*/ *_pxgl_update)(double dt) = NULL;

const int KEYSTATES_SIZE = GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST;
__HIDDEN__ pxgl_keystate_t *_pxgl_keystates;

#pragma endregion Variables
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ // Function Declarations

// TODO: support width and height other than powers of 2
void pxgl_init(int width, int height, int pixelsize, double fps, int fullscreen, void (*updatefunction)(double));
void pxgl_free();

pxgl_keystate_t pxgl_keystate(int glfw_key);
pxgl_keystate_t pxgl_mbuttonstate(int glfw_mousebutton);

void pxgl_sleep(long long sleepMs);
vector2d_t pxgl_mouseposition();
vector2i_t pxgl_mpos_to_pixel(vector2d_t mpos);
extern inline void pxgl_setpixel(int x, int y, pixel_t col);
extern inline void pxgl_setpixel_unsafe(int x, int y, pixel_t col);
extern inline pixel_t pxgl_getpixel(int x, int y);
extern inline pixel_t pxgl_getpixel_unsafe(int x, int y);

// note: action has corresponding values to pxgl_keystate
__HIDDEN__ void _pxgl_key_fun(GLFWwindow *win, int key, int scancode, int action, int modifier_bits);
// note: action has corresponding values to pxgl_keystate
__HIDDEN__ void _pxgl_mouse_fun(GLFWwindow *window, int button, int action, int mods);

__HIDDEN__ extern inline int _pxgl_pixelindex(int x, int y);
__HIDDEN__ void _pxgl_update_keypresses();
__HIDDEN__ void _pxgl_refresh_framebuffer();
__HIDDEN__ void _pxgl_create_framebuffer();

// default update function
__HIDDEN__ void _pxgl_randomize_pixels(double dt);
__HIDDEN__ void _pxgl_loop();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ // Function Implementation

void pxgl_sleep(long long sleepMs)
{
#ifdef __linux__
    usleep(sleepMs * 1000); // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _WIN32
    Sleep(sleepMs);
#endif
}

void pxgl_clear()
{
    memset(_pxgl_pixels, 0, _pxgl_pixels_size * sizeof(pixel_t));
}

pxgl_keystate_t pxgl_keystate(int glfw_key)
{
    return _pxgl_keystates[glfw_key];
}

pxgl_keystate_t pxgl_mbuttonstate(int glfw_mousebutton)
{
    return _pxgl_keystates[glfw_mousebutton + GLFW_KEY_LAST];
}

vector2d_t pxgl_mouseposition()
{
    vector2d_t v;
    glfwGetCursorPos(_win, &v.x, &v.y);
    return v;
}

vector2i_t pxgl_mpos_to_pixel(vector2d_t mpos)
{
    int x = mpos.x / _pixelsize;
    int y = ((_height * _pixelsize) - mpos.y) / _pixelsize;
    vclamp(x, 0, _width - 1);
    vclamp(y, 0, _height - 1);

    vector2i_t r = {.x = x, .y = y};
    return r;
}

__HIDDEN__ inline int _pxgl_pixelindex(int x, int y)
{
    return x + (y * _width);
}

void pxgl_setpixel_unsafe(int x, int y, pixel_t col)
{
    _pxgl_pixels[_pxgl_pixelindex(x, y)] = col;
}

inline void pxgl_setpixel(int x, int y, pixel_t col)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height)
        pxgl_setpixel_unsafe(x, y, col);
}

inline pixel_t pxgl_getpixel(int x, int y)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height)
        return pxgl_getpixel_unsafe(x, y);
    else
    {
        vthrow("pxgl_getpixel called with out of bounds coordinates!");
    }
}

inline pixel_t pxgl_getpixel_unsafe(int x, int y)
{
    return _pxgl_pixels[_pxgl_pixelindex(x, y)];
}

__HIDDEN__ void _pxgl_create_framebuffer()
{
    // allocate texture id for framebuffer
    glGenTextures(1, &_pxgl_framebuffer);
    glBindTexture(GL_TEXTURE_2D, _pxgl_framebuffer);

    // mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // provide with valid framebuffer of pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _pxgl_pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

__HIDDEN__ void _pxgl_refresh_framebuffer()
{
    glDeleteTextures(1, &_pxgl_framebuffer);
    _pxgl_create_framebuffer();
}

__HIDDEN__ void _pxgl_randomize_pixels(double dt)
{
    for (int i = 0; i < _pxgl_pixels_size; i++)
    {
        _pxgl_pixels[i].r = rand() % 255; //r
        _pxgl_pixels[i].g = rand() % 255; //g
        _pxgl_pixels[i].b = rand() % 255; //b
    }
}

__HIDDEN__ void _pxgl_key_fun(GLFWwindow *win, int key, int scancode, int action, int modifier_bits)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE)
        _pxgl_keystates[key] = action;
}

__HIDDEN__ void _pxgl_mouse_fun(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE)
        _pxgl_keystates[button + GLFW_KEY_LAST] = action;
}

__HIDDEN__ void _pxgl_update_keypresses()
{
    for (int i = 0; i < KEYSTATES_SIZE; i++)
    {
        switch (_pxgl_keystates[i])
        {
        case PXGLKEY_PRESSED:
            _pxgl_keystates[i] = PXGLKEY_HELD;
            break;
        case PXGLKEY_RELEASED:
            _pxgl_keystates[i] = PXGLKEY_NO_STATE;
            break;
        case PXGLKEY_HELD:
        case PXGLKEY_NO_STATE:
        default:
            break;
        }
    }
}

void pxgl_init(int width, int height, int pixelsize, double fps, int fullscreen, void (*updatefunction)(double))
{
    _width = width;
    _height = height;
    _pixelsize = pixelsize;
    _fullscreen = fullscreen;
    _fps = fps;
    _s_delay = 1.0 / _fps;
    _pxgl_update = updatefunction == NULL ? _pxgl_randomize_pixels : updatefunction;

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

    _pxgl_pixels_size = _width * _height;
    _pxgl_pixels = (pixel_t *)calloc(sizeof(pixel_t), _pxgl_pixels_size);

    _pxgl_create_framebuffer();

    // setup projection
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, _width, 0, _height, -1, 1); // world size
    glMatrixMode(GL_MODELVIEW);

    // Adjust viewport to pixel size
    glViewport(0, 0, _width * _pixelsize, _height * _pixelsize);

    // Input Management
    glfwSetKeyCallback(_win, _pxgl_key_fun);
    glfwSetMouseButtonCallback(_win, _pxgl_mouse_fun);
    _pxgl_keystates = (pxgl_keystate_t *)malloc(sizeof(pxgl_keystate_t) * KEYSTATES_SIZE);
    memset(_pxgl_keystates, PXGLKEY_NO_STATE, sizeof(pxgl_keystate_t) * KEYSTATES_SIZE);

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

    _pxgl_loop();
}

__HIDDEN__ void _pxgl_loop()
{
    double pre, post, diff;
    GLenum code;

    while (!glfwWindowShouldClose(_win))
    {
        pre = glfwGetTime();
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(_shader_program);

        glBindTexture(GL_TEXTURE_2D, _pxgl_framebuffer);
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
        _pxgl_update(diff);
        _pxgl_update_keypresses();
        _pxgl_refresh_framebuffer();

        post = glfwGetTime();
        diff = post - pre;
        if (diff < _s_delay)
        {
            pxgl_sleep((_s_delay - diff) * 1e3);
        }
    }
}

void pxgl_free()
{
    glfwTerminate();
}
