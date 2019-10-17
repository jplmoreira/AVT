#pragma once

#include <chrono>
#include <vector>
#include <memory>

#include "mesh.hpp"
#include "vsShaderLib.h"

class camera;
class shader;

class object {
    mesh obj_mesh;
    float dirx, diry, dirz;
    float velocity;
    std::vector<std::unique_ptr<object>> child_objs;
    std::chrono::time_point<std::chrono::system_clock> last, current;

public:
    object(mesh m);

    void add_child(std::unique_ptr<object> obj);

    virtual void move(float x, float y, float z);
    virtual void render(camera& cam, VSShaderLib& shader);
};