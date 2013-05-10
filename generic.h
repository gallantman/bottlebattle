int SCREEN_WIDTH;
int SCREEN_HEIGHT;

void bb_display(void);
void bb_idle(void);
void bb_init(void);
void bb_keyboard(unsigned char key, int x, int y);
void bb_special(int key, int x, int y);
void bb_motion(int x, int y);
void get_screen_size();