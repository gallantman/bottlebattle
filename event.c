#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

void bb_idle(void)
{
	refresh();
	glutPostRedisplay();
}

void bb_idle_left(void)
{
	refresh();
	glutPostRedisplay();
}

void bb_idle_right(void)
{
	refresh();
	glutPostRedisplay();
}
	
void bb_keyboard(unsigned char key, int x, int y)
{
	
}
	
void bb_special(int key, int x, int y) 
{

}

void bb_motion(int x, int y)
{

}

void bb_mouse_left(int button, int state, int x, int y)
{
	if (state == CHOOSE) {
		int screenx = x;
		int screeny = 1080-y;
		if (button == GLUT_LEFT_BUTTON) {
			int gridx = screenx/152;
			int gridy = screeny/135;
			int morex = screenx - gridx*152;
			int morey = screeny - gridy*135;
			if (morex >= 44 && morex <= 108 && morey >= 36 && morey <= 100 && spell_chosen_map[gridy*10 + gridx] == 0) {
				set_hero_spell(my, gridy*10 + gridx);
				if (full_spell(my)) {
					state = FIGHT;
				}
			}
		}
	}
	glutSetWindow(RIGHT_WINDOW);
	glutPostRedisplay();
	glutSetWindow(LEFT_WINDOW);
	glutPostRedisplay();
}

void bb_mouse_right(int button, int state, int x, int y)
{

}