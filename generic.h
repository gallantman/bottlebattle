#define CHOOSE 0
#define FIGHT 1

#define GAIN 2

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int MAIN_WINDOW;
int LEFT_WINDOW;
int RIGHT_WINDOW;
double SCALEX, SCALEY;

int border[4]; /* up, down, left, right */

char *grass;
char *wood;
char *map;
char *map2;
char *blank;
char *small_map;
char **items;
char **spells;
char *num[10];
int spell_chosen_map[80];
GLuint grass_texture;

int state;
int soldier_count;
int light_inited;

GLfloat lig_ambient[4];
GLfloat lig_diffuse[4];
GLfloat lig_position[4];
GLfloat mat_diffuse[4];
GLfloat mat_specular[4];
GLfloat mat_shininess[1];

struct pos {
	float x;
	float y;
	float z;
};

struct color {
	int R;
	int G;
	int B;
};

struct direction {
	float x;
	float y;
	float z;
};

struct hero {
	int level;
	struct pos pos;
	struct direction direction;
	int speed;
	int damage;
	int armor;
	int strength;
	int agility;
	int intelligence;
	int blood;
	int full_blood;
	int mana;
	int full_mana;
	int gold;
	int is_live;
	int state;
	int party;
	struct color color;
	int spell[5];
	int experience;
	int item[8];
};

struct soldier {
	struct pos pos;
	struct direction direction;
	int speed;
	int damage;
	int armor;
	int full_blood;
	int blood;
	int party;
	struct color color;
	struct soldier *prev;
	struct soldier *next;
};

void load_num(void);
int enough_ball(int i, int j);
void refresh_border();
int in_border();
void light_init();
void right_render_choose(void);
void init_left_light(void);
void bb_display(int value);
void bb_init(void);
void bb_keyboard(unsigned char key, int x, int y);
void bb_special(int key, int x, int y);
void bb_motion_left(int x, int y);
void bb_motion_right(int x, int y);
void get_screen_size(void);
void load_textures(void);
void load_grass(void);
void load_wood(void);
void load_items(void);
void load_spells(void);
void load_map(void);
void load_blank(void);
int dtoi(double val);
int next(FILE *fp);
void bb_display_left(void);
void bb_display_right(void);
void init_creatures(void);
void bb_mouse_left(int button, int state, int x, int y);
void bb_mouse_right(int button, int state, int x, int y);
int full_spell(struct hero *hero);
void set_hero_spell(struct hero *hero, int spell);
void refresh(void);
void render_map();
void render_grass();
void render_bottles();
void set_view();
void init_camera();
void init_soldier_heads();

#define NORMAL 0
#define FOCUSED 1
#define PURCHASE 2;
#define ATTACK 3
#define MAGIC 4

#define SENTINEL 0
#define SCOURGE 1

struct hero *my;
struct hero *ally[4];
struct hero *enemy[5];

struct soldier *a_head;
struct soldier *a_tail;
struct soldier *e_head;
struct soldier *e_tail;

struct pos camera_pos;
struct direction camera_direction;