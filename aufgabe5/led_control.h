void init_led_matrix(uint8_t led_matrix[8][32]);

void fill_screen(uint8_t led_matrix[8][32]);
void clean_screen(uint8_t led_matrix[8][32]);
void clean_matrice(uint8_t led_matrix[8][32], uint8_t matrice);
void set_led_at(uint8_t led_matrix[8][32], uint8_t x, uint8_t y, uint8_t value);
void draw_matrice(uint8_t led_matrix[8][32]);
void draw_screen(uint8_t led_matrix[8][32]);

void draw_screen_delayed(uint8_t led_matrix[8][32], uint32_t delay);

void draw_left_to_right(uint8_t led_matrix[8][32], uint32_t delay);
void draw_right_to_left(uint8_t led_matrix[8][32], uint32_t delay);

void invert_matrix(uint8_t led_matrix[8][32]);