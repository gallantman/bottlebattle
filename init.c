#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

void bb_init()
{
	glClearColor(0, 0, 0, 0);
	state = CHOOSE;
	light_inited = 0;
	gold_counter = 0;

	light_init();
	init_level();
	init_tower();
	init_left_light();
	init_texture();
	init_bullet_head();
	glutSetWindow(RIGHT_WINDOW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
}

void init_level()
{
	level_experience[0] = 0;
	level_experience[1] = 200;
	level_experience[2] = 600;
	level_experience[3] = 1200;
	level_experience[4] = 2000;
	level_experience[5] = 3000;
	level_experience[6] = 4200;
	level_experience[7] = 5600;
	level_experience[8] = 7200;
	level_experience[9] = 9000;
	level_experience[10] = 11000;
	level_experience[11] = 13200;
	level_experience[12] = 15600;
	level_experience[13] = 18200;
	level_experience[14] = 21000;
	level_experience[15] = 24000;
	level_experience[16] = 27200;
	level_experience[17] = 30600;
	level_experience[18] = 34200;
	level_experience[19] = 38000;
	level_experience[20] = 42000;
	level_experience[21] = 46200;
	level_experience[22] = 50600;
	level_experience[23] = 55200;
	level_experience[24] = 60000;
}

void init_bullet_head()
{
	bullet_head = bb_malloc(sizeof(struct bullet));
	bullet_tail = bb_malloc(sizeof(struct bullet));
	bullet_head->owner_bottle = -1;
	bullet_tail->owner_bottle = -1;
	bullet_head->owner_tower = -1;
	bullet_tail->owner_tower = -1;
	bullet_head->next = bullet_tail;
	bullet_head->prev = NULL;
	bullet_tail->prev = bullet_head;
	bullet_tail->next = NULL;
}

void init_texture()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &grass_texture);
	glBindTexture(GL_TEXTURE_2D, grass_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 200, 200, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass);
}

void light_init()
{
	lig_ambient[0] = 1.0;
	lig_ambient[1] = 1.0;
	lig_ambient[2] = 1.0;
	lig_ambient[3] = 1.0;
	lig_diffuse[0] = 1.0;
	lig_diffuse[1] = 1.0;
	lig_diffuse[2] = 1.0;
	lig_diffuse[3] = 1.0;
	lig_position[0] = camera_pos.x;
	lig_position[1] = camera_pos.y;
	lig_position[2] = camera_pos.z;
	lig_position[3] = 0.0;
	mat_diffuse[0] = 0.8;
	mat_diffuse[1] = 0.8;
	mat_diffuse[2] = 0.8;
	mat_diffuse[3] = 1.0;
	mat_specular[0] = 1.0;
	mat_specular[1] = 1.0;
	mat_specular[2] = 1.0;
	mat_specular[3] = 1.0;
	mat_shininess[0] = 30.0; 
}

void init_soldier_heads()
{
	soldier_count = 0;
	a_head = bb_malloc(sizeof(struct soldier));
	a_tail = bb_malloc(sizeof(struct soldier));
	e_head = bb_malloc(sizeof(struct soldier));
	e_tail = bb_malloc(sizeof(struct soldier));
	a_head->party = -1;
	a_tail->party = -1;
	e_head->party = -1;
	e_tail->party = -1;
	a_head->next = a_tail;
	a_tail->prev = a_head;
	e_head->next = e_tail;
	e_tail->prev = e_head;
}

void init_camera()
{
	camera_pos.x = 0.0;
	camera_pos.y = -51.9615;
	camera_pos.z = 51.9615;
	camera_direction.x = camera_direction.y = camera_direction.z = 0.0;
}

