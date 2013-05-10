#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	bb_init();
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("bottlebattle");
	glutFullScreen();
	glutDisplayFunc(bb_display);
	glutIdleFunc(bb_idle);
	glutKeyboardFunc(bb_keyboard);
	glutSpecialFunc(bb_special);
	glutMotionFunc(bb_motion);
	glutMainLoop();
	return 0;
}
