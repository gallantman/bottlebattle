#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"
	
void bb_keyboard(unsigned char key, int x, int y)
{
	if (key == 'w') {
		camera_direction.y += GAIN;
		if (camera_direction.y > 750) {
			camera_direction.y -= GAIN;
		} else {
			camera_pos.y += GAIN;
		}
	}

	if (key == 's') {
		camera_direction.y -= GAIN;
		if (camera_direction.y < -750) {
			camera_direction.y += GAIN;
		} else {
			camera_pos.y -= GAIN;
		}
	}

	if (key == 'a') {
		camera_direction.x -= GAIN;
		if (camera_direction.x < -750) {
			camera_direction.x += GAIN;
		} else {
			camera_pos.x -= GAIN;
		}
	}

	if (key == 'd') {
		camera_direction.x += GAIN;
		if (camera_direction.x > 750) {
			camera_direction.x -= GAIN;
		} else {
			camera_pos.x += GAIN;
		}
	}
}
	
void bb_special(int key, int x, int y) 
{

}

void bb_motion_left(int x, int y)
{
	if (x <= 4) {
		border[2] = 1;
	} else {
		border[2] = 0;
	}

	if (y <= 4) {
		border[0] = 1;
	} else {
		border[0] = 0;
	}

	if (y >= 1075) {
		border[1] = 1;
	} else {
		border[1] = 0;
	}
}

void bb_motion_right(int x, int y)
{
	if (y <= 4) {
		border[0] = 1;
	} else {
		border[0] = 0;
	}

	if (x >= 395) {
		border[3] = 1;
	} else {
		border[3] = 0;
	}

	if (y >= 1075) {
		border[1] = 1;
	} else {
		border[1] = 0;
	}
}

void bb_mouse_left(int button, int press_state, int x, int y)
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
}

int in_border()
{
	int i;
	for (i = 0; i < 4; ++i) {
		if (border[i] == 1) {
			return 1;
		}
	}
	return 0;
}

void bb_mouse_right(int button, int press_state, int x, int y)
{
	int relativex, relativey;
	float ratio;
	if (state == FIGHT && !in_border()) {
		ratio = (float) 1500 / (float) 360;
		relativex = x - 20;
		relativey = (1080 - y) - 700;
		if (x >= 0 && x < 360 && y >= 0 && y < 360) {
			camera_direction.x = ratio*relativex - 750;
			camera_direction.y = ratio*relativey - 750;
			camera_pos.x = camera_direction.x;
			camera_pos.y = camera_direction.y - 51.9615;
		}
	}
}