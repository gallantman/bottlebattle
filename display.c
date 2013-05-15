#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include "generic.h"

void bb_display(int value)
{
	refresh();
	test_exit();
	glutSetWindow(RIGHT_WINDOW);
	glutPostRedisplay();
	glutSetWindow(LEFT_WINDOW);
	glutPostRedisplay();
	glutTimerFunc(50, bb_display, 0);
}

void test_exit()
{
	int i;
	int res = 0;

	for (i = 0; i < 10; ++i) {
		if (a_tower[i].blood <= 0) {
			++res;
		}
	}

	if (res == 10) {
		exit(0);
	}

	res = 0;

	for (i = 0; i < 10; ++i) {
		if (e_tower[i].blood <= 0) {
			++res;
		}
	}

	if (res == 10) {
		exit(0);
	}
}

void bb_display_left(void)
{
	if (state == CHOOSE) {
		int i,j;
		int posx, posy;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 1520, 0, 1080);
		glClear(GL_COLOR_BUFFER_BIT);
		for (i = 0; i < 8; ++i) {
			for (j = 0; j < 6; ++j) {
				glRasterPos2i(200*i, 200*j);
				glDrawPixels(200, 200, GL_BGRA, GL_UNSIGNED_BYTE, grass);
			}
		}
		for (i = 0; i < 80; ++i) {
			posx = 152*(i - (i/10)*10) + 44;
			posy = 135*(i/10) + 36;
			glRasterPos2i(posx, posy);
			if (spell_chosen_map[i] == 0) {
				glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, spells[i]);
			} else {
				glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
			}
		}
		glutSwapBuffers();
		glFlush();
	} else if (state == FIGHT) {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		set_view();
		render_map();
		render_tower();
		render_my();
		render_fighters();
		render_bullets();
		glutSwapBuffers();
	}
}

void render_my()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(my->pos.x, my->pos.y, my->pos.z);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(my->angle, 0.0, 1.0, 0.0);
	glutSolidTeapot(6);
	glPopMatrix();
}

void render_bullets()
{
	struct bullet *temp = bullet_head->next;

	while (temp != bullet_tail) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(temp->pos.x, temp->pos.y, temp->pos.z);
		glutSolidSphere(0.5, 8, 8);
		glPopMatrix();
		temp = temp->next;
	}
}

void render_fighters()
{
	int i;

	for (i = 0; i < 9 ; ++i) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(ally[i]->pos.x, ally[i]->pos.y, ally[i]->pos.z);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glRotatef(ally[i]->angle, 0.0, 1.0, 0.0);
		glutSolidTeapot(6);
		glPopMatrix();
	}

	for (i = 0; i < 10 ; ++i) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(enemy[i]->pos.x, enemy[i]->pos.y, enemy[i]->pos.z);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glRotatef(enemy[i]->angle, 0.0, 1.0, 0.0);
		glutSolidTeapot(6);
		glPopMatrix();
	}
}

void render_tower()
{
	int i;

	for (i = 0; i < 10; ++i) {
		if (e_tower[i].blood > 0) {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(e_tower[i].pos.x, e_tower[i].pos.y, e_tower[i].pos.z + 5);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glRotatef(e_tower[i].direction, 0.0, 1.0, 0.0);
			glutSolidTeapot(e_tower[i].size);
			glPopMatrix();
		}
		
		if (a_tower[i].blood > 0) {
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(a_tower[i].pos.x, a_tower[i].pos.y, a_tower[i].pos.z + 5);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glRotatef(a_tower[i].direction, 0.0, 1.0, 0.0);
			glutSolidTeapot(a_tower[i].size);
			glPopMatrix();
		}
	}
}

void set_view()
{
	glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();
	gluPerspective(60.0, 1.40741, 20.0, 210.0);
	gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, 
		camera_direction.x, camera_direction.y, camera_direction.z, 0.0, 0.70711, 0.70711);
}

void render_map()
{
	render_grass();
	render_bottles();
}

void render_grass()
{
	int i,j;
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();
	glEnable(GL_NORMALIZE);

	glBindTexture(GL_TEXTURE_2D, grass_texture);
	glEnable(GL_TEXTURE_2D);
	for (j = -40; j < 40; ++j) {
		for (i = -40; i < 40; ++i) {
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(0.0 + 25.0*i, 0.0 + 25.0*j, 0.0);
			glTexCoord2f(0.0, 1.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(0.0 + 25.0*i, 25.0 + 25.0*j, 0.0);
			glTexCoord2f(1.0, 1.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(25.0 + 25.0*i, 25.0 + 25.0*j, 0.0);
			glTexCoord2f(1.0, 0.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(25.0 + 25.0*i, 0.0 + 25.0*j, 0.0);
			glEnd();
		}
	}
}

void render_bottles()
{
	int i,j;
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	for (j = (int) (camera_direction.y - 60); j < (int) (camera_direction.y + 130); ++j) {
		for (i = (int) (camera_direction.x - 130); i < (int) (camera_direction.x + 130); ++i) {
			if (!(i%5 == 0) || !(j%5 == 0)) {
				continue;
			}
			if (map2[(j/5+150)*300 + i/5 + 150] == 1) {
				glPushMatrix();
				glTranslatef(1.0*i + 2.5, 1.0*j + 2.5, 3.0);
				glutSolidSphere(3.0, 20, 20);
				glPopMatrix();
			}
		}
	}
}

void bb_display_right(void)
{
	if (state == CHOOSE) {
		right_render_choose();
	} else if (state == FIGHT) {
		right_render_choose();
	}
}

void right_render_choose()
{
	int i;
	int posx, posy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRasterPos2i(0, 0);
	glDrawPixels(400, 1080, GL_BGRA, GL_UNSIGNED_BYTE, wood);
	glRasterPos2i(20, 700);
	glDrawPixels(360, 360, GL_BGRA, GL_UNSIGNED_BYTE, small_map);
	for (i = 0; i < 5; ++i) {
		posx = 80*i + 8;
		posy = 620;
		glRasterPos2i(posx, posy);
		if (my->spell[i] == -1) {
			glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
		} else {
			glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, spells[my->spell[i]]);
		}
	}
	for (i = 0; i < 4; ++i) {
		posx = 100*i + 18;
		posy = 100;
		glRasterPos2i(posx, posy);
		if (my->item[i] == -1){
			glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
		} else {
			glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, items[my->item[i]]);
		}
	}
	for (i = 0; i < 4; ++i) {
		posx = 100*i + 18;
		posy = 20;
		glRasterPos2i(posx, posy);
		if (my->item[i + 4] == -1) {
			glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, blank);
		} else {
			glDrawPixels(64, 64, GL_BGRA, GL_UNSIGNED_BYTE, items[my->item[i + 4]]);
		}
		
	}
	glutSwapBuffers();
	glFlush();
}

void refresh()
{
	generate_bullet();
	refresh_border();
	refresh_position();
	hit_system();
	adjust_bottle_target();
	adjust_bottle_angle();
	adjust_tower_angle();
	refresh_bottle_status();
}

