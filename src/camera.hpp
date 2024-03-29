#pragma once

#include "vsShaderLib.h"
#include "scene_manager.hpp"

class camera {
    int w, h;
    float fov;
    float x, y, z;
    float upx, upy, upz;
    float atx, aty, atz;

public:
    camera(int w, int h, float fov, float x, float y, float z,
        float upx, float upy, float upz,
        float atx, float aty, float atz, object stencil);
    camera();

    void resize(int w, int h, VSShaderLib &shader);
    void look(float x, float y, float z);
    void move_to(float x, float y, float z);
    void set_up(float x, float y, float z);
    void make_ortho(bool flag);
    void make_moving(bool flag);
    void draw(scene_manager& scene, VSShaderLib& shader);
    void create_stencil(VSShaderLib& shader);

    GLint pvm_uniformId = 0;
    GLint vm_uniformId = 0;
    GLint normal_uniformId = 0;
    bool is_ortho = true;
    bool moving = false;
    object stencil_obj;
};