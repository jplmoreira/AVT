#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"

#define CAPTION "Frogger"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;



VSShaderLib shader;

struct MyMesh mesh[4];
const int objLen = 4;
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
float lightPos[4] = { 4.0f, 6.0f, 2.0f, 1.0f };

void timer(int value) {
    std::ostringstream oss;
    oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
    std::string s = oss.str();
    glutSetWindow(WindowHandle);
    glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void refresh(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, refresh, 0);
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

    float ratio;
    // Prevent a divide by zero, when window is too short
    if(h == 0)
        h = 1;
    // set the viewport to be the entire window
    glViewport(0, 0, w, h);
    // set the projection matrix
    ratio = (1.0f * w) / h;
    loadIdentity(PROJECTION);
    perspective(53.13f, ratio, 0.1f, 1000.0f);
}


// ------------------------------------------------------------
//
// Render stufff
//

void renderScene(void) {

    GLint loc;

    FrameCount++;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load identity matrices
    loadIdentity(VIEW);
    // set the camera using a function similar to gluLookAt
    lookAt(camX, camY, camZ, 0, 0, 0, 1, 0, 0);
    // use our shader
    glUseProgram(shader.getProgramIndex());

    //send the light position in eye coordinates

        //glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

    float res[4];
    multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
    glUniform4fv(lPos_uniformId, 1, res);

    for(objId = 0; objId < objLen; objId++) {
        // send the material
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);
        pushMatrix(MODEL);
        loadMatrix(MODEL, mesh[objId].transform);

        // send matrices to OGL
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

        // Render mesh
        glBindVertexArray(mesh[objId].vao);

        if(!shader.isProgramValid()) {
            printf("Program Not Valid!\n");
            exit(1);
        }
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        popMatrix(MODEL);
    }

    glutSwapBuffers();
}

// ------------------------------------------------------------
//
// Events from the Keyboard
//

void processKeys(unsigned char key, int xx, int yy) {
    switch(key) {

    case 27:
        glutLeaveMainLoop();
        break;
    case '1':
        camX = 0.0f;
        camY = 10.0f;
        camZ = 0.0f;
        break;
    case 'c':
        printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
        break;
    case 'm': glEnable(GL_MULTISAMPLE); break;
    case 'n': glDisable(GL_MULTISAMPLE); break;
    }
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy) {
    // start tracking the mouse
    if(state == GLUT_DOWN) {
        startX = xx;
        startY = yy;
        if(button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if(button == GLUT_RIGHT_BUTTON)
            tracking = 2;
    }

    //stop tracking the mouse
    else if(state == GLUT_UP) {
        if(tracking == 1) {
            alpha -= (xx - startX);
            beta += (yy - startY);
        }
        else if(tracking == 2) {
            r += (yy - startY) * 0.01f;
            if(r < 0.1f)
                r = 0.1f;
        }
        tracking = 0;
    }
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy) {

    int deltaX, deltaY;
    float alphaAux = 0.0f, betaAux = 0.0f;
    float rAux = 0.0f;

    deltaX = -xx + startX;
    deltaY = yy - startY;

    // left mouse button: move camera
    if(tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if(betaAux > 85.0f)
            betaAux = 85.0f;
        else if(betaAux < -85.0f)
            betaAux = -85.0f;
        rAux = r;
    }
    // right mouse button: zoom
    else if(tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r + (deltaY * 0.01f);
        if(rAux < 0.1f)
            rAux = 0.1f;
    }

    camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
    camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
    camY = rAux * sin(betaAux * 3.14f / 180.0f);

    //  uncomment this if not using an idle or refresh func
    //	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

    r += direction * 0.1f;
    if(r < 0.1f)
        r = 0.1f;

    camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camY = r * sin(beta * 3.14f / 180.0f);

    //  uncomment this if not using an idle or refresh func
    //	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

    // Shader for models
    shader.init();
    shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
    shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

    // set semantics for the shader variables
    glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
    glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
    glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
    //glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

    glLinkProgram(shader.getProgramIndex());

    pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
    vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
    normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
    lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");

    printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

    return(shader.isProgramLinked());
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init() {
    // set the camera position based on its spherical coordinates
    /*camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camY = r *   						     sin(beta * 3.14f / 180.0f);*/
    camX = 0.0f;
    camY = 10.0f;
    camZ = 0.0f;


    float amb1[] = { 0.15f, 1.0f, 0.5f, 1.0f };
    float diff1[] = { 0.2f, 1.0f, 0.5f, 1.0f };
    float spec1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float emissive1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float shininess1 = 100.0f;
    int texcount1 = 0;

    loadIdentity(MODEL);

    // create geometry and VAO of the pawn
    objId = 0;
    memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
    memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
    memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
    memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
    mesh[objId].mat.shininess = shininess1;
    mesh[objId].mat.texCount = texcount1;
    createSphere(2.0f, 5);

    pushMatrix(MODEL);
    memcpy(mesh[objId].transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    float amb2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diff2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float spec2[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    objId = 1;
    memcpy(mesh[objId].mat.ambient, amb2, 4 * sizeof(float));
    memcpy(mesh[objId].mat.diffuse, diff2, 4 * sizeof(float));
    memcpy(mesh[objId].mat.specular, spec2, 4 * sizeof(float));
    memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
    mesh[objId].mat.shininess = shininess1;
    mesh[objId].mat.texCount = texcount1;
    createSphere(0.5f, 3);

    pushMatrix(MODEL);
    translate(MODEL, 1.2f, 1.2f, 1.2f);
    memcpy(mesh[objId].transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    objId = 2;
    memcpy(mesh[objId].mat.ambient, amb2, 4 * sizeof(float));
    memcpy(mesh[objId].mat.diffuse, diff2, 4 * sizeof(float));
    memcpy(mesh[objId].mat.specular, spec2, 4 * sizeof(float));
    memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
    mesh[objId].mat.shininess = shininess1;
    mesh[objId].mat.texCount = texcount1;
    createSphere(0.5f, 3);

    pushMatrix(MODEL);
    translate(MODEL, 1.2f, 1.2f, -1.2f);
    memcpy(mesh[objId].transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    float amb3[] = { 0.2f, 0.15f, 0.1f, 1.0f };
    float diff3[] = { 0.8f, 0.6f, 0.4f, 1.0f };
    float spec3[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    objId = 3;
    memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
    memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
    memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
    memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
    mesh[objId].mat.shininess = shininess1;
    mesh[objId].mat.texCount = texcount1;
    createQuad(10.0f, 10.0f);

    pushMatrix(MODEL);
    translate(MODEL, 0.0f, -2.0f, 0.0f);
    rotate(MODEL, -90.0f, 1.0f, 0.0f, 0.0f);
    memcpy(mesh[objId].transform, mMatrix[MODEL], 16 * sizeof(float));
    popMatrix(MODEL);

    // some GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}


// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char** argv) {

    //  GLUT initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WinX, WinY);
    WindowHandle = glutCreateWindow(CAPTION);


    //  Callback Registration
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutTimerFunc(0, timer, 0);
    glutIdleFunc(renderScene);  // Use it for maximum performance
    //glutTimerFunc(0, refresh, 0);    //use it to to get 60 FPS whatever

//	Mouse and Keyboard Callbacks
    glutKeyboardFunc(processKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    glutMouseWheelFunc ( mouseWheel ) ;



//	return from main loop
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    //	Init GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    if(!setupShaders())
        return(1);

    init();

    //  GLUT main loop
    glutMainLoop();

    return(0);

}

