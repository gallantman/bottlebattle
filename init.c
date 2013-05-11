#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

void bb_init()
{
	get_screen_size();
	load_textures();
	glClearColor(0, 0, 0, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);  
}

void load_textures()
{
	load_grass();
	load_wood();
	load_items();
	load_spells();
	load_map();
	load_blank();
}

void load_blank()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	blank = malloc(sizeof(char) * 64 * 64 * 4);

	fp = fopen("./data/blank","rw+");
	for (j = 0; j < 64; ++j) {
		for (i = 0; i < 64; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			blank[(j*64 + i)*4] = (char) R;
			blank[(j*64 + i)*4 + 1] = (char) G;
			blank[(j*64 + i)*4 + 2] = (char) B;
			blank[(j*64 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);
}

void load_map()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	map = malloc(sizeof(char) * 1500 * 1500);

	fp = fopen("./data/map","rw+");
	for (j = 0; j < 1500; ++j) {
		for (i = 0; i < 1500; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			if (R == 0 && G == 0 && B == 0) {
				map[j*1500 + i] = 1;
			} else {
				map[j*1500 + i] = 0;
			}
		}
	}
	fclose(fp);

	small_map = malloc(sizeof(char) * 360 * 360 * 4);
	fp = fopen("./data/map2","rw+");
	for (j = 0; j < 360; ++j) {
		for (i = 0; i < 360; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			small_map[(j*360 + i)*4] = (char) R;
			small_map[(j*360 + i)*4 + 1] = (char) G;
			small_map[(j*360 + i)*4 + 2] = (char) B;
			small_map[(j*360 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);
}

void load_wood()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	wood = malloc(sizeof(char) * 400 * 1080 * 4);

	fp = fopen("./data/wood","rw+");
	for (j = 0; j < 1080; ++j) {
		for (i = 0; i < 400; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			wood[(j*400 + i)*4] = (char) R;
			wood[(j*400 + i)*4 + 1] = (char) G;
			wood[(j*400 + i)*4 + 2] = (char) B;
			wood[(j*400 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);
}

void load_grass()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	grass = malloc(sizeof(char) * 200 * 200 * 4);

	fp = fopen("./data/grass","rw+");
	for (j = 0; j < 200; ++j) {
		for (i = 0; i < 200; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			grass[(j*200 + i)*4] = (char) R;
			grass[(j*200 + i)*4 + 1] = (char) G;
			grass[(j*200 + i)*4 + 2] = (char) B;
			grass[(j*200 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);
}

void load_items()
{
	FILE *fp = NULL;
	int i,j,k;
	int R,G,B,A;
	char *path_head = "./data/item/";
	char *path = malloc(sizeof(char) * 15);
	strcpy(path, path_head);

	items = malloc(sizeof(char *) * 80);
	for (k = 0; k < 80; ++k) {
		items[k] = malloc(sizeof(char) * 64 * 64 * 4);
	}
	for (k = 11; k < 91; ++k) {
		path[12] = (char) (k/10 + 48);
		path[13] = (char) (k - (k/10)*10 + 48);
		path[14] = '\0';
		fp = fopen(path, "rw+");
		for (j = 0; j < 64; ++j) {
			for (i = 0; i < 64; ++i) {
				R = next(fp);
				G = next(fp);
				B = next(fp);
				A = next(fp);
				items[k-11][(j*64 + i)*4] = (char) R;
				items[k-11][(j*64 + i)*4 + 1] = (char) G;
				items[k-11][(j*64 + i)*4 + 2] = (char) B;
				items[k-11][(j*64 + i)*4 + 3] = (char) A;
			}
		}
		fclose(fp);
	}
}

void load_spells()
{
	FILE *fp = NULL;
	int i,j,k;
	int R,G,B,A;
	char *path_head = "./data/spell/";
	char *path = malloc(sizeof(char) * 16);
	strcpy(path, path_head);

	spells = malloc(sizeof(char *) * 80);
	for (k = 0; k < 80; ++k) {
		spells[k] = malloc(sizeof(char) * 64 * 64 * 4);
	}
	for (k = 11; k < 91; ++k) {
		path[13] = (char) (k/10 + 48);
		path[14] = (char) (k - (k/10)*10 + 48);
		path[15] = '\0';
		fp = fopen(path, "rw+");
		for (j = 0; j < 64; ++j) {
			for (i = 0; i < 64; ++i) {
				R = next(fp);
				G = next(fp);
				B = next(fp);
				A = next(fp);
				spells[k-11][(j*64 + i)*4] = (char) R;
				spells[k-11][(j*64 + i)*4 + 1] = (char) G;
				spells[k-11][(j*64 + i)*4 + 2] = (char) B;
				spells[k-11][(j*64 + i)*4 + 3] = (char) A;
			}
		}
		fclose(fp);
	}
}

void get_screen_size()
{
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	SCALEX = (double) SCREEN_WIDTH / (double) 1920.0;
	SCALEY = (double) SCREEN_HEIGHT / (double) 1080.0;
}
