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
	float* wood_pos();
	void wood_move();
	void wood_inverse_move();
	void wood_stop();
	float* river_pos();
	float* road_pos();
};