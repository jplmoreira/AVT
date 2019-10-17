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
#include "mesh.hpp"
#include "scene_manager.hpp"
#include "camera.hpp"

#define CAPTION "Frogger"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

float fov = 53.13f;
float camX = 0.0f, camY = 50.0f, camZ = 0.0f;
float upX = 1.0f, upY = 0.0f, upZ = 0.0f;
float atX = 0.0f, atY = 0.0f, atZ = 0.0f;

camera cam = camera(WinX, WinY, fov, camX, camY, camZ, upX, upY, upZ, atX, atY, atZ);

scene_manager scene;
VSShaderLib shader;

struct mesh mesh[4];
const int objLen = 4;
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
/// The normal matrix
extern float mNormal3x3[9];

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];

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
    cam.resize(w, h);
}


// ------------------------------------------------------------
//
// Render stufff
//

void renderScene(void) {

    FrameCount++;

    cam.draw(scene, shader);

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
        cam.move_to(0.0f, 50.0f, 0.0f);
        cam.look(0.0f, 0.0f, 0.0f);
        cam.make_ortho(true);
        break;
    case '2':
        cam.move_to(0.0f, 50.0f, 0.0f);
        cam.look(0.0f, 0.0f, 0.0f);
        cam.make_ortho(false);
        break;
    case 'q':
        scene.player_forward();
        break;
    case 'a':
        scene.player_back();
        break;
    case 'o':
        scene.player_left();
        break;
    case 'p':
        scene.player_right();
        break;
    case 'c':
        printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
        break;
    case 'm': glEnable(GL_MULTISAMPLE); break;
    case 'n': glDisable(GL_MULTISAMPLE); break;
    }
}

void processKeysUp(unsigned char key, int xx, int yy) {
    switch(key) {

    case 'q':
    case 'a':
    case 'o':
    case 'p':
        scene.player_stop();
        break;
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
    if(cam.moving) {
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

        cam.move_to(camX, camY, camZ);

        //  uncomment this if not using an idle or refresh func
        //	glutPostRedisplay();
    }
}


void mouseWheel(int wheel, int direction, int x, int y) {

    r += direction * 0.1f;
    if(r < 0.1f)
        r = 0.1f;

    camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camY = r * sin(beta * 3.14f / 180.0f);

    cam.move_to(camX, camY, camZ);

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

    cam.pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
    cam.vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
    cam.normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
    cam.lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");

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

    scene.prepare_scene();

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
    glutKeyboardUpFunc(processKeysUp);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    //glutMouseWheelFunc ( mouseWheel ) ;

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