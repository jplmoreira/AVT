#pragma once

#include <GL/glew.h>

struct light {
    int id;
    bool is_enabled;
    bool is_local;
    bool is_spot;
    float color[3];
    float position[3];
    float dir[3];
    float spot_cutoff;
    float spot_exponent;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;

    virtual void setup(GLuint program) = 0;
};