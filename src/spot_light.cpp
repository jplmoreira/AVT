#include "spot_light.hpp"

void spot_light::setup(GLuint program, float* player) {
    GLint uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_enabled").c_str());
    glUniform1i(uniform_id, is_enabled);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_local").c_str());
    glUniform1i(uniform_id, is_local);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_spot").c_str());
    glUniform1i(uniform_id, is_spot);

    pushMatrix(MODEL);
    loadIdentity(MODEL);
    translate(MODEL, player[0], player[1], player[2]);
    float pos[4] = { position[0], position[1], position[2], 1.0f };
    float res[4];
    multMatrixPoint(MODEL, pos, res);
    popMatrix(MODEL);

    multMatrixPoint(VIEW, res, pos);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].position").c_str());
    glUniform3f(uniform_id, pos[0], pos[1], pos[2]);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].direction").c_str());
    glUniform3f(uniform_id, dir[0], dir[1], dir[2]);

    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].spot_cutoff").c_str());
    glUniform1f(uniform_id, spot_cutoff);
}
