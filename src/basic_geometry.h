#define MAX_TEXTURES 4

#include "mesh.hpp"

mesh createCube();
mesh createQuad(float size_x, float size_y);
mesh createSphere(float radius, int divisions);
mesh createTorus(float innerRadius, float outerRadius, int rings, int sides);
mesh createCylinder(float height, float radius, int sides);
mesh createCone(float height, float baseRadius, int sides);
mesh createPawn();
mesh computeVAO(int numP, float* p, float* pfloatoints, int sides, float smoothCos);
// create(float* p, int numP, int sides, int closed, float smoothCos);
int revSmoothNormal2(float* p, float* nx, float* ny, float smoothCos, int beginEnd);
float* circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX = 0.0f, float transY = 0.0f);
