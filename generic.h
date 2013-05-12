#define CHOOSE 0
#define FIGHT 1

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int MAIN_WINDOW;
int LEFT_WINDOW;
int RIGHT_WINDOW;
double SCALEX, SCALEY;

char *grass;
char *wood;
char *map;
char *blank;
char *small_map;
char **items;
char **spells;
int spell_chosen_map[80];

int state;

struct pos {
	int x;
	int y;
	int z;
};

struct color {
	int R;
	int G;
	int B;
};

struct direction {
	int x;
	int y;
	int z;
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
};

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
void bb_idle_right(void);
void bb_idle_left(void);
void refresh(void);

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
