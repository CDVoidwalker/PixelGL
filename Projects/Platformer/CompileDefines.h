#ifndef COMPILE_DEFINES_H
#define COMPILE_DEFINES_H
#include "pixel.h"

using namespace PixelGL;

const int WIDTH = 256, HEIGHT = 240, PIXEL_SIZE = 3, FPS = 60, BLOCK_DIM = 16;
const Pixel SKY_COLOR{61, 85, 248};
const float WALK_ACCEL = 8.0f; //acceleration of walking on ground
const float RUN_ACCEL = WALK_ACCEL * 2.0f; // acceleration of running on ground
const float MIN_SPEED = 0.7f;
const float MAX_WALK_SPEED = 6.4f;// --fastest speedx when walking
const float MAX_RUN_SPEED = 9.0f;// --fastest speedx when running
const float FRICTION_GROUND = 14.0f;// --amount of speed that is substracted when not pushing buttons, as well as speed added to acceleration when changing directions
const float FRICTION_AIR = 0.0f;// --see above, but in air
const float AIRSLIDE_FACTOR = 0.8f;// --multiply of acceleration in air when changing direction
const float JUMPFORCE = 16.0f, JUMPFORCE_ADD = 1.9f;
const float JUMP_GRAV = 30.0f, GRAV = 80.0f;
#endif