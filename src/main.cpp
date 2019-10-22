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

int n = 0;
void stuff_moving() {
	float* e = scene.wood_pos();
	float f = e[2];
	int f_bis = (int)f;
	float j = (float)WinY; 
	if (0< f< j/15 &&n%2==0) {
		scene.wood_move();
		if (f_bis == 30) {
			n++;
		}
	}
	else {
		scene.wood_inverse_move();
		if (f_bis == -30) {
			n++;
		}
		
	}


}


int i = 0;
void aabb_collision(int i){
	float* wood = scene.wood_pos();
	float woody = wood[2];
	int wood_bis_y = (int)woody;
	float* player = scene.player_pos();
	float playery = player[2];
	float playerx = player[0];
	int player_bis_x = (int)playerx;
	int player_bis_y = (int)playery;
	float* river = scene.river_pos();
	float riverx = river[0];
	float rivery = river[2];
	int river_x = (int)riverx;
	int river_y = (int)rivery;
	float river_up_width = (float)8.700000;
	float river_down_width = (float)1.200000;
	float wood_length = (float)5.000000;

	printf("%lf\n", playerx);


if ((playerx >= riverx+ river_down_width && playerx <= riverx + river_up_width
	&&(playery>woody+wood_length ||playery<woody-wood_length))
	)
{
	scene.player_stop(); //die
}
else if (i==1)
{
	scene.player_back();
}

else if(i==0){
	scene.player_forward();
}


//example aabb
//if (player1.x < player2.x + player2.width &&
//	player1.x + player1.width > player2.x&&
//	player1.y < player2.y + player2.height &&
//	player1.y + player1.height > player2.y)
//{
//	System.out.println("Collision Detected");
//}
	
}


void move_l_limits() {
	float* player = scene.player_pos();
	float playery = player[2];
	int player_bis = (int)playery;
	//printf("%i\n", player_bis);
	if (-30<player_bis && player_bis<31) {
		scene.player_left();
	}
	else {
		scene.player_stop();
	}
}

void move_r_limits() {
	float* player = scene.player_pos();
	float playery = player[2];
	int player_bis = (int)playery;
	//printf("%i\n", player_bis);
	if (-31 < player_bis && player_bis < 30) {
		scene.player_right();
	}
	else {
		scene.player_stop();
	}

}



// ------------------------------------------------------------
//
// Render stuff
//

void renderScene(void) {

    FrameCount++;

    cam.draw(scene, shader);
	stuff_moving();


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
    {
        cam.move_to(0.0f, 50.0f, 0.0f);
        cam.look(0.0f, 0.0f, 0.0f);
        cam.set_up(1.0f, 0.0f, 0.0f);
        cam.make_ortho(true);
        cam.make_moving(false);
    }
    break;
    case '2':
    {
        cam.move_to(0.0f, 50.0f, 0.0f);
        cam.look(0.0f, 0.0f, 0.0f);
        cam.set_up(1.0f, 0.0f, 0.0f);
        cam.make_ortho(false);
        cam.make_moving(false);
    }
    break;
    case '3':
    {
		float* player = scene.player_pos();
        cam.move_to(player[0] - 20.0f, player[1] + 20.0f, player[2]);
        cam.look(player[0], player[1], player[2]);
        cam.set_up(0.0f, 1.0f, 0.0f);
        cam.make_ortho(false);
        cam.make_moving(true);
    }
    break;
    case 'q':
		aabb_collision(0);
        break;
    case 'a':
		aabb_collision(1);
        break;
    case 'o':
		move_l_limits();
        break;
    case 'p':
		move_r_limits();
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

//void processMouseButtons(int button, int state, int xx, int yy) {
//    // start tracking the mouse
//    if(state == GLUT_DOWN) {
//        startX = xx;
//        startY = yy;
//        if(button == GLUT_LEFT_BUTTON)
//            tracking = 1;
//        else if(button == GLUT_RIGHT_BUTTON)
//            tracking = 2;
//    }
//
//    //stop tracking the mouse
//    else if(state == GLUT_UP) {
//        if(tracking == 1) {
//            alpha -= (xx - startX);
//            beta += (yy - startY);
//        }
//        else if(tracking == 2) {
//            r += (yy - startY) * 0.01f;
//            if(r < 0.1f)
//                r = 0.1f;
//        }
//        tracking = 0;
//    }
//}
//
//// Track mouse motion while buttons are pressed
//
//void processMouseMotion(int xx, int yy) {
//    if(cam.moving) {
//        int deltaX, deltaY;
//        float alphaAux = 0.0f, betaAux = 0.0f;
//        float rAux = 0.0f;
//
//        deltaX = -xx + startX;
//        deltaY = yy - startY;
//
//        // left mouse button: move camera
//        if(tracking == 1) {
//
//
//            alphaAux = alpha + deltaX;
//            betaAux = beta + deltaY;
//
//            if(betaAux > 85.0f)
//                betaAux = 85.0f;
//            else if(betaAux < -85.0f)
//                betaAux = -85.0f;
//            rAux = r;
//        }
//        // right mouse button: zoom
//        else if(tracking == 2) {
//
//            alphaAux = alpha;
//            betaAux = beta;
//            rAux = r + (deltaY * 0.01f);
//            if(rAux < 0.1f)
//                rAux = 0.1f;
//        }
//
//        camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
//        camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
//        camY = rAux * sin(betaAux * 3.14f / 180.0f);
//
//        cam.move_to(camX, camY, camZ);
//
//        //  uncomment this if not using an idle or refresh func
//        //	glutPostRedisplay();
//    }
//}
//
//
//void mouseWheel(int wheel, int direction, int x, int y) {
//
//    r += direction * 0.1f;
//    if(r < 0.1f)
//        r = 0.1f;
//
//    camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
//    camY = r * sin(beta * 3.14f / 180.0f);
//
//    cam.move_to(camX, camY, camZ);
//
//    //  uncomment this if not using an idle or refresh func
//    //	glutPostRedisplay();
//}

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
    camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
    camY = r *   						     sin(beta * 3.14f / 180.0f);
	

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
    //glutIdleFunc(renderScene);  // Use it for maximum performance
    glutTimerFunc(0, refresh, 0);    //use it to to get 60 FPS whatever

//	Mouse and Keyboard Callbacks
    glutKeyboardFunc(processKeys);
    glutKeyboardUpFunc(processKeysUp);
   /* glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);*/


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