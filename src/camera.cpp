#include "camera.hpp"
#include "AVTmathLib.h"

camera::camera(float x, float y, float z,
    float upx, float upy, float upz,
    float atx, float aty, float atz) :
    x(x), y(y), z(z),
    upx(upx), upy(upy), upz(upz),
    atx(atx), aty(aty), atz(atz) {}

void camera::resize(int w, int h) {}

void camera::look(float x, float y, float z) {
    atx = x; aty = y; atz = z;
}

void camera::move_to(float tox, float toy, float toz) {
    x = tox;
    y = toy;
    z = toz;
}

void camera::draw(scene_manager& scene, VSShaderLib& shader) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load identity matrices
    loadIdentity(VIEW);
    // set the camera using a function similar to gluLookAt
    lookAt(x, y, z, atx, aty, atz, upx, upy, upz);
    // use our shader
    glUseProgram(shader.getProgramIndex());

    //send the light position in eye coordinates

    //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 
    //glUniform4fv(lPos_uniformId, 1, lightPos);

    float res[4];
    multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
    glUniform4fv(lPos_uniformId, 1, res);

    for (auto const& ptr : scene.objs) {
        ptr->render(*this, shader);
    }
}
