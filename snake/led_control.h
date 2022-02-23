void init_led_matrix(int8_t led_matrix[8][32]);

void fill_screen(int8_t led_matrix[8][32]);
void clean_screen(int8_t led_matrix[8][32]);
void clean_matrice(int8_t led_matrix[8][32], int8_t matrice);
void set_led_at(int8_t led_matrix[8][32], int8_t x, int8_t y, int8_t value);
void draw_matrice(int8_t led_matrix[8][32]);
void draw_screen(int8_t led_matrix[8][32]);

void draw_screen_delayed(int8_t led_matrix[8][32], uint32_t delay);

void draw_left_to_right(int8_t led_matrix[8][32], uint32_t delay);
void draw_right_to_left(int8_t led_matrix[8][32], uint32_t delay);


void invert_matrix(int8_t led_matrix[8][32]);