#include "object.hpp"

#include "AVTmathLib.h"
#include "camera.hpp"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

object::object(mesh m) : 
    obj_mesh(m), dirx(0.0f), diry(0.0f), dirz(0.0f) {}

void object::add_child(std::unique_ptr<object> obj) {
    child_objs.push_back(std::move(obj));
}

void object::move() {}

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
    pushMatrix(MODEL);
    loadMatrix(MODEL, obj_mesh.transform);

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
        ptr->render(cam, shader);
}