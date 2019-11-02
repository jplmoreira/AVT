#pragma once

#include "light.hpp"
#include "AVTmathLib.h"

#include <cmath>
const double pi = std::acos(-1);

struct spot_light : public light {

public:
    spot_light(int i, bool enabled, float pos[3], float dir[3], float angle) {
        id = i;
        is_enabled = enabled;
        is_local = true;
        is_spot = true;
        position[0] = pos[0];
        position[1] = pos[1];
        position[2] = pos[2];
        this->dir[0] = dir[0];
        this->dir[1] = dir[1];
        this->dir[2] = dir[2];
        spot_cutoff = angle * pi / 180.0f;
    }

    virtual void setup(GLuint program, float* player);
};