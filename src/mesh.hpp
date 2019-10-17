#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

struct material {
    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emissive[4];
    float shininess;
    int texCount;
};

struct mesh {
    static const int MAX_TEXTURES = 4;
    GLuint vao;
    GLuint texUnits[MAX_TEXTURES];
    GLuint texTypes[MAX_TEXTURES];
    float transform[16];
    int numIndexes;
    unsigned int type;
    struct material mat;
};