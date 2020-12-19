#ifndef PIXELGL_ENGINE
#define PIXELGL_ENGINE
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vector2.h"
#include "pixel.h"
namespace PixelGL
{
    enum KeyState
    {
        NO_STATE = -1,
        RELEASED = GLFW_RELEASE,
        PRESSED = GLFW_PRESS,
        HELD,
    };
    
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
        std::string _title;
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
        Engine(int width, int height, int pixelsize, double fps, int fullscreen, const std::string &title = "PixelGL Window");
        ~Engine();
        void Run();
        void SetTitle(const std::string &title);

        KeyState GetKeyState(int glfw_key);
        KeyState GetMouseButtonState(int glfw_mousebutton);

        Vector2<double> MousePosition();
        Vector2<int> MousePixelPosition();

        // Returns true when coordinates are in bounds.
        inline bool SetPixel(const Vector2<int> &pos, Pixel col);
        inline void SetPixelUnsafe(const Vector2<int> &pos, Pixel col);
        inline Pixel GetPixel(const Vector2<int> &pos);
        inline Pixel GetPixelUnsafe(const Vector2<int> &pos);
        inline bool isInBounds(const PixelGL::Vector2<int> &pos);
        void Line(Vector2<int> from, Vector2<int> to, const PixelGL::Pixel &col);

        // TODO: it's slow, speed it up by adding fast horizontal line with memcpy
        void Rect(const Vector2<int> &topLeft, const Vector2<int> &botRight, const PixelGL::Pixel &col);
        void FillRect(const Vector2<int> &topLeft, const Vector2<int> &botRight, const PixelGL::Pixel &col);

        void Clear();
    };
} // namespace PixelGL
#endif