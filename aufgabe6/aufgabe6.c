#include "encoding.h"
#include "plic/plic_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "platform.h"
#include "interrupt.h"

#ifndef true
  #define true 1
#endif

#ifndef false
  #define false 0
#endif


void delay_ms(uint32_t amount)
{
  uint64_t end = (amount >> 1) + get_timer_value();
  while (get_timer_value() < end)
    ;
}

void button_0_handler(void)
{
  printf("Handled Button 0\n");
  GPIO_REG(GPIO_RISE_IP) = (1 << BUTTON_0_OFFSET);
}

void button_1_handler(void)
{
  printf("Handled Button 1\n");
  GPIO_REG(GPIO_RISE_IP) = (1 << BUTTON_1_OFFSET);
}

void button_2_handler(void)
{
  printf("Handled Button 2\n");
  GPIO_REG(GPIO_RISE_IP) = (1 << BUTTON_2_OFFSET);
}



int main(){
  clear_interrupts();
  register_interrupt(BUTTON_0_OFFSET, INT_DEVICE_BUTTON_0, &button_0_handler);
  register_interrupt(BUTTON_1_OFFSET, INT_DEVICE_BUTTON_1, &button_1_handler);
  register_interrupt(BUTTON_2_OFFSET, INT_DEVICE_BUTTON_2, &button_2_handler);
  finalize_interrupts();
}

