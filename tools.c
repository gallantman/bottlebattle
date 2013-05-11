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