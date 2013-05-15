#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include "generic.h"
	
void bb_keyboard(unsigned char key, int x, int y)
{
	if (key == '1') {
		if (my->gold >= 1000) {
			my->gold -= 1000;
			++my->own_item[0];
		}
	}

	if (key == '2') {
		if (my->gold >= 1000) {
			my->gold -= 1000;
			++my->own_item[1];
		}
	}

	if (key == '3') {
		if (my->gold >= 1000) {
			my->gold -= 1000;
			++my->own_item[2];
		}
	}

	if (key == '4') {
		if (my->gold >= 1500) {
			my->gold -= 1500;
			++my->own_item[3];
		}
	}

	if (key == '5') {
		if (my->gold >= 1000) {
			my->gold -= 1000;
			++my->own_item[4];
		}
	}

	if (key == '6') {
		if (my->gold >= 6000) {
			my->gold -= 6000;
			++my->own_item[5];
		}
	}

	if (key == '7') {
		if (my->gold >= 100) {
			my->gold -= 100;
			my->blood = my->blood + 400 >= my->full_blood ? my->full_blood : my->blood + 400;
		}
	}

	if (key == '8') {
		if (my->gold >= 100) {
			my->gold -= 100;
			my->blood = my->mana + 150 >= my->full_mana ? my->full_mana : my->mana + 150;
		}
	}

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

	if (key == ' ') {
		camera_direction.x = my->pos.x;
		camera_direction.y = my->pos.y;
		camera_pos.x = camera_direction.x;
		camera_pos.y = camera_direction.y - 51.9615;
		my->state = FOCUSED;
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
		int screeny = 1080 - y;
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
	} else if (state == FIGHT) {
		int screenx = x;
		int screeny = 1080 - y;
		int mapx, mapy;
		int id;
		
		get_intersect_point(screenx, screeny, &mapx, &mapy);
		if (button == GLUT_LEFT_BUTTON) {
			if (click_my(mapx, mapy)) {
				my->state = FOCUSED;
				my->target_bottle = my->target_tower = -1;
				return;
			}
		} else if (button == GLUT_RIGHT_BUTTON && 
			((my->state == ATTACK) || (my->state == FOCUSED))) {
			if (click_ground(mapx, mapy)) {
				float val = sqrt((mapx - my->pos.x)*(mapx - my->pos.x) + (mapy - my->pos.y)*(mapy - my->pos.y));
				float ratiox = (float) (mapx - my->pos.x) / val;
				float ratioy = (float) (mapy - my->pos.y) / val;
				my->speed = BOTTLE_SPEED;
				my->direction.x = my->speed*ratiox;
				my->direction.y = my->speed*ratioy;
				adjust_bottle_angle();
				my->state = FOCUSED;
				my_dstx = mapx;
				my_dsty = mapy;
				my->target_tower = my->target_bottle = -1;
			} else {
				id = click_enemy_bottle(mapx, mapy);
				if (id != -1) {
					float val = sqrt((mapx - my->pos.x)*(mapx - my->pos.x) + (mapy - my->pos.y)*(mapy - my->pos.y));
					float ratiox = (float) (mapx - my->pos.x) / val;
					float ratioy = (float) (mapy - my->pos.y) / val;
					my->speed = BOTTLE_SPEED;
					my->direction.x = my->speed*ratiox;
					my->direction.y = my->speed*ratioy;
					my->state = ATTACK;
					adjust_bottle_angle();
					my->target_tower = -1;
					my->target_bottle = id;
					return;
				}
				id = click_enemy_tower(mapx, mapy);
				if (id != -1) {
					float val = sqrt((mapx - my->pos.x)*(mapx - my->pos.x) + (mapy - my->pos.y)*(mapy - my->pos.y));
					float ratiox = (float) (mapx - my->pos.x) / val;
					float ratioy = (float) (mapy - my->pos.y) / val;
					my->speed = BOTTLE_SPEED;
					my->direction.x = my->speed*ratiox;
					my->direction.y = my->speed*ratioy;
					my->state = ATTACK;
					adjust_bottle_angle();
					my->target_bottle = -1;
					my->target_tower = id;
					return;
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
		if (relativex >= 0 && relativex < 360 && relativey >= 0 && relativey < 360) {
			camera_direction.x = ratio*relativex - 750;
			camera_direction.y = ratio*relativey - 750;
			camera_pos.x = camera_direction.x;
			camera_pos.y = camera_direction.y - 51.9615;
		}
	}
}