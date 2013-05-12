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
	MAIN_WINDOW = glutCreateWindow("bottlebattle");
	glutFullScreen();

	glutInitWindowSize(dtoi(1520 * SCALEX), dtoi(1080 * SCALEY));
	LEFT_WINDOW = glutCreateSubWindow(MAIN_WINDOW, 0, 0, dtoi(1520 * SCALEX), dtoi(1080 * SCALEY));
	glutDisplayFunc(bb_display_left);
	glutMouseFunc(bb_mouse_left);
	glutKeyboardFunc(bb_keyboard);
	glutIdleFunc(bb_idle_left);

	glutInitWindowSize(dtoi(400 * SCALEX), dtoi(1080 * SCALEY));
	RIGHT_WINDOW = glutCreateSubWindow(MAIN_WINDOW, dtoi(1520 * SCALEX), 0, dtoi(400 * SCALEX), dtoi(1080 * SCALEY));
	glutDisplayFunc(bb_display_right);
	glutMouseFunc(bb_mouse_right);
	glutKeyboardFunc(bb_keyboard);
	glutIdleFunc(bb_idle_right);

	glutMainLoop();
	return 0;
}
