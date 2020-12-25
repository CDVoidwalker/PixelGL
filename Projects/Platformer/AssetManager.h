#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include "image.h"
using namespace PixelGL;

struct TileImages
{
    Image Ground;
    Image Brick;
    Image QM_Block;
    Image QM_Block2;
    Image QM_Block3;
    Image Pipe_tl;
    Image Pipe_tr;
    Image Pipe_l;
    Image Pipe_r;
    Image Flagpole_top;
    Image Flagpole;
    Image Block;
};

class AssetManager
{
    static AssetManager *_instance;

public:
    TileImages tileImages;
    static AssetManager *getInstance();
    AssetManager();
};
#endif