void refresh_bottle_status()
{
	int i;
	int hurt;

	for (i = 0; i < 9; ++i) {
		hurt = ally[i]->full_blood - ally[i]->blood;
		ally[i]->level = get_level_from_experience(ally[i]->experience);
		ally[i]->agility = 26 + ally[i]->level*4;
		ally[i]->strength = 21 + ally[i]->level*4;
		ally[i]->intelligence = 16 + ally[i]->level*4;
		ally[i]->armor = 0.5*(3 + ally[i]->level);
		ally[i]->damage = 25 + 10*ally[i]->level + 0.2*(ally[i]->agility + ally[i]->strength + ally[i]->intelligence);
		ally[i]->full_blood = 200 + ally[i]->strength*19;
		ally[i]->full_mana = ally[i]->mana = 150 + ally[i]->intelligence*13;
		ally[i]->mana = ally[i]->full_mana;
		ally[i]->blood = ally[i]->full_blood - hurt;
	}

	for (i = 0; i < 10; ++i) {
		hurt = enemy[i]->full_blood - enemy[i]->blood;
		enemy[i]->level = get_level_from_experience(enemy[i]->experience);
		enemy[i]->agility = 26 + enemy[i]->level*4;
		enemy[i]->strength = 21 + enemy[i]->level*4;
		enemy[i]->intelligence = 16 + enemy[i]->level*4;
		enemy[i]->armor = 0.5*(3 + enemy[i]->level);
		enemy[i]->damage = 25 + 10*enemy[i]->level + 0.2*(enemy[i]->agility + enemy[i]->strength + enemy[i]->intelligence);
		enemy[i]->full_blood = 200 + enemy[i]->strength*19;
		enemy[i]->full_mana = enemy[i]->mana = 150 + enemy[i]->intelligence*13;
		enemy[i]->mana = enemy[i]->full_mana;
		enemy[i]->blood = enemy[i]->full_blood - hurt;
	}

	hurt = my->full_blood - my->blood;
	my->level = get_level_from_experience(my->experience);
	my->agility = 26 + my->level*4 + 10*my->own_item[0];
	my->strength = 21 + my->level*4 + 10*my->own_item[1];
	my->intelligence = 16 + my->level*4 + 10*my->own_item[2];
	my->armor = 0.5*(3 + my->level) + 10*my->own_item[3];
	my->damage = 25 + 10*my->level + 0.2*(my->agility + my->strength + my->intelligence) + 20*my->own_item[4] + 300*my->own_item[5];
	my->full_blood = 200 + my->strength*19;
	my->full_mana = my->mana = 150 + my->intelligence*13;
	my->mana = my->full_mana;
	my->blood = my->full_blood - hurt;
}

