#pragma once

#include <vector>
#include <memory>

#include "vsShaderLib.h"
#include "mesh.hpp"

class camera;
class shader;

class object {
    mesh obj_mesh;
    float dirx, diry, dirz;
    std::vector<std::unique_ptr<object>> child_objs;

public:
    object(mesh m);

    void add_child(std::unique_ptr<object> obj);

    virtual void move();
    virtual void render(camera& cam, VSShaderLib& shader);
};