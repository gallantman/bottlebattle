#define CHOOSE 0
#define FIGHT 1

#define GAIN 2

#define BULLET_SPEED 6
#define BOTTLE_SPEED 2

#define MY_BOTTLE 1
#define ALLY_BOTTLE 2
#define ENEMY_BOTTLE 3
#define ALLY_TOWER 4
#define ALLY_BASE 5
#define ENEMY_TOWER 6
#define ENEMY_BASE 7
#define BULLET 8
#define OBSTACLE 9

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int MAIN_WINDOW;
int LEFT_WINDOW;
int RIGHT_WINDOW;
double SCALEX, SCALEY;

int border[4]; /* up, down, left, right */

int my_dstx, my_dsty;

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
GLubyte grass_image[64][64][4];

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
	int target_tower;
	int target_bottle;
	int dummy_counter;
	int kill;
	int death;
	int own_item[8];
	float angle;
	int bullet_counter;
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

#define REST 1
#define ATTACK_TOWER 2

struct tower {
	struct pos pos;
	int damage;
	int armor;
	int full_blood;
	int blood;
	int party;
	float size;
	int direction;
	struct direction direction2;
	int state;
	struct color color;
	int target;
	int bullet_counter;
};

struct bullet {
	int speed;
	struct pos pos;
	struct direction direction;
	int damage;
	int owner_tower;
	int owner_bottle;
	int target_tower;
	int target_bottle;
	int party;
	int counter;
	struct bullet *prev;
	struct bullet *next;
};

struct bullet *bullet_head;
struct bullet *bullet_tail;

void *bb_malloc(int size);
void adjust_tower_angle(void);
void refresh_bottle_status(void);
int bullet_in_range(struct bullet *temp);
int get_true_damage(int damage, int armor);
void generate_bullet(void);
int click_ground(int mapx, int mapy);
int click_enemy_bottle(int mapx, int mapy);
int click_enemy_tower(int mapx, int mapy);
void init_bullet_head(void);
void render_tower(void);
void bb_set_materil(int type);
void init_tower(void);
void init_texture(void);
void load_num(void);
int enough_ball(int i, int j);
void refresh_border(void);
int in_border(void);
void light_init(void);
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
void render_map(void);
void render_grass(void);
void render_bottles(void);
void set_view(void);
void init_camera(void);
void init_soldier_heads(void);
void render_my(void);
void render_fighters(void);
void render_bullets(void);
void get_intersect_point(int x, int y, int *resx, int *resy);
int click_my(int x, int y);
void refresh_position(void);
void refresh_bottle_positions(void);
void refresh_bullet_positions(void);
void hit_system(void);
void refresh_my_status(void);
int is_at_obstacle(int x, int y);
void adjust_bottle_target();
int rand_bottle(int type);
int rand_tower(int type);
int get_level_from_experience(int exp);
void init_level(void);
void test_exit(void);
void adjust_bottle_angle(void);
float bb_abs(float val);

#define NORMAL 0
#define FOCUSED 1
#define PURCHASE 2;
#define ATTACK 3
#define MAGIC 4

#define SENTINEL 0
#define SCOURGE 1

#define SHOT 50
#define SHOT_TOWER 90

struct hero *my;
struct hero *ally[9];
struct hero *enemy[10];

struct tower e_tower[10];
struct tower a_tower[10];

struct soldier *a_head;
struct soldier *a_tail;
struct soldier *e_head;
struct soldier *e_tail;

struct pos camera_pos;
struct direction camera_direction;

#define PI 3.1415927

int level_experience[25];