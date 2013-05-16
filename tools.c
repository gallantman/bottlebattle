#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include "generic.h"

int dtoi(double val)
{
	return (int) val;
}

int next(FILE *fp)
{
	char a,b,c;
	a = fgetc(fp);
	b = fgetc(fp);
	c = fgetc(fp);
	fgetc(fp);
	return ((int)(a)-48)*100+((int)(b)-48)*10+((int)(c)-48);
}

int full_spell(struct hero *hero)
{
	int i;
	for (i = 0; i < 5; ++i) {
		if (hero->spell[i] == -1) {
			return 0;
		}
	}
	return 1;
}

void set_hero_spell(struct hero *hero, int spell)
{
	int i;
	for (i = 0; i < 5; ++i) {
		if(hero->spell[i] == -1) {
			hero->spell[i] = spell;
			spell_chosen_map[spell] = 1;
			return;
		}
	}
}

int enough_ball(int i, int j)
{
	int count = 0;
	int m, n;
	for (m = j*5; m < (j+1)*5; ++m) {
		for (n = i*5; n < (i+1)*5; ++n) {
			if (map[m*1500 + n] == 1) {
				++count;
			}
		}
	}

	if (count > 4) {
		return 1;
	}

	return 0;
}

void get_intersect_point(int x, int y, int *resx, int *resy)
{
	int newx = x - 760;
	int newy = y - 540;
	float ratiox = (float) newx / (float) 760;
	float ratioy = (float) newy / (float) 540;
	float tempx = ratiox * 0.812567;
	float tempy = ratioy * 0.57735;
	float finalx = tempx;
	float finaly = 0.70711 + tempy*0.70711;
	float finalz = -0.70711 + tempy*0.70711;
	float fratiox = finalx / -finalz;
	float fratioy = finaly / -finalz;
	*resx = (int) (51.9615*fratiox + camera_pos.x + 0.5);
	*resy = (int) (51.9615*fratioy + camera_pos.y + 0.5);
}

int click_my(int x, int y)
{
	float dist;
	dist = sqrt((x - my->pos.x)*(x - my->pos.x) + (y - my->pos.y)*(y - my->pos.y));
	if (dist <= 6.0) {
		return 1;
	} else {
		return 0;
	}
}

int click_ground(int x, int y)
{
	return (click_enemy_bottle(x, y) == -1) && (click_enemy_tower(x, y) == -1);
}

int click_enemy_bottle(int x, int y)
{
	int i;
	float dist;

	for (i = 0; i < 10; ++i) {
		dist = sqrt((x - enemy[i]->pos.x)*(x - enemy[i]->pos.x) + (y - enemy[i]->pos.y)*(y - enemy[i]->pos.y));
		if (dist <= 6.0) {
			return i;
		}
	}

	return -1;
}

int click_enemy_tower(int x, int y)
{
	int i;
	float dist;

	for (i = 0; i < 10; ++i) {
		dist = sqrt((x - e_tower[i].pos.x)*(x - e_tower[i].pos.x) + (y - e_tower[i].pos.y)*(y - e_tower[i].pos.y));
		if (i == 0 || i == 3 || i == 6) {
			if (dist <= 10 && e_tower[i].blood > 0) {
				return i;
			}
		} else if (i == 1 || i == 4 || i == 7) {
			if (dist <= 12 && e_tower[i].blood > 0) {
				return i;
			}
		} else if (i == 2 || i == 5 || i == 8) {
			if (dist <= 14 && e_tower[i].blood > 0) {
				return i;
			}
		} else if (i == 9) {
			if (dist <= 18 && e_tower[i].blood > 0) {
				return i;
			}
		}
	}

	return -1;
}

int get_level_from_experience(int exp)
{
	int res = 0;

	int i;
	for (i = 0; i < 25; ++i) {
		if (exp >= level_experience[i]) {
			++res;
		}
	}

	return res;
}

float bb_abs(float val)
{
	if (val >= 0) {
		return val;
	}

	return -val;
}

int get_true_damage(int damage, int armor)
{
	float coeff = (float) 1 / (float) 15;
	float ratio = (float) (1 - exp(-coeff*armor)) / (float) (1 + exp(-coeff*armor));
	return (int) ((1 - ratio)*damage);
}

int bullet_in_range(struct bullet *temp) {
	float dist;

	if (temp->party == SENTINEL) {
		if (temp->target_tower != -1) {
			int i = temp->target_tower;
			dist = sqrt((temp->pos.x - e_tower[i].pos.x)*(temp->pos.x - e_tower[i].pos.x) +
				(temp->pos.y - e_tower[i].pos.y)*(temp->pos.y - e_tower[i].pos.y));
			if (dist <= 12) {
				return 1;
			} else {
				return 0;
			}
		} else if (temp->target_bottle != -1) {
			int i = temp->target_bottle;
			dist = sqrt((temp->pos.x - enemy[i]->pos.x)*(temp->pos.x - enemy[i]->pos.x) +
				(temp->pos.y - enemy[i]->pos.y)*(temp->pos.y - enemy[i]->pos.y));
			if (dist <= 8) {
				return 1;
			} else {
				return 0;
			}
		}
	} else if (temp->party == SCOURGE) {
		if (temp->target_tower != -1) {
			int i = temp->target_tower;
			dist = sqrt((temp->pos.x - a_tower[i].pos.x)*(temp->pos.x - a_tower[i].pos.x) +
				(temp->pos.y - a_tower[i].pos.y)*(temp->pos.y - a_tower[i].pos.y));
			if (dist <= 12) {
				return 1;
			} else {
				return 0;
			}
		} else if (temp->target_bottle == 9) {
			dist = sqrt((temp->pos.x - my->pos.x)*(temp->pos.x - my->pos.x) +
				(temp->pos.y - my->pos.y)*(temp->pos.y - my->pos.y));
			if (dist <= 8) {
				return 1;
			} else {
				return 0;
			}
		} else {
			int i = temp->target_bottle;
			dist = sqrt((temp->pos.x - ally[i]->pos.x)*(temp->pos.x - ally[i]->pos.x) +
				(temp->pos.y - ally[i]->pos.y)*(temp->pos.y - ally[i]->pos.y));
			if (dist <= 8) {
				return 1;
			} else {
				return 0;
			}
		}
	}

	return 0;
}

void *bb_malloc(int size) {
	void *res = malloc(size);
	if (!res) {
		fprintf(stderr, "Error in malloc!\n");
		return NULL;
	}
	return res;
}