#include "AssetManager.h"

AssetManager* AssetManager::_instance = nullptr;

AssetManager *AssetManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new AssetManager();

    return _instance;
}

AssetManager::AssetManager()
{
    tileImages.Ground = Image("./assets/platformer/blocks.png", {0, 0}, {16, 16});
    tileImages.Brick = Image("./assets/platformer/blocks.png", {16, 0}, {16, 16});
    tileImages.QM_Block = Image("./assets/platformer/blocks.png", {240, 0}, {16, 16});
    tileImages.QM_Block2 = Image("./assets/platformer/blocks.png", {256, 0}, {16, 16});
    tileImages.QM_Block3 = Image("./assets/platformer/blocks.png", {272, 0}, {16, 16});
    tileImages.Pipe_tl = Image("./assets/platformer/blocks.png", {0, 128}, {16, 16});
    tileImages.Pipe_tr = Image("./assets/platformer/blocks.png", {16, 128}, {16, 16});
    tileImages.Pipe_l = Image("./assets/platformer/blocks.png", {0, 144}, {16, 16});
    tileImages.Pipe_r = Image("./assets/platformer/blocks.png", {16, 144}, {16, 16});
    tileImages.Flagpole_top = Image("./assets/platformer/blocks.png", {256, 256}, {16, 16});
    tileImages.Flagpole = Image("./assets/platformer/blocks.png", {256, 272}, {16, 16});
    tileImages.Block = Image("./assets/platformer/blocks.png", {0, 16}, {16, 16});
}