void generate_bullet()
{
	int i;

	if (my->target_bottle != -1) {
		int tar = my->target_bottle;
		float length;
		float vecx, vecy, ratiox, ratioy;
		length = sqrt((enemy[tar]->pos.x - my->pos.x)*(enemy[tar]->pos.x - my->pos.x) + 
			(enemy[tar]->pos.y - my->pos.y)*(enemy[tar]->pos.y - my->pos.y));
		if (length <= SHOT && my->bullet_counter == 0) {
			struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
			vecx = enemy[tar]->pos.x - my->pos.x;
			vecy = enemy[tar]->pos.y - my->pos.y;
			ratiox = vecx / length;
			ratioy = vecy / length;
			new_bullet->speed = BULLET_SPEED;
			new_bullet->pos.x = my->pos.x + my->direction.x*8;
			new_bullet->pos.y = my->pos.y + my->direction.y*8;
			new_bullet->pos.z = my->pos.z + 8*0.25;
			new_bullet->direction.x = ratiox * BOTTLE_SPEED;
			new_bullet->direction.y = ratioy * BOTTLE_SPEED;
			new_bullet->damage = my->damage;
			new_bullet->counter = 0;
			new_bullet->party = SENTINEL;
			new_bullet->owner_tower = -1;
			new_bullet->owner_bottle = 9;
			new_bullet->target_tower = -1;
			new_bullet->target_bottle = tar;
			new_bullet->prev = bullet_head;
			new_bullet->next = bullet_head->next;
			bullet_head->next = new_bullet;
			bullet_head->next->prev = new_bullet; 
			my->bullet_counter = (int) ((float) 60*20 / (float) my->agility);
		}
	} else if (my->target_tower != -1) {
		int tar = my->target_tower;
		float length;
		float vecx, vecy, ratiox, ratioy;
		length = sqrt((e_tower[tar].pos.x - my->pos.x)*(e_tower[tar].pos.x - my->pos.x) + 
			(e_tower[tar].pos.y - my->pos.y)*(e_tower[tar].pos.y - my->pos.y));
		if (length <= SHOT && my->bullet_counter == 0) {
			struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
			vecx = e_tower[tar].pos.x - my->pos.x;
			vecy = e_tower[tar].pos.y - my->pos.y;
			ratiox = vecx / length;
			ratioy = vecy / length;
			new_bullet->speed = BULLET_SPEED;
			new_bullet->pos.x = my->pos.x + my->direction.x*8;
			new_bullet->pos.y = my->pos.y + my->direction.y*8;
			new_bullet->pos.z = my->pos.z + 8*0.25;
			new_bullet->direction.x = ratiox * BOTTLE_SPEED;
			new_bullet->direction.y = ratioy * BOTTLE_SPEED;
			new_bullet->damage = my->damage;
			new_bullet->counter = 0;
			new_bullet->party = SENTINEL;
			new_bullet->owner_tower = -1;
			new_bullet->owner_bottle = 9;
			new_bullet->target_tower = tar;
			new_bullet->target_bottle = -1;
			new_bullet->prev = bullet_head;
			new_bullet->next = bullet_head->next;
			bullet_head->next = new_bullet;
			bullet_head->next->prev = new_bullet;
			my->bullet_counter = (int) ((float) 60*20 / (float) my->agility);
		}
	}

	for (i = 0; i < 9; ++i) {
		float length;
		if (ally[i]->target_bottle != -1) {
			int tar = ally[i]->target_bottle;
			length = sqrt((enemy[tar]->pos.x - ally[i]->pos.x)*(enemy[tar]->pos.x - ally[i]->pos.x) + 
				(enemy[tar]->pos.y - ally[i]->pos.y)*(enemy[tar]->pos.y - ally[i]->pos.y));
			if (length <= SHOT && ally[i]->bullet_counter == 0) {
				struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
				new_bullet->speed = BULLET_SPEED;
				new_bullet->pos.x = ally[i]->pos.x + ally[i]->direction.x*8;
				new_bullet->pos.y = ally[i]->pos.y + ally[i]->direction.y*8;
				new_bullet->pos.z = ally[i]->pos.z + 8*0.25;
				new_bullet->direction.x = ally[i]->direction.x*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->direction.y = ally[i]->direction.y*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->damage = ally[i]->damage;
				new_bullet->counter = 0;
				new_bullet->party = SENTINEL;
				new_bullet->owner_tower = -1;
				new_bullet->owner_bottle = i;
				new_bullet->target_tower = -1;
				new_bullet->target_bottle = tar;
				new_bullet->prev = bullet_head;
				new_bullet->next = bullet_head->next;
				bullet_head->next = new_bullet;
				bullet_head->next->prev = new_bullet;
				ally[i]->bullet_counter = (int) ((float) 60*20 / (float) ally[i]->agility); 
			}
		} else if (ally[i]->target_tower != -1) {
			int tar = ally[i]->target_tower;
			length = sqrt((e_tower[tar].pos.x - ally[i]->pos.x)*(e_tower[tar].pos.x - ally[i]->pos.x) + 
				(e_tower[tar].pos.y - ally[i]->pos.y)*(e_tower[tar].pos.y - ally[i]->pos.y));
			if (length <= SHOT && ally[i]->bullet_counter == 0) {
				struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
				new_bullet->speed = BULLET_SPEED;
				new_bullet->pos.x = ally[i]->pos.x + ally[i]->direction.x*8;
				new_bullet->pos.y = ally[i]->pos.y + ally[i]->direction.y*8;
				new_bullet->pos.z = ally[i]->pos.z + 8*0.25;
				new_bullet->direction.x = ally[i]->direction.x*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->direction.y = ally[i]->direction.y*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->damage = ally[i]->damage;
				new_bullet->counter = 0;
				new_bullet->party = SENTINEL;
				new_bullet->owner_tower = -1;
				new_bullet->owner_bottle = i;
				new_bullet->target_tower = tar;
				new_bullet->target_bottle = -1;
				new_bullet->prev = bullet_head;
				new_bullet->next = bullet_head->next;
				bullet_head->next = new_bullet;
				bullet_head->next->prev = new_bullet;
				ally[i]->bullet_counter = (int) ((float) 60*20 / (float) ally[i]->agility);
			}
		}
	}

	for (i = 0; i < 10; ++i) {
		float length;
		if (enemy[i]->target_bottle != -1) {
			int tar = enemy[i]->target_bottle;
			if (tar == 9) {
				length = sqrt((my->pos.x - enemy[i]->pos.x)*(my->pos.x - enemy[i]->pos.x) + 
				(my->pos.y - enemy[i]->pos.y)*(my->pos.y - enemy[i]->pos.y));
				if (length <= SHOT && enemy[i]->bullet_counter == 0) {
					struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
					new_bullet->speed = BULLET_SPEED;
					new_bullet->pos.x = enemy[i]->pos.x + enemy[i]->direction.x*8;
					new_bullet->pos.y = enemy[i]->pos.y + enemy[i]->direction.y*8;
					new_bullet->pos.z = enemy[i]->pos.z + 8*0.25;
					new_bullet->direction.x = enemy[i]->direction.x*BULLET_SPEED/BOTTLE_SPEED;
					new_bullet->direction.y = enemy[i]->direction.y*BULLET_SPEED/BOTTLE_SPEED;
					new_bullet->damage = enemy[i]->damage;
					new_bullet->counter = 0;
					new_bullet->party = SCOURGE;
					new_bullet->owner_tower = -1;
					new_bullet->owner_bottle = i;
					new_bullet->target_tower = -1;
					new_bullet->target_bottle = 9;
					new_bullet->prev = bullet_head;
					new_bullet->next = bullet_head->next;
					bullet_head->next = new_bullet;
					bullet_head->next->prev = new_bullet;
					enemy[i]->bullet_counter = (int) ((float) 60*20 / (float) enemy[i]->agility);
				}
			} else {
				length = sqrt((ally[tar]->pos.x - enemy[i]->pos.x)*(ally[tar]->pos.x - enemy[i]->pos.x) + 
				(ally[tar]->pos.y - enemy[i]->pos.y)*(ally[tar]->pos.y - enemy[i]->pos.y));
				if (length <= SHOT && enemy[i]->bullet_counter == 0) {
					struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
					new_bullet->speed = BULLET_SPEED;
					new_bullet->pos.x = enemy[i]->pos.x + enemy[i]->direction.x*8;
					new_bullet->pos.y = enemy[i]->pos.y + enemy[i]->direction.y*8;
					new_bullet->pos.z = enemy[i]->pos.z + 8*0.25;
					new_bullet->direction.x = enemy[i]->direction.x*BULLET_SPEED/BOTTLE_SPEED;
					new_bullet->direction.y = enemy[i]->direction.y*BULLET_SPEED/BOTTLE_SPEED;
					new_bullet->damage = enemy[i]->damage;
					new_bullet->counter = 0;
					new_bullet->party = SCOURGE;
					new_bullet->owner_tower = -1;
					new_bullet->owner_bottle = i;
					new_bullet->target_tower = -1;
					new_bullet->target_bottle = tar;
					new_bullet->prev = bullet_head;
					new_bullet->next = bullet_head->next;
					bullet_head->next = new_bullet;
					bullet_head->next->prev = new_bullet;
					enemy[i]->bullet_counter = (int) ((float) 60*20 / (float) enemy[i]->agility);
				}
			}
		} else if (enemy[i]->target_tower != -1) {
			int tar = enemy[i]->target_tower;
			length = sqrt((a_tower[tar].pos.x - enemy[i]->pos.x)*(a_tower[tar].pos.x - enemy[i]->pos.x) + 
				(a_tower[tar].pos.y - enemy[i]->pos.y)*(a_tower[tar].pos.y - enemy[i]->pos.y));
			if (length <= SHOT && enemy[i]->bullet_counter == 0) {
				struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
				new_bullet->speed = BULLET_SPEED;
				new_bullet->pos.x = enemy[i]->pos.x + enemy[i]->direction.x*8;
				new_bullet->pos.y = enemy[i]->pos.y + enemy[i]->direction.y*8;
				new_bullet->pos.z = enemy[i]->pos.z + 8*0.25;
				new_bullet->direction.x = enemy[i]->direction.x*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->direction.y = enemy[i]->direction.y*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->damage = enemy[i]->damage;
				new_bullet->counter = 0;
				new_bullet->party = SCOURGE;
				new_bullet->owner_tower = -1;
				new_bullet->owner_bottle = i;
				new_bullet->target_tower = tar;
				new_bullet->target_bottle = -1;
				new_bullet->prev = bullet_head;
				new_bullet->next = bullet_head->next;
				bullet_head->next = new_bullet;
				bullet_head->next->prev = new_bullet;
				enemy[i]->bullet_counter = (int) ((float) 60*20 / (float) enemy[i]->agility);
			}
		}
	}

	for (i = 0; i < 10; ++i) {
		float length;
		if (a_tower[i].target != -1 && a_tower[i].state == ATTACK_TOWER) {
			int tar = a_tower[i].target;
			length = sqrt((enemy[tar]->pos.x - a_tower[i].pos.x)*(enemy[tar]->pos.x - a_tower[i].pos.x) + 
			(enemy[tar]->pos.y - a_tower[i].pos.y)*(enemy[tar]->pos.y - a_tower[i].pos.y));
			if (length <= SHOT_TOWER && a_tower[i].bullet_counter == 0) {
				struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
				new_bullet->speed = BULLET_SPEED;
				new_bullet->pos.x = a_tower[i].pos.x + a_tower[i].direction2.x*a_tower[i].size;
				new_bullet->pos.y = a_tower[i].pos.y + a_tower[i].direction2.y*a_tower[i].size;
				new_bullet->pos.z = a_tower[i].pos.z + a_tower[i].size*0.25;
				new_bullet->direction.x = a_tower[i].direction2.x*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->direction.y = a_tower[i].direction2.y*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->damage = a_tower[i].damage;
				new_bullet->counter = 0;
				new_bullet->party = SENTINEL;
				new_bullet->owner_tower = i;
				new_bullet->owner_bottle = -1;
				new_bullet->target_tower = -1;
				new_bullet->target_bottle = tar;
				new_bullet->prev = bullet_head;
				new_bullet->next = bullet_head->next;
				bullet_head->next = new_bullet;
				bullet_head->next->prev = new_bullet;
				a_tower[i].bullet_counter = (int) ((float) 60*20 / (float) 45);
			}
		} 
	}

	for (i = 0; i < 10; ++i) {
		float length;
		if (e_tower[i].target != -1 && e_tower[i].state == ATTACK_TOWER) {
			int tar = e_tower[i].target;
			length = sqrt((ally[tar]->pos.x - e_tower[i].pos.x)*(ally[tar]->pos.x - e_tower[i].pos.x) + 
			(ally[tar]->pos.y - e_tower[i].pos.y)*(ally[tar]->pos.y - e_tower[i].pos.y));
			if (length <= SHOT_TOWER && e_tower[i].bullet_counter == 0) {
				struct bullet *new_bullet = bb_malloc(sizeof(struct bullet));
				new_bullet->speed = BULLET_SPEED;
				new_bullet->pos.x = e_tower[i].pos.x + e_tower[i].direction2.x*e_tower[i].size;
				new_bullet->pos.y = e_tower[i].pos.y + e_tower[i].direction2.y*e_tower[i].size;
				new_bullet->pos.z = e_tower[i].pos.z + e_tower[i].size*0.25;
				new_bullet->direction.x = e_tower[i].direction2.x*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->direction.y = e_tower[i].direction2.y*BULLET_SPEED/BOTTLE_SPEED;
				new_bullet->damage = e_tower[i].damage;
				new_bullet->counter = 0;
				new_bullet->party = SCOURGE;
				new_bullet->owner_tower = i;
				new_bullet->owner_bottle = -1;
				new_bullet->target_tower = -1;
				new_bullet->target_bottle = tar;
				new_bullet->prev = bullet_head;
				new_bullet->next = bullet_head->next;
				bullet_head->next = new_bullet;
				bullet_head->next->prev = new_bullet;
				e_tower[i].bullet_counter = (int) ((float) 60*20 / (float) 45);
			}
		} 
	}

	if (my->bullet_counter > 0) {
		--my->bullet_counter;
	}

	for (i = 0; i < 9; ++i) {
		if(ally[i]->bullet_counter > 0) {
			--ally[i]->bullet_counter;
		}
	}

	for (i = 0; i < 10; ++i) {
		if (enemy[i]->bullet_counter > 0) {
			--enemy[i]->bullet_counter;
		}
	}

	for (i = 0; i < 10; ++i) {
		if (a_tower[i].bullet_counter > 0) {
			--a_tower[i].bullet_counter;
		}
	}

	for (i = 0; i < 10; ++i) {
		if (e_tower[i].bullet_counter > 0) {
			--e_tower[i].bullet_counter;
		}
	}
}

