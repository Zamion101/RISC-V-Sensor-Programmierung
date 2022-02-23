#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "led-matrix.h"


void draw_delay(uint32_t amount){
  uint64_t end = (amount >> 1) + get_timer_value();
  while( get_timer_value () < end );
}

void fill_screen(int8_t led_matrix[8][32]){
    for(int8_t x = 0; x < 8; x++){
        for(int8_t y = 0; y < 32; y++){
            led_matrix[x][y] = 1;
        }
    }
}

void clean_screen(int8_t led_matrix[8][32]){
    for(int8_t x = 0; x < 8; x++){
        for(int8_t y = 0; y < 32; y++){
            led_matrix[x][y] = 0;
        }
    }
}

void init_led_matrix(int8_t led_matrix[8][32]){
  println("INIT[LED MATRIX]");
  setup_matrix();
  clear();
  clean_screen(led_matrix);
}

void clean_matrice(int8_t led_matrix[8][32], int8_t matrice){
    int8_t y_start = matrice*8;
    for(int8_t x = 0; x < 8; x++){
        for(int8_t y = y_start; y < y_start+8; y++){
            led_matrix[x][y] = 0;
        }
    }
}

void draw_x(int8_t led_matrix[8][32], int8_t x){
    for(int8_t n = 0; n < 4; n++){
        int8_t rsl = 0;
        for(int8_t y = 0; y < 8; y++){
            int8_t val = led_matrix[x][(n*8)+y];
            rsl = rsl | (val << 7-y);
        }
        set_column((n*8)+x, rsl);
    }
}


void draw_screen(int8_t led_matrix[8][32]){
    for(int8_t col = 0; col < 32; col++){
        int8_t x = col % 8;
        int8_t n = col / 8; //matrice
        int8_t rsl = 0;
        for(int8_t y = 0; y < 8; y++){
            int8_t val = led_matrix[x][(n*8)+y];
            rsl = rsl | (val << 7-y);
        }
        set_column(col, rsl);
    }
}

void draw_screen_delayed(int8_t led_matrix[8][32], uint32_t delay){
    for(int8_t col = 0; col < 32; col++){
        int8_t x = col % 8;
        int8_t n = col / 8; //matrice
        int8_t rsl = 0;
        for(int8_t y = 0; y < 8; y++){
            int8_t val = led_matrix[col%8][(n*8)+y];
            rsl = rsl | (val << 7-y);
        }
        set_column(col, rsl);
    }
    delay_ms(delay);
    
}

void set_led_at(int8_t led_matrix[8][32], int8_t x, int8_t y, int8_t val){
    led_matrix[x][y] = val;
}


void draw_left_to_right(int8_t led_matrix[8][32], uint32_t delay){
    for(int8_t x = 0; x < 8; x++){
        draw_x(led_matrix, x);
        draw_delay(delay);
    }
}
void draw_right_to_left(int8_t led_matrix[8][32], uint32_t delay){
    for(int8_t x = 0; x < 8; x++){
        draw_x(led_matrix, 7-x);
        draw_delay(delay);
    }
}

void invert_matrix(int8_t led_matrix[8][32]){
    for(int8_t x = 0; x < 8; x++){
        for(int8_t y = 0; y < 32; y++){
            led_matrix[x][y] = !led_matrix[x][y];
        }
    }
}