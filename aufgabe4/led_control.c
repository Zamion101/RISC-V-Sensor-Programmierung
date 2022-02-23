#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "led-matrix.h"


void draw_delay(uint32_t amount){
  uint64_t end = (amount >> 1) + get_timer_value();
  while( get_timer_value () < end );
}

void fill_screen(uint8_t led_matrix[8][32]){
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 32; y++){
            led_matrix[x][y] = 1;
        }
    }
}

void clean_screen(uint8_t led_matrix[8][32]){
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 32; y++){
            led_matrix[x][y] = 0;
        }
    }
}

void init_led_matrix(uint8_t led_matrix[8][32]){
  println("INIT[LED MATRIX]");
  setup_matrix();
  clear();
  clean_screen(led_matrix);
}

void clean_matrice(uint8_t led_matrix[8][32], uint8_t matrice){
    uint8_t y_start = matrice*8;
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = y_start; y < y_start+8; y++){
            led_matrix[x][y] = 0;
        }
    }
}

void draw_x(uint8_t led_matrix[8][32], uint8_t x){
    for(uint8_t n = 0; n < 4; n++){
        uint8_t rsl = 0;
        for(uint8_t y = 0; y < 8; y++){
            uint8_t val = led_matrix[x][(n*8)+y];
            rsl = rsl | (val << 7-y);
        }
        set_column((n*8)+x, rsl);
    }
}

void draw_screen(uint8_t led_matrix[8][32]){
    for(uint8_t col = 0; col < 32; col++){
        uint8_t x = col % 8;
        uint8_t n = col / 8; //matrice
        uint8_t rsl = 0;
        for(uint8_t y = 0; y < 8; y++){
            uint8_t val = led_matrix[x][(n*8)+y];
            rsl = rsl | (val << 7-y);
        }
        set_column(col, rsl);
    }
}

void draw_screen_delayed(uint8_t led_matrix[8][32], uint32_t delay){
    for(uint8_t col = 0; col < 32; col++){
        uint8_t x = col % 8;
        uint8_t n = col / 8; //matrice
        uint8_t rsl = 0;
        for(uint8_t y = 0; y < 8; y++){
            uint8_t val = led_matrix[col%8][(n*8)+y];
            rsl = rsl | (val << 7-y);
        }
        set_column(col, rsl);
    }
    draw_delay(delay);
    
}

void set_led_at(uint8_t led_matrix[8][32], uint8_t x, uint8_t y, uint8_t val){
    led_matrix[x][y] = val;
}


void draw_left_to_right(uint8_t led_matrix[8][32], uint32_t delay){
    for(uint8_t x = 0; x < 8; x++){
        draw_x(led_matrix, x);
        draw_delay(delay);
    }
}
void draw_right_to_left(uint8_t led_matrix[8][32], uint32_t delay){
    for(uint8_t x = 0; x < 8; x++){
        draw_x(led_matrix, 7-x);
        draw_delay(delay);
    }
}

void invert_matrix(uint8_t led_matrix[8][32]){
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 32; y++){
            led_matrix[x][y] = !led_matrix[x][y];
        }
    }
}