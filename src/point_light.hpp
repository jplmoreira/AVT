#pragma once

#include "light.hpp"
#include "AVTmathLib.h"

struct point_light : public light {

public:
    point_light(int i, bool enabled, float pos[3]) {
        id = i;
        is_enabled = enabled;
        is_local = true;
        is_spot = false;
        this->position[0] = pos[0];
        this->position[1] = pos[1];
        this->position[2] = pos[2];
        spot_cutoff = 0.0f;
    }

    virtual void setup(GLuint program, float *player);
};