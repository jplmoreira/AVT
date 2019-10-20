#include "dir_light.hpp"

void dir_light::setup(GLuint program) {
    GLint uniform_id = glGetUniformLocation(program, 
        ("lights[" + std::to_string(id) + "].is_enabled").c_str());
    glUniform1i(uniform_id, is_enabled);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_local").c_str());
    glUniform1i(uniform_id, is_local);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].is_spot").c_str());
    glUniform1i(uniform_id, is_spot);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].color").c_str());
    glUniform3f(uniform_id, color[0], color[1], color[2]);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].position").c_str());
    glUniform3f(uniform_id, position[0], position[1], position[2]);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].direction").c_str());
    glUniform3f(uniform_id, dir[0], dir[1], dir[2]);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].spot_cutoff").c_str());
    glUniform1f(uniform_id, spot_cutoff);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].spot_exponent").c_str());
    glUniform1f(uniform_id, spot_exponent);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].constant_attenuation").c_str());
    glUniform1f(uniform_id, constant_attenuation);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].linear_attenuation").c_str());
    glUniform1f(uniform_id, linear_attenuation);
    uniform_id = glGetUniformLocation(program,
        ("lights[" + std::to_string(id) + "].quadratic_attenuation").c_str());
    glUniform1f(uniform_id, quadratic_attenuation);
}