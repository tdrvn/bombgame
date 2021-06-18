#include "headers/viewer.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

int windowWidth;
int windowHeight;
int stillSomething[3][ROWS][COLUMNS];
const float PI = 4 * atan(1);
int tickSpeed = 70;
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

void drawSquare (float x, float y, float side)
{
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
    glVertex2f(x + side, y);
    glVertex2f(x + side, y - side);
    glVertex2f(x, y - side);
    glEnd();
}

void drawEmptySquare (float x, float y, float halfSide)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(x + halfSide, y + halfSide);
    glVertex2f(x + halfSide, y - halfSide);
    glVertex2f(x - halfSide, y - halfSide);
    glVertex2f(x - halfSide, y + halfSide);
    glEnd();
}


void drawLine(float x1, float y1, float x2, float y2) {
	glLineWidth(4.0f);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
    glVertex2f(x2, y2);
	glEnd();
}
void onKeyPress (int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
    {
        tickSpeed = std::max(tickSpeed - 20, 30);
    }
    if(key == GLUT_KEY_DOWN)
    {
        tickSpeed = std::min(tickSpeed + 20, 410);
    }
}
void drawArena (int gameState, GameTable *table)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0.8f, 0.8f, 0.8f);
	glRasterPos2f(-2.0f, 2.0f);
	writeText("\n");
	if (gameState == PLAYING) {
		std::string aux="\n";
		writeText("Still playing!         ");
		glColor3f(0.9f, 0.0f, 0.0f);
		glRasterPos2f(-2.0f, 2.0f);
		aux+=     "                                   ";
		
		writeText((aux+TeamNames[0]).c_str());
		glColor3f(0.8f, 0.8f, 0.8f);
		glRasterPos2f(-2.0f, 2.0f);
		for(int i=0;i<3*TeamNames[0].size();++i)
			aux+=' ';
		
		writeText((aux+" VS ").c_str());
		glColor3f(0.0f, 0.0f, 0.9f);
		glRasterPos2f(-2.0f, 2.0f);
		aux+=          "         ";
		writeText((aux+TeamNames[1]).c_str());
		glColor3f(0.8f, 0.8f, 0.8f);
	} else if (gameState == BLUE_WINS) {
		glColor3f(0.0f, 0.0f, 0.9f);
		glRasterPos2f(-2.0f, 2.0f);
		writeText("\n");
		std::string aux="Game outcome: " + TeamNames[1] + " wins!\n";
		writeText(aux.c_str());
		glColor3f(0.8f, 0.8f, 0.8f);
	} else if (gameState == RED_WINS) {
		glColor3f(0.9f, 0.0f, 0.0f);
		glRasterPos2f(-2.0f, 2.0f);
		writeText("\n");
		std::string aux="Game outcome: " + TeamNames[0] + " wins!\n";
		writeText(aux.c_str());
		glColor3f(0.8f, 0.8f, 0.8f);
	} else if (gameState == DRAW) {
		writeText("Game outcome: DRAW!\n");
	} else {
		writeText("Game outcome: Unknown!\n");
	}
	//printf("%d\n", currentTick);
	char buffer[100] = {'\0'};
	glRasterPos2f(-2.0f, -1.9f);
	sprintf(buffer, "Action speed is %d", tickSpeed);
	writeText(buffer);
	
	
	glRasterPos2f(-0.1f, -1.9f);
	sprintf(buffer, "Ticks left: %d", MAX_TICKS - currentTick);
	writeText(buffer);
	
	
	//printf("printing sth\n");
	//drawEmptySquare(0.0f, 0.0f, 1.5f);
	for(int i = 0; i < ROWS; ++i)
		for(int j = 0; j < COLUMNS; ++j)
		{
			if(DEFAULT_MAP[i][j] != CELL_FREE)
				continue;
			int vc[3] = {0,0,0};
			for(int k=0;k<=2;++k)
				if(_DISTANCE[k][i][j])
					stillSomething[k][i][j] = 1;
			for(int k=0;k<=2;++k)
				vc[k] = stillSomething[k][i][j];
			for(int k=0;k<=2;++k)
				if(!_DISTANCE[k][i][j])
					stillSomething[k][i][j] = 0;
					
			// TODO: for Tudor find colors for all combinations 2^3 !
			
			if(vc[0] == 0 && vc[1] == 0 && vc[2] == 0)
				glColor3f(0.8f, 0.8f, 0.8f);
			if(vc[0] == 1 && vc[1] == 0 && vc[2] == 0)
				glColor3f(0.96f, 0.6f, 0.23f);
			if(vc[0] == 0 && vc[1] == 0 && vc[2] == 1)
				glColor3f(0.1f, 0.9f, 0.1f);
			if(vc[0] == 1 && vc[1] == 0 && vc[2] == 1)
				glColor3f(0.1f, 0.9f, 0.1f);
			//
			if(vc[0] == 0 && vc[1] == 1 && vc[2] == 0)
				glColor3f(0.69f, 0.19f, 0.79f);
			if(vc[0] == 1 && vc[1] == 1 && vc[2] == 0)
				glColor3f(0.69f, 0.19f, 0.79f);
			if(vc[0] == 0 && vc[1] == 1 && vc[2] == 1)
				glColor3f(0.69f, 0.19f, 0.79f);
			if(vc[0] == 1 && vc[1] == 1 && vc[2] == 1)
				glColor3f(0.69f, 0.19f, 0.79f);
			float x = 0 - 1.7f + (float)j*0.034f;
			float y = 0 + 1.7f - (float)i*0.034f;
			drawSquare(x, y, 0.034f);
		}
	for(int i = 0; i < NUMBER_OF_PLAYERS; ++i)
	{
		float val = 1.0f;
		if(table->players[i].invisibleTime)
			val = 0.6f;
		if(table->players[i].respawnTime > 0)
			continue;
		if(table->players[i].team == RED_TEAM)
			glColor4f(0.9f, 0.0f, 0.0f, val);
		else
			glColor4f(0.0f, 0.0f, 0.9f, val);
		int ii=table->players[i].position.row;
		int jj=table->players[i].position.col;
		float x = 0 - 1.7f + (float)jj*0.034f;
		float y = 0 + 1.7f - (float)ii*0.034f;
		x+=0.017;
		y-=0.017;
		drawRegPoly(x, y, 0.038f, 10);
		if(table->players[i].classType == TANK)
		{
			glColor4f(0.0f, 0.8f, 0.0f, val);
			drawRegPoly(x, y, 0.022f, 10);
		}
		if(table->players[i].classType == NINJA)
		{
			//glColor4f(0.87f, 0.37f, 0.96f, val);
			glColor4f(0.0f, 0.0f, 0.0f, val);
			drawRegPoly(x, y, 0.022f, 10);
		}
	}
	glColor3f(0.9f, 0.0f, 0.0f);
	int ii=table->flags[0].position.row;
	int jj=table->flags[0].position.col;
	float x = 0 - 1.7f + (float)jj*0.034f;
	float y = 0 + 1.7f - (float)ii*0.034f;
	x+=0.017;
	y-=0.017;
	drawRegPoly(x, y, 0.034f, 3);
	
	glColor3f(0.0f, 0.0f, 0.9f);
	ii=table->flags[1].position.row;
	jj=table->flags[1].position.col;
	x = 0 - 1.7f + (float)jj*0.034f;
	y = 0 + 1.7f - (float)ii*0.034f;
	x+=0.017;
	y-=0.017;
	drawRegPoly(x, y, 0.034f, 3);
    glFlush();
	glutSwapBuffers();
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
int whatAction = 0;
void actualLoop (int a)
{
	glutTimerFunc(tickSpeed, actualLoop, -1);
	nextTick(whatAction);
	glutPostRedisplay();//
	whatAction++;
	whatAction%=MAX_SPEED;
}
int mainLoop(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Bombgame");
	glutReshapeFunc(onWindowResize);
	glutDisplayFunc(display);
	glutSpecialFunc(onKeyPress);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glutIdleFunc(nextMove);
	//glutMouseFunc(onMouseClick);
	glutTimerFunc(1000, actualLoop, -1);
	glutMainLoop();
	return 0;
}
