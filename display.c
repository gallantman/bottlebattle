#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

void bb_display(void)
{
	glutPostRedisplay();
}

void bb_display_left(void)
{
	int i,j;
	int posx, posy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1520, 0, 1080);
	glClear(GL_COLOR_BUFFER_BIT);
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 6; ++j) {
			glRasterPos2i(200*i, 200*j);
			glDrawPixels(200, 200, GL_BGRA, GL_UNSIGNED_BYTE, grass);
		}
	}
	for (i = 0; i < 80; ++i) {
		posx = 152*(i - (i/10)*10) + 44;
		posy = 135*(i/10) + 36;
		glRasterPos2i(posx, posy);
		glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, spells[i]);
	}
	glutSwapBuffers();
}

void bb_display_right(void)
{
	int i,j;
	int posx, posy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0, 1080);
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	glDrawPixels(400, 1080, GL_BGRA, GL_UNSIGNED_BYTE, wood);
	glRasterPos2i(20, 700);
	glDrawPixels(360, 360, GL_BGRA, GL_UNSIGNED_BYTE, small_map);
	for (i = 0; i < 5; ++i) {
		posx = 80*i + 8;
		posy = 620;
		glRasterPos2i(posx, posy);
		glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
	}
	for (i = 0; i < 4; ++i) {
		posx = 100*i + 18;
		posy = 20;
		glRasterPos2i(posx, posy);
		glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
	}
	for (i = 0; i < 4; ++i) {
		posx = 100*i + 18;
		posy = 100;
		glRasterPos2i(posx, posy);
		glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
	}
	glutSwapBuffers();
}