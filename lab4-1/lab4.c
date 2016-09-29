// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <stdio.h>
#include <math.h>
#include <time.h>


// Lägg till egna globaler här efter behov.
#define numberOfSprites 15
float aCertainDistance = 150;
float distanceThreshold = 1;




float distanceBetween(FPoint p1, FPoint p2)
{
	float h = p1.h - p2.h;
	float v = p1.v - p2.v;
	return sqrt( h * h +  v * v );
}


void SpriteBehavior() // Din kod!
{
// Lägg till din labbkod här. Det går bra att ändra var som helst i
// koden i övrigt, men mycket kan samlas här. Du kan utgå från den
// globala listroten, gSpriteRoot, för att kontrollera alla sprites
// hastigheter och positioner, eller arbeta från egna globaler.
	SpriteRec* sp = gSpriteRoot;
	SpriteRec* spritesToSearch[numberOfSprites];

	//Fill searchlist
	int i,j = 0;
	for(i = 0; i < numberOfSprites; i++)
	{
		spritesToSearch[i] = sp;
		sp = sp->next;
	}

	//Check each sprite
	for(i = 0; i < numberOfSprites; ++i)
	{
		SpriteRec* spriteToCheck = spritesToSearch[i];

		SpriteRec* group[numberOfSprites]; //Making it maximum size cause why nots
		int boidsInGroup = 0;
		group[boidsInGroup++] = spriteToCheck;

		for(j = 0; j < numberOfSprites; ++j)
		{
			if(j == i) //Don't check towards self (although you could without breaking much...)
				continue;

			SpriteRec* spriteToCheckAgainst = spritesToSearch[j];
			
			float distance = distanceBetween(spriteToCheck->position, spriteToCheckAgainst->position);

			if(distance > aCertainDistance)
				continue;ßß

			group[boidsInGroup++] = spriteToCheckAgainst;
		}

		
		if(boidsInGroup < 2) //Dont touch it if it's alone
			continue;

		//Calculate mass center for the group
		FPoint massPoint = {0,0};
		float totalMass = 0;
		for(j = 0; j < boidsInGroup; ++j)
		{
			massPoint.h += group[j]->attraction * group[j]->position.h;
			massPoint.v += group[j]->attraction * group[j]->position.v;
			totalMass += group[j]->attraction; 
		}
		massPoint.h /= totalMass;
		massPoint.v /= totalMass;

		//Check if mass point reached
		float length = distanceBetween(massPoint, spriteToCheck->position);
		if(length < distanceThreshold)
		{
			spriteToCheck->speed.h = 0;
			spriteToCheck->speed.v = 0;
			continue;
		}

		//If not then change movement towards mass center
		FPoint newDirection;
		newDirection.h = massPoint.h - spriteToCheck->position.h;
		newDirection.h /= length;
		newDirection.v = massPoint.v - spriteToCheck->position.v;
		newDirection.v /= length;
		float speed = 1;//group[j]->absVel;
		spriteToCheck->speed.h = newDirection.h*speed;
		spriteToCheck->speed.v = newDirection.v*speed;
	}
}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	aCertainDistance++;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	aCertainDistance--;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett får
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart får
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	int i;
	srand(time(NULL));
	for(i = 0; i < numberOfSprites; ++i)
	{
		NewSprite(sheepFace, rand() % 800, rand() % 600, rand() % 4 - 2 , rand() % 4 - 2);	
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
