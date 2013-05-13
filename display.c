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
	if (a_tower[9].blood <= 0 || e_tower[9].blood <= 0) {
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
		if (my->item[i + 4]) {
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
	refresh_border();
	refresh_position();
	hit_system();
	adjust_bottle_target();
	adjust_bottle_angle();
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
	} else {
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
	if (map[(y + 750)*1500 + x + 750] == 1) {
		return 1;
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
		if (temp->owner == SENTINEL) {
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
		} else if (temp->owner == SCOURGE) {
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
		temp = temp->next;
	}
}

void hit_system()
{

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