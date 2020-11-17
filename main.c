#include "pixelgl.h"

int x = 5, y = 5;
const pixel_t PLAYER_COLOR = {.r = 255, .g = 0, .b = 0};

void update(double dt)
{
    pxgl_clear();
    if (pxgl_keystate(GLFW_KEY_A) == PXGLKEY_PRESSED)
        x--;

    if (pxgl_keystate(GLFW_KEY_D) == PXGLKEY_RELEASED)
        x++;

    if (pxgl_keystate(GLFW_KEY_W) == PXGLKEY_HELD)
        y++;

    if (pxgl_keystate(GLFW_KEY_S) == PXGLKEY_HELD)
        y--;

    if (pxgl_mbuttonstate(GLFW_MOUSE_BUTTON_1) == PXGLKEY_PRESSED)
        vlog("Mouse PRESSED");

    if (pxgl_mbuttonstate(GLFW_MOUSE_BUTTON_1) == PXGLKEY_HELD)
        vlog("Mouse HELD");

    if (pxgl_mbuttonstate(GLFW_MOUSE_BUTTON_1) == PXGLKEY_RELEASED)
        vlog("Mouse RELEASED");

    pxgl_setpixel(x,y, PLAYER_COLOR);
}

int main(int argc, char **argv)
{
    pxgl_init(128, 64, 8, 60, GL_FALSE, update); // update
    pxgl_free();

    return 0;
}