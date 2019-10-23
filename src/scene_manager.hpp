#pragma once

#include <vector>
#include <memory>

#include "object.hpp"
#include "light.hpp"

class scene_manager {
public:
    std::vector<std::unique_ptr<object>> objs;
    std::vector<std::unique_ptr<light>> lights;

    void prepare_scene();
    void player_forward();
    void player_back();
    void player_right();
    void player_left();
    void player_stop();
    float* player_pos();
    void toggle_spot();
    void toggle_points();

private:
    void create_frog();
    void create_floor(float offset);
    void create_road();
    void create_water();
    void create_car(float x, float y, float dirz);
    void create_log(float x, float y, float dirz);

	void create_frog_ai();

    void create_point(float off_x, float off_z);
    void create_spot();
    void create_dir();
};