void init_creatures()
{
	int i,j;
	my = bb_malloc(sizeof(struct hero));
	my->level = 1;
	my->pos.x = -0;
	my->pos.y = -0;
	my->pos.z = 5;
	my->direction.x = my->direction.y = my->direction.z = 0;
	my->speed = 0;
	my->damage = 50;
	my->armor = 2;
	my->strength = 25;
	my->agility = 30;
	my->intelligence = 20;
	my->full_blood = my->blood = 200 + my->strength*19;
	my->full_mana = my->mana = 150 + my->intelligence*13;
	my->is_live = 0;
	my->state = NORMAL;
	my->party = SENTINEL;
	my->color.R = 150;
	my->color.G = 50;
	my->color.B = 200;
	my->target_tower = -1;
	my->target_bottle = -1;
	my->experience = 0;
	for (i = 0; i < 5; ++i) {
		my->spell[i] = -1;
	}
	my->item[0] = 32 - 11;
	my->item[1] = 61 - 11;
	my->item[2] = 58 - 11;
	my->item[3] = 41 - 11;
	my->item[4] = 72 - 11;
	my->item[5] = 36 - 11;
	my->item[6] = 80 - 11;
	my->item[7] = 59 - 11;
	my->gold = 853;
	my->kill = 0;
	my->death = 0;
	my->angle = 0.0;
	for (i = 0; i < 8; ++i) {
		my->own_item[i] = 0;
	}
	my->experience = 0;
	my->dummy_counter = 0;
	my->bullet_counter = 0;

	for (j = 0; j < 9; ++j) {
		ally[j] = bb_malloc(sizeof(struct hero));
		ally[j]->level = 1;
		ally[j]->pos.x = -700 + 20*(j+1);
		ally[j]->pos.y = -500 - 20*(j+1);
		ally[j]->pos.z = 5;
		ally[j]->direction.x = ally[j]->direction.y = ally[j]->direction.z = 0;
		ally[j]->speed = 0;
		ally[j]->damage = 50;
		ally[j]->armor = 2;
		ally[j]->strength = 25;
		ally[j]->agility = 30;
		ally[j]->intelligence = 20;
		ally[j]->full_blood = ally[j]->blood = 200 + ally[j]->strength*19;
		ally[j]->full_mana = ally[j]->mana = 150 + ally[j]->intelligence*13;
		ally[j]->is_live = 0;
		ally[j]->state = NORMAL;
		ally[j]->party = SENTINEL;
		ally[j]->color.R = 150;
		ally[j]->color.G = 50;
		ally[j]->color.B = 200;
		for (i = 0; i < 5; ++i) {
			ally[j]->spell[i] = -1;
		}
		for (i = 0; i < 8; ++i) {
			ally[j]->item[i] = -1;
		}
		ally[j]->experience = 0;
		ally[j]->target_bottle = -1;
		ally[j]->target_tower = -1;
		ally[j]->dummy_counter = 0;
		ally[j]->gold = 853;
		ally[j]->kill = 0;
		ally[j]->death = 0;
		ally[j]->angle = 0.0;
		for (i = 0; i < 8; ++i) {
			ally[j]->own_item[i] = 0;
		}
		ally[j]->bullet_counter = 0;
	}

	for (j = 0; j < 10; ++j) {
		enemy[j] = bb_malloc(sizeof(struct hero));
		enemy[j]->level = 1;
		enemy[j]->pos.x = 700 - 20*(j+1);
		enemy[j]->pos.y = 500 + 20*(j+1);
		enemy[j]->pos.z = 5;
		enemy[j]->direction.x = enemy[j]->direction.y = enemy[j]->direction.z = 0;
		enemy[j]->speed = 0;
		enemy[j]->damage = 50;
		enemy[j]->armor = 2;
		enemy[j]->strength = 25;
		enemy[j]->agility = 30;
		enemy[j]->intelligence = 20;
		enemy[j]->full_blood = enemy[j]->blood = 200 + enemy[j]->strength*19;
		enemy[j]->full_mana = enemy[j]->mana = 150 + enemy[j]->intelligence*13;
		enemy[j]->is_live = 0;
		enemy[j]->state = NORMAL;
		enemy[j]->party = SENTINEL;
		enemy[j]->color.R = 150;
		enemy[j]->color.G = 50;
		enemy[j]->color.B = 200;
		for (i = 0; i < 5; ++i) {
			enemy[j]->spell[i] = -1;
		}
		for (i = 0; i < 8; ++i) {
			enemy[j]->item[i] = -1;
		}
		enemy[j]->experience = 0;
		enemy[j]->target_tower = -1;
		enemy[j]->target_bottle = -1;
		enemy[j]->dummy_counter = 0;
		enemy[j]->gold = 853;
		enemy[j]->kill = 0;
		enemy[j]->death = 0;
		enemy[j]->angle = 0.0;
		for (i = 0; i < 8; ++i) {
			enemy[j]->own_item[i] = 0;
		}
		enemy[j]->bullet_counter = 0;
	}
}