void adjust_tower_angle()
{
	int i;
	float atan_val;
	float angle;
	float length;
	float vecx, vecy;

	for (i = 0; i < 10; ++i) {
		if (a_tower[i].blood <= 0 || a_tower[i].state == REST) {
			continue;
		} else {
			int tar = a_tower[i].target;
			length = sqrt((enemy[tar]->pos.x - a_tower[i].pos.x)*(enemy[tar]->pos.x - a_tower[i].pos.x) + 
					(enemy[tar]->pos.y - a_tower[i].pos.y)*(enemy[tar]->pos.y - a_tower[i].pos.y));
			vecx = enemy[tar]->pos.x - a_tower[i].pos.x;
			vecy = enemy[tar]->pos.y - a_tower[i].pos.y;
			atan_val = atan((float) bb_abs(vecy) / (float) bb_abs(vecx));
			angle = (atan_val / PI) * 180;
			if (vecx > 0 && vecy >= 0) {
				angle += 0;
			} else if (vecx <=0 && vecy > 0) {
				angle = 180 - angle;
			} else if (vecx < 0 && vecy <= 0) {
				angle += 180;
			} else {
				angle = -angle;
			}
			a_tower[i].direction = angle;
			if (length > SHOT_TOWER) {
				a_tower[i].state = REST;
				a_tower[i].target = -1;
			}
		}
	}

	for (i = 0; i < 10; ++i) {
		if (e_tower[i].blood <= 0 || e_tower[i].state == REST) {
			continue;
		} else {
			int tar = e_tower[i].target;
			length = sqrt((ally[tar]->pos.x - e_tower[i].pos.x)*(ally[tar]->pos.x - e_tower[i].pos.x) + 
					(ally[tar]->pos.y - e_tower[i].pos.y)*(ally[tar]->pos.y - e_tower[i].pos.y));
			vecx = ally[tar]->pos.x - e_tower[i].pos.x;
			vecy = ally[tar]->pos.y - e_tower[i].pos.y;
			atan_val = atan((float) bb_abs(vecy) / (float) bb_abs(vecx));
			angle = (atan_val / PI) * 180;
			if (vecx > 0 && vecy >= 0) {
				angle += 0;
			} else if (vecx <=0 && vecy > 0) {
				angle = 180 - angle;
			} else if (vecx < 0 && vecy <= 0) {
				angle += 180;
			} else {
				angle = -angle;
			}
			e_tower[i].direction = angle;
			if (length > SHOT_TOWER) {
				e_tower[i].state = REST;
				e_tower[i].target = -1;
			}
		}
	}
}

void adjust_bottle_angle()
{
	int i;
	float atan_val;
	float angle;

	if (my->speed > 0.5 && bb_abs(my->direction.x) > 0.001 && bb_abs(my->direction.y) > 0.001) {
		atan_val = atan((float) bb_abs(my->direction.y) / (float) bb_abs(my->direction.x));
		angle = (atan_val / PI) * 180;
		if (my->direction.x > 0 && my->direction.y >= 0) {
			angle += 0;
		} else if (my->direction.x <=0 && my->direction.y > 0) {
			angle = 180 - angle;
		} else if (my->direction.x < 0 && my->direction.y <= 0) {
			angle += 180;
		} else {
			angle = -angle;
		}
		my->angle = angle;
	}

	for (i = 0; i < 9; ++i) {
		if (ally[i]->speed > 0.5 && bb_abs(ally[i]->direction.x) > 0.001 && bb_abs(ally[i]->direction.y) > 0.001) {
			atan_val = atan((float) bb_abs(ally[i]->direction.y) / (float) bb_abs(ally[i]->direction.x));
			angle = (atan_val / PI) * 180;
			if (ally[i]->direction.x > 0 && ally[i]->direction.y >= 0) {
				angle += 0;
			} else if (ally[i]->direction.x <=0 && ally[i]->direction.y > 0) {
				angle = 180 - angle;
			} else if (ally[i]->direction.x < 0 && ally[i]->direction.y <= 0) {
				angle += 180;
			} else {
				angle = -angle;
			}
			ally[i]->angle = angle;
		}
	}

	for (i = 0; i < 10; ++i) {
		if (enemy[i]->speed > 0.5 && bb_abs(enemy[i]->direction.x) > 0.001 && bb_abs(enemy[i]->direction.y) > 0.001) {
			atan_val = atan((float) bb_abs(enemy[i]->direction.y) / (float) bb_abs(enemy[i]->direction.x));
			angle = (atan_val / PI) * 180;
			if (enemy[i]->direction.x > 0 && enemy[i]->direction.y >= 0) {
				angle += 0;
			} else if (enemy[i]->direction.x <=0 && enemy[i]->direction.y > 0) {
				angle = 180 - angle;
			} else if (enemy[i]->direction.x < 0 && enemy[i]->direction.y <= 0) {
				angle += 180;
			} else {
				angle = -angle;
			}
			enemy[i]->angle = angle;
		}
	}
}

void adjust_bottle_target()
{
	int i;
	int seed;

	for (i = 0; i < 9; ++i) {
		seed = rand() % 2;
		if (ally[i]->target_bottle == -1 && ally[i]->target_tower == -1) {
			if (seed == 1) {
				ally[i]->target_bottle = rand_bottle(0);
			} else if (seed == 0) {
				ally[i]->target_tower = rand_tower(0);
			}
		} else if (ally[i]->target_tower != -1 && e_tower[ally[i]->target_tower].blood <= 0) {
			if (seed == 1) {
				ally[i]->target_bottle = rand_bottle(0);
			} else if (seed == 0) {
				ally[i]->target_tower = rand_tower(0);
			}
		}
	}

	for (i = 0; i < 10; ++i) {
		seed = rand() % 2;
		if (enemy[i]->target_bottle == -1 && enemy[i]->target_tower == -1) {
			if (seed == 1) {
				enemy[i]->target_bottle = rand_bottle(1);
			} else if (seed == 0) {
				enemy[i]->target_tower = rand_tower(1);
			}
		} else if (enemy[i]->target_tower != -1 && a_tower[enemy[i]->target_tower].blood <= 0) {
			if (seed == 1) {
				enemy[i]->target_bottle = rand_bottle(1);
			} else if (seed == 0) {
				enemy[i]->target_tower = rand_tower(1);
			}
		}
	}
}

