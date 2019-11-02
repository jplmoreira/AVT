#pragma once

#include <GL/glew.h>

struct light {
    int id;
    bool is_enabled;
    bool is_local;
    bool is_spot;
    float position[3];
    float dir[3];
    float spot_cutoff;

    virtual void setup(GLuint program, float* player) = 0;
};