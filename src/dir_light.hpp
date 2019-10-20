#pragma once

#include "light.hpp"
#include "AVTmathLib.h"

struct dir_light : public light {

public:
    dir_light(int i, bool enabled, float dir[3]) {
        id = i;
        is_enabled = enabled;
        is_local = false;
        is_spot = false;
        this->dir[0] = dir[0];
        this->dir[1] = dir[1];
        this->dir[2] = dir[2];
        spot_cutoff = 0.0f;
        spot_exponent = 0.0f;
        constant_attenuation = 0.0f;
        linear_attenuation = 0.0f;
        quadratic_attenuation = 0.0f;
    }

    virtual void setup(GLuint program);
};