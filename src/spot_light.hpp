#pragma once

#include "light.hpp"
#include "AVTmathLib.h"

#include <cmath>
const double pi = std::acos(-1);

struct spot_light : public light {

public:
    spot_light(int i, bool enabled, float pos[3], float dir[3]) {
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
        spot_cutoff = std::cos(1.0f * pi / 180.0f);
        spot_exponent = 1.0f;
        constant_attenuation = 0.75f;
        linear_attenuation = 0.01f;
        quadratic_attenuation = 0.001f;
    }

    virtual void setup(GLuint program, float* player);
};