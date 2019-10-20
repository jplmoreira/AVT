#include "camera.hpp"
#include "AVTmathLib.h"

camera::camera(int w, int h, float fov,
    float x, float y, float z,
    float upx, float upy, float upz,
    float atx, float aty, float atz) :
    x(x), y(y), z(z),
    upx(upx), upy(upy), upz(upz),
    atx(atx), aty(aty), atz(atz),
    w(w), h(h), fov(fov) {}

void camera::resize(int width, int height) {
    w = width;
    h = height;
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
        l->setup(shader.getProgramIndex());

    for(auto const& ptr : scene.objs)
        ptr->render(*this, shader);
}
