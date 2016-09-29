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


// L�gg till egna globaler h�r efter behov.
#define numberOfSprites 15
float attractionDistance = 300;
float repulsionDistance = 150;
float alignmentDistance = 100;
float attractionConstant = 0.4;
float repulsionConstant = 0.3;
float alignmentConstant = 0.2;



float DistanceBetween(FPoint p1, FPoint p2)
{
	float h = p1.h - p2.h;
	float v = p1.v - p2.v;
	return sqrt( h * h +  v * v );
}

FPoint NormalizeFP(FPoint p)
{
	FPoint newP;
	float length = sqrt((p.v * p.v) + (p.h * p.h));
	
/*
	if( length < 0.0001 )
	{
		newP.v = 0;
		newP.h = 0;
		return newP;
	}
*/
	newP.v = p.v / length;
	newP.h = p.h / length;
	return newP;
}

FPoint ScalarMultFP(FPoint p, float s)
{
	FPoint newP;
	newP.v = p.v * s;
	newP.h = p.h * s;
	return newP;
}

FPoint AddFP(FPoint p1, FPoint p2)
{
	FPoint newP;
	newP.v = p1.v + p2.v;
	newP.h = p1.h + p2.h;
	return newP;
}

FPoint SubFP(FPoint p1, FPoint p2)
{
	FPoint newP;
	newP.v = p1.v - p2.v;
	newP.h = p1.h - p2.h;
	return newP;
}

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.
	SpriteRec* sp = gSpriteRoot;
	SpriteRec* spritesToSearch[numberOfSprites];

	//Fill searchlist
	int i,j = 0;
	for(i = 0; i < numberOfSprites; i++)
	{
		spritesToSearch[i] = sp;
		sp = sp->next;
	}


	//Attraction
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
			
			float distance = DistanceBetween(spriteToCheck->position, spriteToCheckAgainst->position);

			if(distance > attractionDistance)
				continue;

			group[boidsInGroup++] = spriteToCheckAgainst;
		}

		
		if(boidsInGroup < 2) //Dont touch it if it's alone
			continue;

		//Calculate mass center for the group
		FPoint massPoint = {0,0};
		float totalMass = 0;
		for(j = 0; j < boidsInGroup; ++j)
		{
			massPoint = AddFP(massPoint, ScalarMultFP(group[j]->position, group[j]->attraction));
			totalMass += group[j]->attraction;
		}
		massPoint.h /= totalMass;
		massPoint.v /= totalMass;

		//Change to move towards mass center
		FPoint newDirection;
		newDirection = SubFP(massPoint, spriteToCheck->position);
		newDirection = NormalizeFP(newDirection);
		newDirection = ScalarMultFP(newDirection, attractionConstant);

		spriteToCheck->speed = AddFP(spriteToCheck->speed, newDirection);
		spriteToCheck->speed = ScalarMultFP(NormalizeFP(spriteToCheck->speed), spriteToCheck->absVelocity);
	}


	//Repulsion
	for(i = 0; i < numberOfSprites; ++i)
	{
		SpriteRec* spriteToCheck = spritesToSearch[i];

		for(j = 0; j < numberOfSprites; ++j)
		{
			if(j == i) //Don't check towards self (although you could without breaking much...)
				continue;

			SpriteRec* spriteToCheckAgainst = spritesToSearch[j];
			
			float distance = DistanceBetween(spriteToCheck->position, spriteToCheckAgainst->position);

			if(distance > repulsionDistance)
				continue;

			//If not then change movement towards mass center
			FPoint newDirection;
			newDirection = SubFP(spriteToCheck->position, spriteToCheckAgainst->position);
			newDirection = NormalizeFP(newDirection);

			float repulsionStrength = (repulsionDistance/distance) * repulsionConstant;
			newDirection = ScalarMultFP(newDirection, repulsionStrength);

			spriteToCheck->speed = AddFP(spriteToCheck->speed, newDirection);
			spriteToCheck->speed = ScalarMultFP(NormalizeFP(spriteToCheck->speed), spriteToCheck->absVelocity);
		}
	}


	//Alignment
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
			
			float distance = DistanceBetween(spriteToCheck->position, spriteToCheckAgainst->position);

			if(distance > attractionDistance)
				continue;

			group[boidsInGroup++] = spriteToCheckAgainst;
		}

		
		if(boidsInGroup < 2) //Dont touch it if it's alone
			continue;

		//Calculate average alignment
		FPoint avAlignment = {0,0};
		for(j = 0; j < boidsInGroup; ++j)
		{
			avAlignment = AddFP(avAlignment, NormalizeFP(group[j]->speed));
		}
		avAlignment = ScalarMultFP(NormalizeFP(avAlignment), alignmentConstant);

		//Tilt towards alignment center
		spriteToCheck->speed = AddFP(spriteToCheck->speed, avAlignment);
		spriteToCheck->speed = ScalarMultFP(NormalizeFP(spriteToCheck->speed), spriteToCheck->absVelocity);
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
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
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
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	int i;
	srand(time(NULL));
	for(i = 0; i < numberOfSprites; ++i)
	{
		NewSprite(sheepFace, rand() % 800, rand() % 600, rand() % 4 -2 , rand() % 4 - 2);	
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
