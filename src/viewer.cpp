#pragma once
#include "headers/viewer.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

int windowWidth;
int windowHeight;

const float PI = 4 * atan(1);

void writeText(const char* string) {
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)string);
}

void drawRegPoly(float x, float y, float r, int edges) {
 	glBegin(GL_POLYGON);
	for (int i = 0; i < edges; i++) {
		float phi = i * 2.0f * PI / edges;
		glVertex2f(x + r * cos(phi), y + r * sin(phi));
	}
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
    glVertex2f(x2, y2);
	glEnd();
}
void drawArena (int gameState, GameTable *table)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0.8f, 0.8f, 0.8f);
	glRasterPos2f(-2.0f, 2.0f);
	writeText("\n");
	if (gameState == PLAYING) {
		writeText("Still playing!n");
	} else if (gameState == BLUE_WINS) {
		writeText("Game outcome: BLUE wins!\n");
	} else if (gameState == RED_WINS) {
		writeText("Game outcome: RED wins!\n");
	} else {
		writeText("Game outcome: Unknown!\n");
	}
	// TODO:
	// drawing the board and stuff
}
void onWindowResize(int w, int h) {
  //printf("onWindowResize %d %d\n", w, h);
  windowWidth = w;
  windowHeight = h;
  
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
    glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
      2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
  else
    glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
      2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
}
void actualLoop (int a)
{
	glutTimerFunc(100, actualLoop, -1);
	nextTick();
	glutPostRedisplay();
}
int mainLoop(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Bombgame");
	glutReshapeFunc(onWindowResize);
	glutDisplayFunc(display);
	//glutIdleFunc(nextMove);
	//glutMouseFunc(onMouseClick);
	glutTimerFunc(100, actualLoop, -1);
	glutMainLoop();
	return 0;
}