int rand_bottle(int type)
{
	return rand() % 10;
}

int rand_tower(int type)
{
	int rand_num;

	if (type == 0) {
		while (1) {
			rand_num = rand() % 10;
			if (e_tower[rand_num].blood > 0) {
				return rand_num;
			}
		}
	} else if (type == 1) {
		while (1) {
			rand_num = rand() % 10;
			if (a_tower[rand_num].blood > 0) {
				return rand_num;
			}
		}
	}

	return -1;
}

void refresh_position()
{
	refresh_bottle_positions();
	refresh_bullet_positions();
}

void refresh_bottle_positions()
{
	int i;

	my->pos.x += my->direction.x;
	my->pos.y += my->direction.y;

	if (is_at_obstacle((int) (my->pos.x + 0.5), (int) (my->pos.y + 0.5))) {
		my->pos.x -= my->direction.x;
		my->pos.y -= my->direction.y;
		my->speed = 0;
		my->direction.x = my->direction.y = 0;
	} else if (my->state == FOCUSED) {
		float length;
		length = sqrt((my->pos.x - my_dstx)*(my->pos.x - my_dstx) + 
				(my->pos.y - my_dsty)*(my->pos.y - my_dsty));
		if (length <= BOTTLE_SPEED + 1) {
			my->pos.x = my_dstx + 0.001;
			my->pos.y = my_dsty + 0.001;
			my->speed = 0;
			my->direction.x = my->direction.y = 0;
		}
	}

	if (my->target_bottle != -1) {
		int tar = my->target_bottle;
		float length;
		float vecx, vecy, ratiox, ratioy;
		length = sqrt((enemy[tar]->pos.x - my->pos.x)*(enemy[tar]->pos.x - my->pos.x) + 
			(enemy[tar]->pos.y - my->pos.y)*(enemy[tar]->pos.y - my->pos.y));
		if (length <= SHOT - 6) {
			float atan_val;
			float angle;
			my->speed = 0;
			vecx = (enemy[tar]->pos.x - my->pos.x);
			vecy = (enemy[tar]->pos.y - my->pos.y);
			ratiox = vecx / length;
			ratioy = vecy / length;
			atan_val = atan((float) bb_abs(ratioy) / (float) bb_abs(ratiox));
			angle = (atan_val / PI) * 180;
			if (ratiox > 0 && ratioy >= 0) {
				angle += 0;
			} else if (ratiox <=0 && ratioy > 0) {
				angle = 180 - angle;
			} else if (ratiox < 0 && ratioy <= 0) {
				angle += 180;
			} else {
				angle = -angle;
			}
			my->angle = angle;
			my->direction.x = my->direction.y = 0;
		} else {
			vecx = (enemy[tar]->pos.x - my->pos.x);
			vecy = (enemy[tar]->pos.y - my->pos.y);
			ratiox = vecx / length;
			ratioy = vecy / length;
			my->speed = BOTTLE_SPEED;
			my->direction.x = BOTTLE_SPEED * ratiox;
			my->direction.y = BOTTLE_SPEED * ratioy;
		}
	} else if (my->target_tower != -1) {
		int tar = my->target_tower;
		float length;
		float vecx, vecy, ratiox, ratioy;
		length = sqrt((e_tower[tar].pos.x - my->pos.x)*(e_tower[tar].pos.x - my->pos.x) + 
			(e_tower[tar].pos.y - my->pos.y)*(e_tower[tar].pos.y - my->pos.y));
		if (length <= SHOT - 6) {
			my->speed = 0;
			my->direction.x = my->direction.y = 0;
		} else {
			vecx = (e_tower[tar].pos.x - my->pos.x);
			vecy = (e_tower[tar].pos.y - my->pos.y);
			ratiox = vecx / length;
			ratioy = vecy / length;
			my->speed = BOTTLE_SPEED;
			my->direction.x = BOTTLE_SPEED * ratiox;
			my->direction.y = BOTTLE_SPEED * ratioy;
		}
	}

	for (i = 0; i < 9; ++i) {
		float length;
		if (ally[i]->target_bottle != -1) {
			int tar = ally[i]->target_bottle;
			length = sqrt((enemy[tar]->pos.x - ally[i]->pos.x)*(enemy[tar]->pos.x - ally[i]->pos.x) + 
				(enemy[tar]->pos.y - ally[i]->pos.y)*(enemy[tar]->pos.y - ally[i]->pos.y));
			if (length <= SHOT - 6) {
				ally[i]->speed = 0;
				ally[i]->direction.x = ally[i]->direction.y = 0;
			}
		} else if (ally[i]->target_tower != -1) {
			int tar = ally[i]->target_tower;
			length = sqrt((e_tower[tar].pos.x - ally[i]->pos.x)*(e_tower[tar].pos.x - ally[i]->pos.x) + 
				(e_tower[tar].pos.y - ally[i]->pos.y)*(e_tower[tar].pos.y - ally[i]->pos.y));
			if (length <= SHOT - 6) {
				ally[i]->speed = 0;
				ally[i]->direction.x = ally[i]->direction.y = 0;
			}
		}
	}

	for (i = 0; i < 10; ++i) {
		float length;
		if (enemy[i]->target_bottle != -1) {
			int tar = enemy[i]->target_bottle;
			if (tar == 9) {
				length = sqrt((my->pos.x - enemy[i]->pos.x)*(my->pos.x - enemy[i]->pos.x) + 
				(my->pos.y - enemy[i]->pos.y)*(my->pos.y - enemy[i]->pos.y));
				if (length <= SHOT - 6) {
					enemy[i]->speed = 0;
					enemy[i]->direction.x = enemy[i]->direction.y = 0;
				}
			} else {
				length = sqrt((ally[tar]->pos.x - enemy[i]->pos.x)*(ally[tar]->pos.x - enemy[i]->pos.x) + 
				(ally[tar]->pos.y - enemy[i]->pos.y)*(ally[tar]->pos.y - enemy[i]->pos.y));
				if (length <= SHOT - 6) {
					enemy[i]->speed = 0;
					enemy[i]->direction.x = enemy[i]->direction.y = 0;
				}
			}
		} else if (enemy[i]->target_tower != -1) {
			int tar = enemy[i]->target_tower;
			length = sqrt((a_tower[tar].pos.x - enemy[i]->pos.x)*(a_tower[tar].pos.x - enemy[i]->pos.x) + 
				(a_tower[tar].pos.y - enemy[i]->pos.y)*(a_tower[tar].pos.y - enemy[i]->pos.y));
			if (length <= SHOT - 6) {
				enemy[i]->speed = 0;
				enemy[i]->direction.x = enemy[i]->direction.y = 0;
			}
		}
	}

	for (i = 0; i < 9; ++i) {
		int seed = rand() % 2;
		ally[i]->pos.x += ally[i]->direction.x;
		ally[i]->pos.y += ally[i]->direction.y;

		if (!ally[i]->dummy_counter) {
			float length;
			float vecx, vecy, ratiox, ratioy;
			if (ally[i]->target_bottle != -1) {
				int tar = ally[i]->target_bottle;
				ally[i]->speed = BOTTLE_SPEED;
				length = sqrt((enemy[tar]->pos.x - ally[i]->pos.x)*(enemy[tar]->pos.x - ally[i]->pos.x) + 
					(enemy[tar]->pos.y - ally[i]->pos.y)*(enemy[tar]->pos.y - ally[i]->pos.y));
				vecx = enemy[tar]->pos.x - ally[i]->pos.x;
				vecy = enemy[tar]->pos.y - ally[i]->pos.y;
				ratiox = vecx / length;
				ratioy = vecy / length;
				ally[i]->direction.x = ratiox * BOTTLE_SPEED;
				ally[i]->direction.y = ratioy * BOTTLE_SPEED;
			} else if (ally[i]->target_tower != -1) {
				int tar = ally[i]->target_tower;
				ally[i]->speed = BOTTLE_SPEED;
				length = sqrt((e_tower[tar].pos.x - ally[i]->pos.x)*(e_tower[tar].pos.x - ally[i]->pos.x) + 
					(e_tower[tar].pos.y - ally[i]->pos.y)*(e_tower[tar].pos.y - ally[i]->pos.y));
				vecx = e_tower[tar].pos.x - ally[i]->pos.x;
				vecy = e_tower[tar].pos.y - ally[i]->pos.y;
				ratiox = vecx / length;
				ratioy = vecy / length;
				ally[i]->direction.x = ratiox * BOTTLE_SPEED;
				ally[i]->direction.y = ratioy * BOTTLE_SPEED;
			}
		}

		if (is_at_obstacle((int) (ally[i]->pos.x + 0.5), (int) (ally[i]->pos.y + 0.5))) {
			float tempx, tempy;
			ally[i]->pos.x -= ally[i]->direction.x;
			ally[i]->pos.y -= ally[i]->direction.y;
			ally[i]->dummy_counter = 30;
			tempx = ally[i]->direction.x;
			tempy = ally[i]->direction.y;
			if (seed == 0) {
				ally[i]->direction.x = -tempy;
				ally[i]->direction.y = tempx;
			} else if (seed == 1) {
				ally[i]->direction.x = tempy;
				ally[i]->direction.y = -tempx;
			}
		}

		if (ally[i]->dummy_counter > 0) {
			--ally[i]->dummy_counter;
		}
	}

	for (i = 0; i < 10; ++i) {
		int seed = rand() % 2;
		enemy[i]->pos.x += enemy[i]->direction.x;
		enemy[i]->pos.y += enemy[i]->direction.y;

		if (!enemy[i]->dummy_counter) {
			float length;
			float vecx, vecy, ratiox, ratioy;
			if (enemy[i]->target_bottle != -1) {
				int tar = enemy[i]->target_bottle;
				if (tar == 9) {
					enemy[i]->speed = BOTTLE_SPEED;
					length = sqrt((my->pos.x - enemy[i]->pos.x)*(my->pos.x - enemy[i]->pos.x) + 
						(my->pos.y - enemy[i]->pos.y)*(my->pos.y - enemy[i]->pos.y));
					vecx = my->pos.x - enemy[i]->pos.x;
					vecy = my->pos.y - enemy[i]->pos.y;
					ratiox = vecx / length;
					ratioy = vecy / length;
					enemy[i]->direction.x = ratiox * BOTTLE_SPEED;
					enemy[i]->direction.y = ratioy * BOTTLE_SPEED;
				} else {
					enemy[i]->speed = BOTTLE_SPEED;
					length = sqrt((ally[tar]->pos.x - enemy[i]->pos.x)*(ally[tar]->pos.x - enemy[i]->pos.x) + 
						(ally[tar]->pos.y - enemy[i]->pos.y)*(ally[tar]->pos.y - enemy[i]->pos.y));
					vecx = ally[tar]->pos.x - enemy[i]->pos.x;
					vecy = ally[tar]->pos.y - enemy[i]->pos.y;
					ratiox = vecx / length;
					ratioy = vecy / length;
					enemy[i]->direction.x = ratiox * BOTTLE_SPEED;
					enemy[i]->direction.y = ratioy * BOTTLE_SPEED;
				}
			} else if (enemy[i]->target_tower != -1) {
				int tar = enemy[i]->target_tower;
				enemy[i]->speed = BOTTLE_SPEED;
				length = sqrt((e_tower[tar].pos.x - enemy[i]->pos.x)*(e_tower[tar].pos.x - enemy[i]->pos.x) + 
					(e_tower[tar].pos.y - enemy[i]->pos.y)*(e_tower[tar].pos.y - enemy[i]->pos.y));
				vecx = e_tower[tar].pos.x - enemy[i]->pos.x;
				vecy = e_tower[tar].pos.y - enemy[i]->pos.y;
				ratiox = vecx / length;
				ratioy = vecy / length;
				enemy[i]->direction.x = ratiox * BOTTLE_SPEED;
				enemy[i]->direction.y = ratioy * BOTTLE_SPEED;
			}
		}

		if (is_at_obstacle((int) (enemy[i]->pos.x + 0.5), (int) (enemy[i]->pos.y + 0.5))) {
			float tempx, tempy;
			enemy[i]->pos.x -= enemy[i]->direction.x;
			enemy[i]->pos.y -= enemy[i]->direction.y;
			enemy[i]->dummy_counter = 30;
			tempx = ally[i]->direction.x;
			tempy = enemy[i]->direction.y;
			if (seed == 0) {
				enemy[i]->direction.x = -tempy;
				enemy[i]->direction.y = tempx;
			} else if (seed == 1) {
				enemy[i]->direction.x = tempy;
				enemy[i]->direction.y = -tempx;
			}
		}

		if (enemy[i]->dummy_counter > 0) {
			--enemy[i]->dummy_counter;
		}
	}
}

