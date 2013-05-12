#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "generic.h"

void bb_display(int value)
{
	refresh();
	glutSetWindow(RIGHT_WINDOW);
	glutPostRedisplay();
	glutSetWindow(LEFT_WINDOW);
	glutPostRedisplay();
	glutTimerFunc(50, bb_display, 0);
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
		glutSwapBuffers();
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