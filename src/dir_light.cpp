#include "dir_light.hpp"
#include "AVTmathLib.h"

void dir_light::setup(GLuint program, float* player) {
    GLint uniform_id = glGetUniformLocation(program, 
        ("lights[" + std::to_string(id) + "].is_enabled").c_str());
    glUniform1i(uniform_id, is_enabled);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_local").c_str());
    glUniform1i(uniform_id, is_local);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_spot").c_str());
    glUniform1i(uniform_id, is_spot);

    float pos[4] = { position[0], position[1], position[2], 1.0f };
    float res[4];
    multMatrixPoint(VIEW, pos, res);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].position").c_str());
    glUniform3f(uniform_id, res[0], res[1], res[2]);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].direction").c_str());
    glUniform3f(uniform_id, dir[0], dir[1], dir[2]);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].spot_cutoff").c_str());
    glUniform1f(uniform_id, spot_cutoff);
}