#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

int main(int argc, char **argv)
{
	int i;
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	get_screen_size();
	for (i = 0; i < 80; ++i) {
		spell_chosen_map[i] = 0;
	}
	init_camera();
	init_soldier_heads();
	load_textures();
	init_creatures();

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	MAIN_WINDOW = glutCreateWindow("bottlebattle");
	glutFullScreen();
	glutTimerFunc(50, bb_display, 0);

	glutInitWindowSize(dtoi(1520 * SCALEX), dtoi(1080 * SCALEY));
	LEFT_WINDOW = glutCreateSubWindow(MAIN_WINDOW, 0, 0, dtoi(1520 * SCALEX), dtoi(1080 * SCALEY));
	glutDisplayFunc(bb_display_left);
	glutMouseFunc(bb_mouse_left);
	glutKeyboardFunc(bb_keyboard);
	glutMotionFunc(bb_motion_left);
	glutPassiveMotionFunc(bb_motion_left);

	glutInitWindowSize(dtoi(400 * SCALEX), dtoi(1080 * SCALEY));
	RIGHT_WINDOW = glutCreateSubWindow(MAIN_WINDOW, dtoi(1520 * SCALEX), 0, dtoi(400 * SCALEX), dtoi(1080 * SCALEY));
	glutDisplayFunc(bb_display_right);
	glutMouseFunc(bb_mouse_right);
	glutKeyboardFunc(bb_keyboard);
	glutMotionFunc(bb_motion_right);
	glutPassiveMotionFunc(bb_motion_right);

	bb_init();
	glutMainLoop();
	return 0;
}