int is_at_obstacle(int x, int y)
{
	int i;
	float length;

	if (map[(y + 750)*1500 + x + 750] == 1) {
		return 1;
	}

	for (i = 0; i < 10; ++i) {
		length = sqrt((e_tower[i].pos.x - x)*(e_tower[i].pos.x - x) + 
				(e_tower[i].pos.y - y)*(e_tower[i].pos.y - y));
		if (length <= 12) {
			return 1;
		}
	}

	for (i = 0; i < 10; ++i) {
		length = sqrt((a_tower[i].pos.x - x)*(a_tower[i].pos.x - x) + 
				(a_tower[i].pos.y - y)*(a_tower[i].pos.y - y));
		if (length <= 12) {
			return 1;
		}
	}

	return 0;
}

void refresh_bullet_positions()
{
	struct bullet *temp = bullet_head->next;
	float length;
	float vecx, vecy, ratiox, ratioy;
	int tar;

	while (temp != bullet_tail) {
		temp->pos.x += temp->direction.x;
		temp->pos.y += temp->direction.y;
		if (temp->owner_bottle == SENTINEL || temp->owner_tower == SENTINEL) {
			if (temp->target_tower != -1) {
				tar = temp->target_tower;
				length = sqrt((e_tower[tar].pos.x - temp->pos.x)*(e_tower[tar].pos.x - temp->pos.x) + 
					(e_tower[tar].pos.y - temp->pos.y)*(e_tower[tar].pos.y - temp->pos.y));
				vecx = e_tower[tar].pos.x - temp->pos.x;
				vecy = e_tower[tar].pos.y - temp->pos.y;
				ratiox = vecx / length;
				ratioy = vecy / length;
				temp->direction.x = BULLET_SPEED * ratiox;
				temp->direction.y = BULLET_SPEED * ratioy;
			} else if (temp->target_bottle != -1) {
				tar = temp->target_bottle;
				length = sqrt((enemy[tar]->pos.x - temp->pos.x)*(enemy[tar]->pos.x - temp->pos.x) + 
					(enemy[tar]->pos.y - temp->pos.y)*(enemy[tar]->pos.y - temp->pos.y));
				vecx = enemy[tar]->pos.x - temp->pos.x;
				vecy = enemy[tar]->pos.y - temp->pos.y;
				ratiox = vecx / length;
				ratioy = vecy / length;
				temp->direction.x = BULLET_SPEED * ratiox;
				temp->direction.y = BULLET_SPEED * ratioy;
			}
		} else if (temp->owner_bottle == SCOURGE || temp->owner_tower == SCOURGE) {
			if (temp->target_tower != -1) {
				tar = temp->target_tower;
				length = sqrt((a_tower[tar].pos.x - temp->pos.x)*(a_tower[tar].pos.x - temp->pos.x) + 
					(a_tower[tar].pos.y - temp->pos.y)*(a_tower[tar].pos.y - temp->pos.y));
				vecx = a_tower[tar].pos.x - temp->pos.x;
				vecy = a_tower[tar].pos.y - temp->pos.y;
				ratiox = vecx / length;
				ratioy = vecy / length;
				temp->direction.x = BULLET_SPEED * ratiox;
				temp->direction.y = BULLET_SPEED * ratioy;
			} else if (temp->target_bottle != -1) {
				if (temp->target_bottle == 9) {
					length = sqrt((my->pos.x - temp->pos.x)*(my->pos.x - temp->pos.x) + 
						(my->pos.y - temp->pos.y)*(my->pos.y - temp->pos.y));
					vecx = my->pos.x - temp->pos.x;
					vecy = my->pos.y - temp->pos.y;
					ratiox = vecx / length;
					ratioy = vecy / length;
					temp->direction.x = BULLET_SPEED * ratiox;
					temp->direction.y = BULLET_SPEED * ratioy;
				} else {
					tar = temp->target_bottle;
					length = sqrt((ally[tar]->pos.x - temp->pos.x)*(ally[tar]->pos.x - temp->pos.x) + 
						(ally[tar]->pos.y - temp->pos.y)*(ally[tar]->pos.y - temp->pos.y));
					vecx = ally[tar]->pos.x - temp->pos.x;
					vecy = ally[tar]->pos.y - temp->pos.y;
					ratiox = vecx / length;
					ratioy = vecy / length;
					temp->direction.x = BULLET_SPEED * ratiox;
					temp->direction.y = BULLET_SPEED * ratioy;
				}	
			}
		}
		temp = temp->next;
	}
}

