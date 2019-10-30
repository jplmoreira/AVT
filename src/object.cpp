#include "object.hpp"

#include "AVTmathLib.h"
#include "camera.hpp"

#include <limits>

extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

object::object(mesh m) : 
    obj_mesh(m), dirx(0.0f), diry(0.0f), dirz(0.0f),
    velocity(10.0f), last(std::chrono::system_clock::now()),
    current(std::chrono::system_clock::now()), looping(false),
    limit_upx(std::numeric_limits<float>::max()),
    limit_dx(std::numeric_limits<float>::min()),
    limit_upz(std::numeric_limits<float>::max()),
    limit_dz(std::numeric_limits<float>::min()) {}

object::object() {}

void object::add_child(std::shared_ptr<object> obj) {
    child_objs.push_back(std::move(obj));
}

bool object::is_looping() {
    return looping;
}

void object::loop(bool l) {
    looping = l;
}

void object::set_limits(float ux, float dx, float uz, float dz) {
    limit_upx = ux;
    limit_dx = dx;
    limit_upz = uz;
    limit_dz = dz;
}

void object::move(float x, float y, float z) {
    dirx = x;
    diry = y;
    dirz = z;
}

void object::render(camera& cam, VSShaderLib& shader) {
    GLint loc;

    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
    glUniform4fv(loc, 1, obj_mesh.mat.ambient);
    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
    glUniform4fv(loc, 1, obj_mesh.mat.diffuse);
    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
    glUniform4fv(loc, 1, obj_mesh.mat.specular);
    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
    glUniform1f(loc, obj_mesh.mat.shininess);

    current = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current - last;
    float delta = diff.count();

    posx += dirx * delta * velocity;
    posy += diry * delta * velocity;
    posz += dirz * delta * velocity;

    if(posx < limit_dx) {
        if(looping) {
            posx = limit_upx;
            pushMatrix(MODEL);
            translate(MODEL, limit_upx - limit_dx, 0.0f, 0.0f);
            multMatrix(MODEL, obj_mesh.transform);
        } else {
            posx = limit_dx;
            dirx = 0.0f;
            pushMatrix(MODEL);
            loadMatrix(MODEL, obj_mesh.transform);
        }
    } else if(posx > limit_upx) {
        if(looping) {
            posx = limit_dx;
            pushMatrix(MODEL);
            translate(MODEL, limit_dx - limit_upx, 0.0f, 0.0f);
            multMatrix(MODEL, obj_mesh.transform);
        } else {
            posx = limit_upx;
            dirx = 0.0f;
            pushMatrix(MODEL);
            loadMatrix(MODEL, obj_mesh.transform);
        }
    } else if(posz < limit_dz) {
        if(looping) {
            posz = limit_upz;
            pushMatrix(MODEL);
            translate(MODEL, 0.0f, 0.0f, limit_upz - limit_dz);
            multMatrix(MODEL, obj_mesh.transform);
        } else {
            posz = limit_dz;
            dirz = 0.0f;
            pushMatrix(MODEL);
            loadMatrix(MODEL, obj_mesh.transform);
        }
    } else if(posz > limit_upz) {
        if(looping) {
            posz = limit_dz;
            pushMatrix(MODEL);
            translate(MODEL, 0.0f, 0.0f, limit_dz - limit_upz);
            multMatrix(MODEL, obj_mesh.transform);
        } else {
            posz = limit_upz;
            dirz = 0.0f;
            pushMatrix(MODEL);
            loadMatrix(MODEL, obj_mesh.transform);
        }
    } else {
        pushMatrix(MODEL);
        translate(MODEL, dirx * delta * velocity,
            diry * delta * velocity,
            dirz * delta * velocity);
        multMatrix(MODEL, obj_mesh.transform);
    }

    // send matrices to OGL
    computeDerivedMatrix(PROJ_VIEW_MODEL);
    glUniformMatrix4fv(cam.vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
    glUniformMatrix4fv(cam.pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
    computeNormalMatrix3x3();
    glUniformMatrix3fv(cam.normal_uniformId, 1, GL_FALSE, mNormal3x3);

    // stencil test func
    glStencilFunc(stencil_func, 0x1, 0x1);

    // Render mesh
    glBindVertexArray(obj_mesh.vao);

    if(!shader.isProgramValid()) {
        printf("Program Not Valid!\n");
        exit(1);
    }
    glDrawElements(obj_mesh.type, obj_mesh.numIndexes, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    memcpy(obj_mesh.transform, mMatrix[MODEL], 16 * sizeof(float));

    popMatrix(MODEL);

    last = std::chrono::system_clock::now();

    for(auto const& ptr : child_objs)
        ptr->render_child(cam, shader, *this);
}

void object::render_child(camera& cam, VSShaderLib& shader, object& parent) {
    GLint loc;

    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
    glUniform4fv(loc, 1, obj_mesh.mat.ambient);
    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
    glUniform4fv(loc, 1, obj_mesh.mat.diffuse);
    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
    glUniform4fv(loc, 1, obj_mesh.mat.specular);
    loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
    glUniform1f(loc, obj_mesh.mat.shininess);

    pushMatrix(MODEL);
    loadMatrix(MODEL, parent.get_mesh().transform);
    multMatrix(MODEL, obj_mesh.transform);

    // send matrices to OGL
    computeDerivedMatrix(PROJ_VIEW_MODEL);
    glUniformMatrix4fv(cam.vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
    glUniformMatrix4fv(cam.pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
    computeNormalMatrix3x3();
    glUniformMatrix3fv(cam.normal_uniformId, 1, GL_FALSE, mNormal3x3);

    // Render mesh
    glBindVertexArray(obj_mesh.vao);

    if(!shader.isProgramValid()) {
        printf("Program Not Valid!\n");
        exit(1);
    }
    glDrawElements(obj_mesh.type, obj_mesh.numIndexes, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    popMatrix(MODEL);

    for(auto const& ptr : child_objs)
        ptr->render_child(cam, shader, *this);
}

mesh object::get_mesh() const {
    return obj_mesh;
}
