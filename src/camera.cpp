#include "camera.hpp"
#include "AVTmathLib.h"

#include <iostream>

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

camera::camera(int w, int h, float fov,
    float x, float y, float z,
    float upx, float upy, float upz,
    float atx, float aty, float atz,
    object stencil) :
    x(x), y(y), z(z),
    upx(upx), upy(upy), upz(upz),
    atx(atx), aty(aty), atz(atz),
    w(w), h(h), fov(fov), stencil_obj(stencil) {}

camera::camera() {}

void camera::resize(int width, int height, VSShaderLib &shader) {
    w = width;
    h = height;
    create_stencil(shader);
    make_ortho(is_ortho);
}

void camera::look(float x, float y, float z) {
    atx = x; aty = y; atz = z;
}

void camera::move_to(float tox, float toy, float toz) {
    x = tox;
    y = toy;
    z = toz;
}

void camera::set_up(float x, float y, float z) {
    upx = x;
    upy = y;
    upz = z;
}

void camera::make_ortho(bool flag) {
    loadIdentity(PROJECTION);
    if(flag) {
        ortho(-w / 20, w / 20, -h / 20, h / 20, -1.0f, 100.0f);
    } else {
        float ratio = (1.0f * w) / h;
        perspective(fov, ratio, 0.1f, 1000.0f);
    }
    is_ortho = flag;
}

void camera::make_moving(bool flag) {
    moving = flag;
}

void camera::draw(scene_manager& scene, VSShaderLib& shader) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load identity matrices
    loadIdentity(VIEW);
    loadIdentity(MODEL);
    // set the camera using a function similar to gluLookAt
    if(moving) {
        float* player = scene.player_pos();
        x = player[0] - 20.0f;
        y = player[1] + 20.0f;
        z = player[2];
        atx = player[0];
        aty = player[1];
        atz = player[2];
    }
    lookAt(x, y, z, atx, aty, atz, upx, upy, upz);

    // use our shader
    glUseProgram(shader.getProgramIndex());

    for(auto const& l : scene.lights)
        l->setup(shader.getProgramIndex(), scene.player_pos());

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    for(auto const& ptr : scene.objs)
        ptr->render(*this, shader);
}

void camera::create_stencil(VSShaderLib &shader) {
    loadIdentity(PROJECTION);

    if(w <= h)
        ortho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
            2.0 * (GLfloat)h / (GLfloat)w, -10, 10);
    else
        ortho(-2.0 * (GLfloat)w / (GLfloat)h,
            2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10, 10);

    loadIdentity(VIEW);
    loadIdentity(MODEL);
    
    glUseProgram(shader.getProgramIndex());
    loadMatrix(MODEL, stencil_obj.get_mesh().transform);
    computeDerivedMatrix(PROJ_VIEW_MODEL);
    glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
    glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
    computeNormalMatrix3x3();
    glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_NEVER, 0x1, 0x1);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

    glBindVertexArray(stencil_obj.get_mesh().vao);
    glDrawElements(stencil_obj.get_mesh().type, stencil_obj.get_mesh().numIndexes, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
