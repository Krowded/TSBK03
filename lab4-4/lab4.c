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
#define max(x,y) (x > y) ? x : y
#define numberOfSprites 15
#define BLACKSHEEP 0
#define blackSheep 4

float attractionDistance = 300;
float repulsionDistance = 150;
float alignmentDistance = 100;
float attractionConstant = 0.5;
float repulsionConstant = 0.2;
float alignmentConstant = 2.5;


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
	
	if( length < 0.0001 )
	{
		newP.v = 0;
		newP.h = 0;
		return newP;
	}

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
		if( abs(totalMass) < 0.001 ) //Black sheep together can cause problems
			continue;
		
		massPoint = ScalarMultFP(massPoint, 1/totalMass);

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

			if(distance > repulsionDistance )
				continue;

			distance = max(distance, 0.0001);

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

			if(distance > alignmentDistance)
				continue;

			group[boidsInGroup++] = spriteToCheckAgainst;
		}

		
		if(boidsInGroup < 2) //Dont touch it if it's alone
			continue;

		//Calculate average alignment
		FPoint avAlignment = {0,0};
		float totalMass = 0;
		for(j = 0; j < boidsInGroup; ++j)
		{
			avAlignment = AddFP(avAlignment, ScalarMultFP(NormalizeFP(group[j]->speed), group[j]->attraction));
			totalMass += group[j]->attraction;
		}

		totalMass = max(totalMass, 0.0001);
		avAlignment = ScalarMultFP(NormalizeFP(avAlignment), alignmentConstant/totalMass);

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
		if(sp->kind == 0 && random() % 1000 == 0)
		{
			sp->absVelocity = 1 + (random() % 50 ) / 10;
		}

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
    case '1':
    	attractionDistance++;
    	printf("attractionDistance = %f\n", attractionDistance);
    	break;
    case '2':
    	attractionDistance--;
    	printf("attractionDistance = %f\n", attractionDistance);
    	break;
	case '4':
    	repulsionDistance++;
    	printf("repulsionDistance = %f\n", repulsionDistance);
    	break;
    case '5':
    	repulsionDistance--;
    	printf("repulsionDistance = %f\n", repulsionDistance);
    	break;
    case '7':
    	alignmentDistance++;
    	printf("alignmentDistance = %f\n", alignmentDistance);
    	break;
    case '8':
    	alignmentDistance--;
    	printf("alignmentDistance = %f\n", alignmentDistance);
    	break;
    case 'z':
    	attractionConstant++;
    	printf("attractionConstant = %f\n", attractionConstant);
    	break;
    case 'x':
    	attractionConstant--;
    	printf("attractionConstant = %f\n", attractionConstant);
    	break;
    case 'a':
    	repulsionConstant++;
    	printf("repulsionConstant = %f\n", repulsionConstant);
    	break;
    case 's':
    	repulsionConstant--;
    	printf("repulsionConstant = %f\n", repulsionConstant);
    	break;
    case 'q':
    	alignmentConstant++;
    	printf("alignmentConstant = %f\n", alignmentConstant);
    	break;
    case 'w':
    	alignmentConstant--;
    	printf("alignmentConstant = %f\n", alignmentConstant);
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
		if(i < blackSheep)
		{
			NewBlackSheep(blackFace, rand() % 800, rand() % 600, rand() % 4 -2 , rand() % 4 - 2);
		}
		else
		{
			NewSprite(sheepFace, rand() % 800, rand() % 600, rand() % 4 -2 , rand() % 4 - 2);	
		}
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
