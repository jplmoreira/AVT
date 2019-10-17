#pragma once

#include "vsShaderLib.h"
#include "scene_manager.hpp"

class camera {
    float x, y, z;
    float upx, upy, upz;
    float atx, aty, atz;
    float lightPos[4] = { 4.0f, 6.0f, 2.0f, 1.0f };

public:
    camera(float x, float y, float z, float upx, float upy, float upz, float atx, float aty, float atz);

    void resize(int w, int h);
    void look(float x, float y, float z);
    void move_to(float x, float y, float z);
    void draw(scene_manager& scene, VSShaderLib& shader);

    GLint pvm_uniformId = 0;
    GLint vm_uniformId = 0;
    GLint normal_uniformId = 0;
    GLint lPos_uniformId = 0;
};