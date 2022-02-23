#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "led-matrix.h"
#include "led_control.h"

#ifndef true
  #define true 1
#endif
#ifndef false
  #define false 0
#endif


uint8_t led_matrix[8][32];

int main(){
  
  println("START[RISC-V]");
  
  // Init LED Matrix
  init_led_matrix(&led_matrix);


  // Round n Round
  while(true){
    // To Ceil
    for(uint8_t ty = 0; ty < 32; ty++){
      set_led_at(&led_matrix, 0, ty, 1);
      if(ty > 0){
        set_led_at(&led_matrix, 0, ty-1, 0);
      }
      draw_screen_delayed(&led_matrix, 0x55FFFF);
      clean_screen(&led_matrix);
    }
    // To Right
    for(uint8_t tx = 1; tx < 8; tx++){
      set_led_at(&led_matrix, tx, 31, 1);
      if(tx > 0){
        set_led_at(&led_matrix, tx-1, 31, 0);
      }
      draw_screen_delayed(&led_matrix, 0x55FFFF);
      clean_screen(&led_matrix);
    }
    // To Bottom
    for(uint8_t ty = 1; ty < 32; ty++){
      set_led_at(&led_matrix, 7, 31-ty, 1);
      if(ty > 0){
        set_led_at(&led_matrix, 7, 32-ty, 0);
      }
      draw_screen_delayed(&led_matrix, 0x55FFFF);
      clean_screen(&led_matrix);
    }
    // To Left
    for(uint8_t tx = 1; tx < 8; tx++){
      set_led_at(&led_matrix, 7-tx, 0, 1);
      if(tx > 0){
        set_led_at(&led_matrix, 8-tx, 0, 0);
      }
      draw_screen_delayed(&led_matrix, 0x55FFFF);
      clean_screen(&led_matrix);
    }
  }


  return 0;
}

