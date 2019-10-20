#include "scene_manager.hpp"

#include "dir_light.hpp"
#include "mesh.hpp"
#include "basic_geometry.h"
#include "AVTmathLib.h"

extern float mMatrix[COUNT_MATRICES][16];

void scene_manager::prepare_scene() {
    // OBJECT CREATION
    loadIdentity(MODEL);

    float amb1[] = { 0.15f, 1.0f, 0.5f, 1.0f };
    float diff1[] = { 0.2f, 1.0f, 0.5f, 1.0f };
    float spec1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float emissive1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess1 = 100.0f;
    int texcount1 = 0;

    material mat1;
    memcpy(mat1.ambient, amb1, 4 * sizeof(float));
    memcpy(mat1.diffuse, diff1, 4 * sizeof(float));
    memcpy(mat1.specular, amb1, 4 * sizeof(float));
    memcpy(mat1.emissive, amb1, 4 * sizeof(float));
    mat1.shininess = shininess1;
    mat1.texCount = texcount1;
    mesh m1 = createSphere(2.0f, 5);
    m1.mat = mat1;

    pushMatrix(MODEL);
    memcpy(m1.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto frog = std::make_unique<object>(m1);

    float amb2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diff2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float spec2[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    material mat2;
    memcpy(mat2.ambient, amb2, 4 * sizeof(float));
    memcpy(mat2.diffuse, diff2, 4 * sizeof(float));
    memcpy(mat2.specular, spec2, 4 * sizeof(float));
    memcpy(mat2.emissive, emissive1, 4 * sizeof(float));
    mat2.shininess = shininess1;
    mat2.texCount = texcount1;
    mesh m2 = createSphere(0.5f, 3);
    m2.mat = mat2;

    pushMatrix(MODEL);
    translate(MODEL, 1.2f, 1.2f, 1.2f);
    memcpy(m2.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto r_eye = std::make_unique<object>(m2);

    pushMatrix(MODEL);
    translate(MODEL, 1.2f, 1.2f, -1.2f);
    memcpy(m2.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto l_eye = std::make_unique<object>(m2);

    frog->add_child(std::move(r_eye));
    frog->add_child(std::move(l_eye));
    objs.push_back(std::move(frog));

    float amb3[] = { 0.2f, 0.15f, 0.1f, 1.0f };
    float diff3[] = { 0.8f, 0.6f, 0.4f, 1.0f };
    float spec3[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    material mat3;
    memcpy(mat3.ambient, amb3, 4 * sizeof(float));
    memcpy(mat3.diffuse, diff3, 4 * sizeof(float));
    memcpy(mat3.specular, spec3, 4 * sizeof(float));
    memcpy(mat3.emissive, emissive1, 4 * sizeof(float));
    mat3.shininess = shininess1;
    mat3.texCount = texcount1;
    mesh m3 = createQuad(10.0f, 10.0f);
    m3.mat = mat3;

    pushMatrix(MODEL);
    translate(MODEL, 0.0f, -2.0f, 0.0f);
    rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
    memcpy(m3.transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    auto floor = std::make_unique<object>(m3);
    objs.push_back(std::move(floor));

    // LIGHT CREATION
    int lightId = 0;

    float dir[3] = { -5.0f, 10.0f, 0.0f };
    auto light = std::make_unique<dir_light>(lightId, true, dir);
    light->color[0] = 0.5f;
    light->color[1] = 0.5f;
    light->color[2] = 0.5f;

    lights.push_back(std::move(light));

    lightId++;
}

void scene_manager::player_forward() {
    objs[0]->move(1.0f, 0.0f, 0.0f);
}

void scene_manager::player_back() {
    objs[0]->move(-1.0f, 0.0f, 0.0f);
}

void scene_manager::player_right() { 
    objs[0]->move(0.0f, 0.0f, 1.0f); 
}

void scene_manager::player_left() {
    objs[0]->move(0.0f, 0.0f, -1.0f);
}

void scene_manager::player_stop() {
    objs[0]->move(0.0f, 0.0f, 0.0f);
}

float* scene_manager::player_pos() {
    float pos[3] = { objs[0]->posx, objs[0]->posy, objs[0]->posz };
    return pos;
}
