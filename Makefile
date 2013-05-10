all: bottlebattle.c init.c event.c display.c generic.h
	gcc bottlebattle.c init.c event.c display.c -o bottlebattle -lGL -lglut -lGLU -Wall
clean:
	rm -f *.o *~ bottlebattle