void hit_system()
{
	struct bullet *temp = bullet_head->next;
	struct bullet *cache;
	int true_damage;

	while (temp != bullet_tail) {
		++temp->counter;

		if (temp->counter >= 30) {
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			cache = temp;
			temp = temp->next;
			free(cache);
			continue;
		}

		if (temp->party == SENTINEL) {
			if (temp->owner_bottle != -1) {
				if (temp->owner_bottle == 9) {
					if (temp->target_bottle != -1) {
						int i = temp->target_bottle;
						if (bullet_in_range(temp)) {
							true_damage = get_true_damage(temp->damage, enemy[i]->armor);
							if (enemy[i]->blood - true_damage <= 0) {
								enemy[i]->blood = enemy[i]->full_blood;
								enemy[i]->target_bottle = -1;
								enemy[i]->target_tower = -1;
								enemy[i]->pos.x = 700 - 20*i;
								enemy[i]->pos.y = 500 + 20*i;
								enemy[i]->direction.x = 0;
								enemy[i]->direction.y = 0;
								enemy[i]->speed = 0;

								my->experience += enemy[i]->level * 200;
								my->gold += 350;
								my->state = FOCUSED;
								my->direction.x = 0;
								my->direction.y = 0;
								my->speed = 0;
								my->target_tower = -1;
								my->target_bottle = -1;
							} else {
								enemy[i]->blood -= true_damage;
							}
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							cache = temp;
							temp = temp->next;
							free(cache);
							continue;
						}
					} else if (temp->target_tower != -1) {
						int i = temp->target_tower;
						if (bullet_in_range(temp)) {
							true_damage = get_true_damage(temp->damage, e_tower[i].armor);
							if (e_tower[i].blood - true_damage <= 0) {
								e_tower[i].blood = 0;

								my->experience += 500;
								my->gold += 1000;
								my->state = FOCUSED;
								my->direction.x = 0;
								my->direction.y = 0;
								my->speed = 0;
								my->target_tower = -1;
								my->target_bottle = -1;
							} else {
								e_tower[i].blood -= true_damage;
							}
							if (e_tower[i].state == REST) {
								e_tower[i].state = ATTACK_TOWER;
								e_tower[i].target = 9;
							}
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							cache = temp;
							temp = temp->next;
							free(cache);
							continue;
						}
					}
				} else {
					int j = temp->owner_bottle;
					if (temp->target_bottle != -1) {
						int i = temp->target_bottle;
						if(bullet_in_range(temp)) {
							true_damage = get_true_damage(temp->damage, enemy[i]->armor);
							if (enemy[i]->blood - true_damage <= 0) {
								enemy[i]->blood = enemy[i]->full_blood;
								enemy[i]->target_bottle = -1;
								enemy[i]->target_tower = -1;
								enemy[i]->pos.x = 700 - 20*i;
								enemy[i]->pos.y = 500 + 20*i;
								enemy[i]->direction.x = 0;
								enemy[i]->direction.y = 0;
								enemy[i]->speed = 0;

								ally[j]->experience += enemy[i]->level * 200;
								ally[j]->gold += 350;
								ally[j]->state = FOCUSED;
								ally[j]->direction.x = 0;
								ally[j]->direction.y = 0;
								ally[j]->speed = 0;
								ally[j]->target_tower = -1;
								ally[j]->target_bottle = -1;
							} else {
								enemy[i]->blood -= true_damage;
							}
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							cache = temp;
							temp = temp->next;
							free(cache);
							continue;
						}
					} else if (temp->target_tower != -1) {
						int i = temp->target_tower;
						if (bullet_in_range(temp)) {
							true_damage = get_true_damage(temp->damage, e_tower[i].armor);
							if (e_tower[i].blood - true_damage <= 0) {
								e_tower[i].blood = 0;

								ally[j]->experience += 500;
								ally[j]->gold += 1000;
								ally[j]->state = FOCUSED;
								ally[j]->direction.x = 0;
								ally[j]->direction.y = 0;
								ally[j]->speed = 0;
								ally[j]->target_tower = -1;
								ally[j]->target_bottle = -1;
							} else {
								e_tower[i].blood -= true_damage;
							}
							if (e_tower[i].state == REST) {
								e_tower[i].state = ATTACK_TOWER;
								e_tower[i].target = j;
							}
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							cache = temp;
							temp = temp->next;
							free(cache);
							continue;
						}
					}
				}
			} else if (temp->owner_tower != -1) {
				int i = temp->target_bottle;
				int j = temp->owner_tower;
				if (bullet_in_range(temp)) {
					true_damage = get_true_damage(temp->damage, enemy[i]->armor);
					if (enemy[i]->blood - true_damage <= 0) {
						enemy[i]->blood = enemy[i]->full_blood;
						enemy[i]->target_bottle = -1;
						enemy[i]->target_tower = -1;
						enemy[i]->pos.x = 700 - 20*i;
						enemy[i]->pos.y = 500 + 20*i;
						enemy[i]->direction.x = 0;
						enemy[i]->direction.y = 0;
						enemy[i]->speed = 0;

						a_tower[j].state = REST;
						a_tower[j].target = -1;
					} else {
						enemy[i]->blood -= true_damage;
					}
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
					cache = temp;
					temp = temp->next;
					free(cache);
					continue;
				}
			}
		} else if (temp->party == SCOURGE) {
			if (temp->owner_bottle != -1) {
				if (temp->target_bottle != -1) {
					if (temp->target_bottle == 9) {
						if (bullet_in_range(temp)) {
							int j = temp->owner_bottle;
							true_damage = get_true_damage(temp->damage, my->armor);
							if (my->blood - true_damage <= 0) {
								my->blood = my->full_blood;
								my->target_bottle = -1;
								my->target_tower = -1;
								my->pos.x = -700;
								my->pos.y = -500;
								my->direction.x = 0;
								my->direction.y = 0;
								my->speed = 0;

								enemy[j]->experience += my->level * 200;
								enemy[j]->gold += 350;
								enemy[j]->state = FOCUSED;
								enemy[j]->direction.x = 0;
								enemy[j]->direction.y = 0;
								enemy[j]->speed = 0;
								enemy[j]->target_tower = -1;
								enemy[j]->target_bottle = -1;
							} else {
								my->blood -= true_damage;
							}
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							cache = temp;
							temp = temp->next;
							free(cache);
							continue;
						}
					} else {
						if(bullet_in_range(temp)) {
							int j = temp->owner_bottle;
							int i = temp->target_bottle;
							true_damage = get_true_damage(temp->damage, ally[i]->armor);
							if (ally[i]->blood - true_damage <= 0) {
								ally[i]->blood = ally[i]->full_blood;
								ally[i]->target_bottle = -1;
								ally[i]->target_tower = -1;
								ally[i]->pos.x = -700 + 20*(i+1);
								ally[i]->pos.y = -500 - 20*(i+1);
								ally[i]->direction.x = 0;
								ally[i]->direction.y = 0;
								ally[i]->speed = 0;

								enemy[j]->experience += ally[i]->level * 200;
								enemy[j]->gold += 350;
								enemy[j]->state = FOCUSED;
								enemy[j]->direction.x = 0;
								enemy[j]->direction.y = 0;
								enemy[j]->speed = 0;
								enemy[j]->target_tower = -1;
								enemy[j]->target_bottle = -1;
							} else {
								ally[i]->blood -= true_damage;
							}
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							cache = temp;
							temp = temp->next;
							free(cache);
							continue;
						}
					}
				} else if (temp->target_tower != -1) {
					if (bullet_in_range(temp)) {
						int j = temp->owner_bottle;
						int i = temp->target_tower;
						true_damage = get_true_damage(temp->damage, a_tower[i].armor);
						if (a_tower[i].blood - true_damage <= 0) {
							a_tower[i].blood = 0;

							enemy[j]->experience += 500;
							enemy[j]->gold += 1000;
							enemy[j]->state = FOCUSED;
							enemy[j]->direction.x = 0;
							enemy[j]->direction.y = 0;
							enemy[j]->speed = 0;
							enemy[j]->target_tower = -1;
							enemy[j]->target_bottle = -1;
						} else {
							a_tower[i].blood -= true_damage;
						}
						if (a_tower[i].state == REST) {
							a_tower[i].state = ATTACK_TOWER;
							a_tower[i].target = j;
						}
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
						cache = temp;
						temp = temp->next;
						free(cache);
						continue;
					}
				}
			} else if (temp->owner_tower != -1) {
				if (temp->target_bottle == 9) {
					if (bullet_in_range(temp)) {
						int j = temp->owner_tower;
						true_damage = get_true_damage(temp->damage, my->armor);
						if (my->blood - true_damage <= 0) {
							my->blood = my->full_blood;
							my->target_bottle = -1;
							my->target_tower = -1;
							my->pos.x = -700;
							my->pos.y = -500;
							my->gold = (my->gold - 300 >= 0) ? my->gold - 300 : 0;
							my->direction.x = 0;
							my->direction.y = 0;
							my->speed = 0;

							e_tower[j].state = REST;
							e_tower[j].target = -1;
						} else {
							my->blood -= true_damage;
						}
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
						cache = temp;
						temp = temp->next;
						free(cache);
						continue;
					}
				} else if (temp->target_bottle > 0) {
					if (bullet_in_range(temp)) {
						int j = temp->owner_tower;
						int i = temp->target_bottle;
						true_damage = get_true_damage(temp->damage, ally[i]->armor);
						if (ally[i]->blood - true_damage <= 0) {
							ally[i]->blood = ally[i]->full_blood;
							ally[i]->target_bottle = -1;
							ally[i]->target_tower = -1;
							ally[i]->pos.x = -700 + 20*(i+1);
							ally[i]->pos.y = -500 - 20*(i+1);
							ally[i]->gold = (my->gold - 300 >= 0) ? ally[i]->gold - 300 : 0;
							ally[i]->direction.x = 0;
							ally[i]->direction.y = 0;
							ally[i]->speed = 0;

							e_tower[j].state = REST;
							e_tower[j].target = -1;
						} else {
							ally[i]->blood -= true_damage;
						}
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
						cache = temp;
						temp = temp->next;
						free(cache);
						continue;
					}
				}
			}
		}
		temp = temp->next;
	}
}

