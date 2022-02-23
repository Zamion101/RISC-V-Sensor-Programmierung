#include <stdio.h>
#include "platform.h"


void delay(int seconds){
  int i, j, k;
  for(i = 0; i < seconds; i++){
    for(j = 0; j < 10000; j++){
      for(k = 0; k < 30; k++){   
        int volatile t = 120 * j * i + k;
        t = t + 5;
        }
    }
  }
}


int main(){
  /*
  GPIO_11 -> Red
  GPIO_12 -> Green
  GPIO_13 -> Blue
  */
  GPIO_REG(GPIO_INPUT_EN) &= ~(1 << 11);
  GPIO_REG(GPIO_OUTPUT_EN) |= (1 << 11);

  // LD2 -> Red
  GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << 11);
  
  delay(2);
  //LD2 -> Yellow
  GPIO_REG(GPIO_INPUT_EN) &= ~(1 << 12);
  GPIO_REG(GPIO_OUTPUT_EN) |= (1 << 12);
  GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << 12);
  
  delay(2);
  // LD2 -> Blink
    // Clear the LD2
  GPIO_REG(GPIO_OUTPUT_VAL) &= ~(1 << 11);
  GPIO_REG(GPIO_OUTPUT_VAL) &= ~(1 << 12);

  while(1){
    // AN
    GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << 11);
    delay(3);
    // AUF
    GPIO_REG(GPIO_OUTPUT_VAL) &= ~(1 << 11);
    delay(3);
  }

  return 0;
}
