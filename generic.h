int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int MAIN_WINDOW;
int LEFT_WINDOW;
int RIGHT_WINDOW;
double SCALEX, SCALEY;

char *grass;
char *wood;
char **items;
char **spells;

void bb_display(void);
void bb_idle(void);
void bb_init(void);
void bb_keyboard(unsigned char key, int x, int y);
void bb_special(int key, int x, int y);
void bb_motion(int x, int y);
void get_screen_size(void);
void load_textures(void);
void load_grass(void);
void load_wood(void);
void load_items(void);
void load_spells(void);
int dtoi(double val);
int next(FILE *fp);
void bb_display_left(void);
void bb_display_right(void);