void refresh_border()
{
	if (border[0] == 1) {
		camera_direction.y += GAIN;
		if (camera_direction.y > 750) {
			camera_direction.y -= GAIN;
		} else {
			camera_pos.y += GAIN;
		}
	}

	if (border[1] == 1) {
		camera_direction.y -= GAIN;
		if (camera_direction.y < -750) {
			camera_direction.y += GAIN;
		} else {
			camera_pos.y -= GAIN;
		}
	}

	if (border[2] == 1) {
		camera_direction.x -= GAIN;
		if (camera_direction.x < -750) {
			camera_direction.x += GAIN;
		} else {
			camera_pos.x -= GAIN;
		}
	}

	if (border[3] == 1) {
		camera_direction.x += GAIN;
		if (camera_direction.x > 750) {
			camera_direction.x -= GAIN;
		} else {
			camera_pos.x += GAIN;
		}
	}
}

void init_left_light()
{
	glutSetWindow(LEFT_WINDOW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lig_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lig_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lig_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void bb_set_materil(int type)
{
	switch(type) {
	case MY_BOTTLE:
		mat_diffuse[0] = 0.2;
		mat_diffuse[1] = 0.2;
		mat_diffuse[2] = 0.9;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.2;
		mat_specular[1] = 0.2;
		mat_specular[2] = 0.9;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case ALLY_BOTTLE:
		mat_diffuse[0] = 0.2;
		mat_diffuse[1] = 0.9;
		mat_diffuse[2] = 0.2;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.2;
		mat_specular[1] = 0.9;
		mat_specular[2] = 0.2;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case ENEMY_BOTTLE:
		mat_diffuse[0] = 0.9;
		mat_diffuse[1] = 0.2;
		mat_diffuse[2] = 0.2;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.9;
		mat_specular[1] = 0.2;
		mat_specular[2] = 0.2;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case ALLY_TOWER:
		mat_diffuse[0] = 0.2;
		mat_diffuse[1] = 0.7;
		mat_diffuse[2] = 0.7;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.2;
		mat_specular[1] = 0.7;
		mat_specular[2] = 0.7;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case ALLY_BASE:
		mat_diffuse[0] = 0.9;
		mat_diffuse[1] = 0.9;
		mat_diffuse[2] = 0.1;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.9;
		mat_specular[1] = 0.9;
		mat_specular[2] = 0.1;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case ENEMY_TOWER:
		mat_diffuse[0] = 0.7;
		mat_diffuse[1] = 0.7;
		mat_diffuse[2] = 0.2;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.7;
		mat_specular[1] = 0.7;
		mat_specular[2] = 0.2;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case ENEMY_BASE:
		mat_diffuse[0] = 0.1;
		mat_diffuse[1] = 0.9;
		mat_diffuse[2] = 0.9;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.1;
		mat_specular[1] = 0.9;
		mat_specular[2] = 0.9;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case BULLET:
		mat_diffuse[0] = 0.3;
		mat_diffuse[1] = 0.8;
		mat_diffuse[2] = 0.4;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.3;
		mat_specular[1] = 0.8;
		mat_specular[2] = 0.4;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	case OBSTACLE:
		mat_diffuse[0] = 0.9;
		mat_diffuse[1] = 0.5;
		mat_diffuse[2] = 0.8;
		mat_diffuse[3] = 1.0;
		mat_specular[0] = 0.6;
		mat_specular[1] = 0.7;
		mat_specular[2] = 0.8;
		mat_specular[3] = 1.0;
		mat_shininess[0] = 30.0;
		break;
	}
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}