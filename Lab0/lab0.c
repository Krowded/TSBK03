
// Converted to MicroGlut and VectorUtils3 2013.
// MicroGLut currently exists for Linux and Mac OS X, and a beta for Windows.
// You will also need GLEW for Windows. You may sometimes need to work around
// differences, e.g. additions in MicroGlut that don't exist in FreeGlut.

// 2015: 

// Linux: gcc lab0.c ../common/*.c ../common/Linux/MicroGlut.c -lGL -o lab0 -I../common -I../common/Linux

// Mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
//#include "zpr.h"
#include "LoadTGA.h"

//constants
const int initWidth=512,initHeight=512;

// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
mat4 bunnyWorldMatrix;
mat4 teddyWorldMatrix;
// World-to-view matrix. Usually set by lookAt() or similar.
mat4 viewMatrix;
// Projection matrix, set by a call to perspective().
mat4 projectionMatrix;

// Globals
// * Model(s)
Model *bunny;
Model *teddy;
Model *teapot;
// * Reference(s) to shader program(s)
GLuint program;
// * Texture(s)
GLuint textureBunny;
GLuint textureTeddy;
GLuint textureTeapot;

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	projectionMatrix = perspective(90, 1.0, 0.1, 1000);
	viewMatrix = lookAt(0,0,1.5, 0,0,0, 0,1,0);

	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/teapot.obj");
	teddy = LoadModelPlus("objects/teapot.obj");
	teapot = LoadModelPlus("objects/teapot.obj");
	printError("load models");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga",&textureBunny);
	LoadTGATextureSimple("textures/dirt.tga",&textureTeddy);
	LoadTGATextureSimple("textures/grass.tga",&textureTeapot);
	printError("load textures");


	bunnyWorldMatrix = T(-0.5, 0.0, 0.0);
	teddyWorldMatrix = T(0.5, 0.0, 0.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTeddy);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, textureBunny);
	glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_2D, textureTeapot);
}



GLint isDirectional[] = { 0, 1, 1 };

GLfloat colors[] = { 1.0, 0.0, 0.0,
					0.0, 1.0, 0.0,
					0.0, 0.0, 1.0};

void display(void)
{
	static GLfloat rotation = 0;
	static GLint lastTime = 0;

	GLfloat rotationSpeed = 0.001;
	GLint currentTime = (GLint)glutGet(GLUT_ELAPSED_TIME);
	GLint timeDiff = currentTime - lastTime;
	lastTime = currentTime;
	rotation += timeDiff*rotationSpeed;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//activate the program, and set its variables
	glUseProgram(program);

	GLfloat lightSources[] = { cos(rotation)-1, sin(rotation), 0.5,
						 	   0.0, cos(rotation), sin(rotation),
						       cos(rotation), 0.0, sin(rotation) };

	glUniform1i(glGetUniformLocation(program, "numberOfLightSources"), 1);
	glUniform3fv(glGetUniformLocation(program, "lightSources"), 3, lightSources);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 3, isDirectional);
	glUniform3fv(glGetUniformLocation(program, "colors"), 3, colors);

	
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "inTexture"), GL_TEXTURE0);//the last argument has to be the same as the texture-unit that is to be used
	mat4 m = Mult(viewMatrix, bunnyWorldMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
	mat4 mT = Mult(S(0.5, 0.5, 0.5),Ry(-rotation));
	glUniformMatrix4fv(glGetUniformLocation(program, "worldMatrix"), 1, GL_TRUE, mT.m);
	DrawModel(bunny, program, "in_Position", "in_Normal", "in_TexCoord");

	glActiveTexture(GL_TEXTURE0+1);
	glUniform1i(glGetUniformLocation(program, "inTexture"), GL_TEXTURE0+1);//the last argument has to be the same as the texture-unit that is to be used
	m = Mult(viewMatrix, teddyWorldMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
	mT = Mult(S(0.5, 0.5, 0.5),Ry(rotation));
	glUniformMatrix4fv(glGetUniformLocation(program, "worldMatrix"), 1, GL_TRUE, mT.m);
	DrawModel(teddy, program, "in_Position", "in_Normal", "in_TexCoord");


	GLfloat red[] = {1.0, 0.0, 0.0};
	mat4 teapotWorldMatrix =
	{ { 1.0, 0.0, 0.0, cos(rotation)-1,
	  0.0, 1.0, 0.0, sin(rotation),
	  0.0, 0.0, 1.0, 0.5,
	  0.0, 0.0, 0.0, 1.0 } };

	  glActiveTexture(GL_TEXTURE0+2);
	glUniform1i(glGetUniformLocation(program, "inTexture"), GL_TEXTURE0+2);	
	glUniform3fv(glGetUniformLocation(program, "colors"), 1, red);
	m = Mult(viewMatrix, teapotWorldMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
	mT = Mult(Rz(3*rotation),S(0.1, 0.1, 0.1));
	glUniformMatrix4fv(glGetUniformLocation(program, "worldMatrix"), 1, GL_TRUE, mT.m);
	DrawModel(teapot, program, "in_Position", "in_Normal", "in_TexCoord");
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	GLint windowWidth = 800;
	GLint windowHeight = 600;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab 0 - OpenGL 3.2+ Introduction");
	glutDisplayFunc(display); 
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}

