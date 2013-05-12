#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
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