void load_textures()
{
	load_grass();
	load_wood();
	load_items();
	load_spells();
	load_map();
	load_blank();
	load_num();
	load_color_dot();
}

void load_blank()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	blank = bb_malloc(sizeof(char) * 64 * 64 * 4);

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

void load_num()
{
	FILE *fp = NULL;
	int i,j,k;
	int R,G,B,A;
	char *path;
	path = bb_malloc(sizeof(char)*13);
	memcpy(path, "./data/num/", sizeof(char)*12);

	for (k = 0; k < 10; ++k) {
		num[k] = bb_malloc(sizeof(char) * 50 * 50 * 4);
		path[11] = (char) (k + 48);
		path[12] = '\0';
		fp = fopen(path, "rw+");
		for (j = 0; j < 50; ++j) {
			for (i = 0; i < 50; ++i) {
				R = next(fp);
				G = next(fp);
				B = next(fp);
				A = next(fp);
				num[k][(j*50 + i)*4] = (char) R;
				num[k][(j*50 + i)*4 + 1] = (char) G;
				num[k][(j*50 + i)*4 + 2] = (char) B;
				num[k][(j*50 + i)*4 + 3] = (char) A;
			}
		}
		fclose(fp);
	}
}

void load_map()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	map = bb_malloc(sizeof(char) * 1500 * 1500);

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

	map2 = bb_malloc(sizeof(char) * 300 * 300);

	for (j = 0; j < 300; ++j) {
		for (i = 0; i < 300; ++i) {
			if (enough_ball(i, j)) {
				map2[j*300 + i] = 1;
			} else {
				map2[j*300 + i] = 0;
			}
		}
	}

	small_map = bb_malloc(sizeof(char) * 360 * 360 * 4);
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
	wood = bb_malloc(sizeof(char) * 400 * 1080 * 4);

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
	grass = bb_malloc(sizeof(char) * 200 * 200 * 4);

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
	char *path = bb_malloc(sizeof(char) * 15);
	strcpy(path, path_head);

	items = bb_malloc(sizeof(char *) * 80);
	for (k = 0; k < 80; ++k) {
		items[k] = bb_malloc(sizeof(char) * 64 * 64 * 4);
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
	char *path = bb_malloc(sizeof(char) * 16);
	strcpy(path, path_head);

	spells = bb_malloc(sizeof(char *) * 80);
	for (k = 0; k < 80; ++k) {
		spells[k] = bb_malloc(sizeof(char) * 64 * 64 * 4);
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

void load_color_dot()
{
	FILE *fp = NULL;
	int i,j;
	int R,G,B,A;
	red = bb_malloc(sizeof(char) * 20 * 20 * 4);
	blue = bb_malloc(sizeof(char) * 20 * 20 * 4);
	green = bb_malloc(sizeof(char) * 20 * 20 * 4);

	fp = fopen("./data/red","rw+");
	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 20; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			red[(j*20 + i)*4] = (char) R;
			red[(j*20 + i)*4 + 1] = (char) G;
			red[(j*20 + i)*4 + 2] = (char) B;
			red[(j*20 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);

	fp = fopen("./data/blue","rw+");
	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 20; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			blue[(j*20 + i)*4] = (char) R;
			blue[(j*20 + i)*4 + 1] = (char) G;
			blue[(j*20 + i)*4 + 2] = (char) B;
			blue[(j*20 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);

	fp = fopen("./data/green","rw+");
	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 20; ++i) {
			R = next(fp);
			G = next(fp);
			B = next(fp);
			A = next(fp);
			green[(j*20 + i)*4] = (char) R;
			green[(j*20 + i)*4 + 1] = (char) G;
			green[(j*20 + i)*4 + 2] = (char) B;
			green[(j*20 + i)*4 + 3] = (char) A;
		}
	}
	fclose(fp);
}

void get_screen_size()
{
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	SCALEX = (double) SCREEN_WIDTH / (double) 1920.0;
	SCALEY = (double) SCREEN_HEIGHT / (double) 1080.0;
}

void init_tower()
{
	int i;

	a_tower[0].size = 10;
	a_tower[0].pos.x = -580;
	a_tower[0].pos.y = 334;
	a_tower[0].pos.z = a_tower[0].size/2;
	a_tower[0].damage = 140;
	a_tower[0].armor = 2;
	a_tower[0].blood = a_tower[0].full_blood = 1450;
	a_tower[0].party = SCOURGE;
	a_tower[0].direction = 90;
	a_tower[0].state = REST;
	a_tower[0].target = -1;
	a_tower[0].bullet_counter = 0;

	a_tower[1].size = 12;
	a_tower[1].pos.x = -586;
	a_tower[1].pos.y = 14;
	a_tower[1].pos.z = a_tower[1].size/2;
	a_tower[1].damage = 180;
	a_tower[1].armor = 5;
	a_tower[1].blood = a_tower[1].full_blood = 1850;
	a_tower[1].party = SCOURGE;
	a_tower[1].direction = 90;
	a_tower[1].state = REST;
	a_tower[1].target = -1;
	a_tower[1].bullet_counter = 0;

	a_tower[2].size = 14;
	a_tower[2].pos.x = -572;
	a_tower[2].pos.y = -452;
	a_tower[2].pos.z = a_tower[2].size/2;
	a_tower[2].damage = 180;
	a_tower[2].armor = 15;
	a_tower[2].blood = a_tower[2].full_blood = 2350;
	a_tower[2].party = SCOURGE;
	a_tower[2].direction = 90;
	a_tower[2].state = REST;
	a_tower[2].target = -1;
	a_tower[2].bullet_counter = 0;

	a_tower[3].size = 10;
	a_tower[3].pos.x = -70;
	a_tower[3].pos.y = -96;
	a_tower[3].pos.z = a_tower[3].size/2;
	a_tower[3].damage = 140;
	a_tower[3].armor = 2;
	a_tower[3].blood = a_tower[3].full_blood = 1450;
	a_tower[3].party = SCOURGE;
	a_tower[3].direction = 45;
	a_tower[3].state = REST;
	a_tower[3].target = -1;
	a_tower[3].bullet_counter = 0;

	a_tower[4].size = 12;
	a_tower[4].pos.x = -218;
	a_tower[4].pos.y = -290;
	a_tower[4].pos.z = a_tower[4].size/2;
	a_tower[4].damage = 180;
	a_tower[4].armor = 5;
	a_tower[4].blood = a_tower[4].full_blood = 1850;
	a_tower[4].party = SCOURGE;
	a_tower[4].direction = 45;
	a_tower[4].state = REST;
	a_tower[4].target = -1;
	a_tower[4].bullet_counter = 0;

	a_tower[5].size = 14;
	a_tower[5].pos.x = -394;
	a_tower[5].pos.y = -476;
	a_tower[5].pos.z = a_tower[5].size/2;
	a_tower[5].damage = 180;
	a_tower[5].armor = 15;
	a_tower[5].blood = a_tower[5].full_blood = 2350;
	a_tower[5].party = SCOURGE;
	a_tower[5].direction = 45;
	a_tower[5].state = REST;
	a_tower[5].target = -1;
	a_tower[5].bullet_counter = 0;

	a_tower[6].size = 10;
	a_tower[6].pos.x = 436;
	a_tower[6].pos.y = -586;
	a_tower[6].pos.z = a_tower[6].size/2;
	a_tower[6].damage = 140;
	a_tower[6].armor = 2;
	a_tower[6].blood = a_tower[6].full_blood = 1450;
	a_tower[6].party = SCOURGE;
	a_tower[6].direction = 0;
	a_tower[6].state = REST;
	a_tower[6].target = -1;
	a_tower[6].bullet_counter = 0;

	a_tower[7].size = 12;
	a_tower[7].pos.x = 54;
	a_tower[7].pos.y = -664;
	a_tower[7].pos.z = a_tower[7].size/2;
	a_tower[7].damage = 180;
	a_tower[7].armor = 5;
	a_tower[7].blood = a_tower[7].full_blood = 1850;
	a_tower[7].party = SCOURGE;
	a_tower[7].direction = 0;
	a_tower[7].state = REST;
	a_tower[7].target = -1;
	a_tower[7].bullet_counter = 0;

	a_tower[8].size = 14;
	a_tower[8].pos.x = -334;
	a_tower[8].pos.y = -656;
	a_tower[8].pos.z = a_tower[8].size/2;
	a_tower[8].damage = 180;
	a_tower[8].armor = 15;
	a_tower[8].blood = a_tower[8].full_blood = 2350;
	a_tower[8].party = SCOURGE;
	a_tower[8].direction = 0;
	a_tower[8].state = REST;
	a_tower[8].target = -1;
	a_tower[8].bullet_counter = 0;

	a_tower[9].size = 18;
	a_tower[9].pos.x = -616;
	a_tower[9].pos.y = -624;
	a_tower[9].pos.z = a_tower[9].size/2;
	a_tower[9].damage = 300;
	a_tower[9].armor = 30;
	a_tower[9].blood = a_tower[9].full_blood = 4750;
	a_tower[9].party = SCOURGE;
	a_tower[9].direction = 45;
	a_tower[9].state = REST;
	a_tower[9].target = -1;
	a_tower[9].bullet_counter = 0;

	e_tower[0].size = 10;
	e_tower[0].pos.x = -414;
	e_tower[0].pos.y = 448;
	e_tower[0].pos.z = e_tower[0].size/2;
	e_tower[0].damage = 140;
	e_tower[0].armor = 2;
	e_tower[0].blood = e_tower[0].full_blood = 1450;
	e_tower[0].party = SCOURGE;
	e_tower[0].direction = 180;
	e_tower[0].state = REST;
	e_tower[0].target = -1;
	e_tower[0].bullet_counter = 0;

	e_tower[1].size = 12;
	e_tower[1].pos.x = -85;
	e_tower[1].pos.y = 520;
	e_tower[1].pos.z = e_tower[1].size/2;
	e_tower[1].damage = 180;
	e_tower[1].armor = 5;
	e_tower[1].blood = e_tower[1].full_blood = 1850;
	e_tower[1].party = SCOURGE;
	e_tower[1].direction = 180;
	e_tower[1].state = REST;
	e_tower[1].target = -1;
	e_tower[1].bullet_counter = 0;

	e_tower[2].size = 14;
	e_tower[2].pos.x = 242;
	e_tower[2].pos.y = 578;
	e_tower[2].pos.z = e_tower[2].size/2;
	e_tower[2].damage = 180;
	e_tower[2].armor = 15;
	e_tower[2].blood = e_tower[2].full_blood = 2350;
	e_tower[2].party = SCOURGE;
	e_tower[2].direction = 180;
	e_tower[2].state = REST;
	e_tower[2].target = -1;
	e_tower[2].bullet_counter = 0;

	e_tower[3].size = 10;
	e_tower[3].pos.x = 42;
	e_tower[3].pos.y = 58;
	e_tower[3].pos.z = e_tower[3].size/2;
	e_tower[3].damage = 140;
	e_tower[3].armor = 2;
	e_tower[3].blood = e_tower[3].full_blood = 1450;
	e_tower[3].party = SCOURGE;
	e_tower[3].direction = 225;
	e_tower[3].state = REST;
	e_tower[3].target = -1;
	e_tower[3].bullet_counter = 0;

	e_tower[4].size = 12;
	e_tower[4].pos.x = 218;
	e_tower[4].pos.y = 186;
	e_tower[4].pos.z = e_tower[4].size/2;
	e_tower[4].damage = 180;
	e_tower[4].armor = 5;
	e_tower[4].blood = e_tower[4].full_blood = 1850;
	e_tower[4].party = SCOURGE;
	e_tower[4].direction = 225;
	e_tower[4].state = REST;
	e_tower[4].target = -1;
	e_tower[4].bullet_counter = 0;

	e_tower[5].size = 14;
	e_tower[5].pos.x = 412;
	e_tower[5].pos.y = 416;
	e_tower[5].pos.z = e_tower[5].size/2;
	e_tower[5].damage = 180;
	e_tower[5].armor = 15;
	e_tower[5].blood = e_tower[5].full_blood = 2350;
	e_tower[5].party = SCOURGE;
	e_tower[5].direction = 225;
	e_tower[5].state = REST;
	e_tower[5].target = -1;
	e_tower[5].bullet_counter = 0;

	e_tower[6].size = 10;
	e_tower[6].pos.x = 592;
	e_tower[6].pos.y = -358;
	e_tower[6].pos.z = e_tower[6].size/2;
	e_tower[6].damage = 140;
	e_tower[6].armor = 2;
	e_tower[6].blood = e_tower[6].full_blood = 1450;
	e_tower[6].party = SCOURGE;
	e_tower[6].direction = 270;
	e_tower[6].state = REST;
	e_tower[6].target = -1;
	e_tower[6].bullet_counter = 0;

	e_tower[7].size = 12;
	e_tower[7].pos.x = 586;
	e_tower[7].pos.y = -52;
	e_tower[7].pos.z = e_tower[7].size/2;
	e_tower[7].damage = 180;
	e_tower[7].armor = 5;
	e_tower[7].blood = e_tower[7].full_blood = 1850;
	e_tower[7].party = SCOURGE;
	e_tower[7].direction = 270;
	e_tower[7].state = REST;
	e_tower[7].target = -1;
	e_tower[7].bullet_counter = 0;

	e_tower[8].size = 14;
	e_tower[8].pos.x = 598;
	e_tower[8].pos.y = 276;
	e_tower[8].pos.z = e_tower[8].size/2;
	e_tower[8].damage = 180;
	e_tower[8].armor = 15;
	e_tower[8].blood = e_tower[8].full_blood = 2350;
	e_tower[8].party = SCOURGE;
	e_tower[8].direction = 270;
	e_tower[8].state = REST;
	e_tower[8].target = -1;
	e_tower[8].bullet_counter = 0;

	e_tower[9].size = 18;
	e_tower[9].pos.x = 666;
	e_tower[9].pos.y = 643;
	e_tower[9].pos.z = e_tower[9].size/2;
	e_tower[9].damage = 300;
	e_tower[9].armor = 30;
	e_tower[9].blood = e_tower[9].full_blood = 4750;
	e_tower[9].party = SCOURGE;
	e_tower[9].direction = 225;
	e_tower[9].state = REST;
	e_tower[9].target = -1;
	e_tower[9].bullet_counter = 0;

	for (i = 0; i < 10; ++i) {
		a_tower[i].direction2.x = 0;
		a_tower[i].direction2.y = 0;
		a_tower[i].direction2.z = 0;
		e_tower[i].direction2.x = 0;
		e_tower[i].direction2.y = 0;
		e_tower[i].direction2.z = 0;
	}
}