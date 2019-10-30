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
    std::vector<std::shared_ptr<object>> child_objs;
    std::chrono::time_point<std::chrono::system_clock> last, current;
    bool looping;
    float limit_upx, limit_dx;
    float limit_upz, limit_dz;

public:
    object(mesh m);
    object();

    void add_child(std::shared_ptr<object> obj);
    bool is_looping();
    void loop(bool l);
    void set_limits(float ux, float dx, float uz, float dz);

    virtual void move(float x, float y, float z);
    virtual void render(camera& cam, VSShaderLib& shader);
    virtual void render_child(camera& cam, VSShaderLib& shader, object& parent);

    mesh get_mesh() const;
    float posx = 0.0f;
    float posy = 0.0f;
    float posz = 0.0f;
    int stencil_func = GL_ALWAYS;
};