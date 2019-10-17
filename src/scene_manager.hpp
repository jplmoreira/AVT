#pragma once

#include <vector>
#include <memory>

#include "object.hpp"

class scene_manager {
public:
    std::vector<std::unique_ptr<object>> objs;

    void prepare_scene();
    void player_forward();
    void player_back();
    void player_right();
    void player_left();
    void player_stop();
    float* player_pos();
};