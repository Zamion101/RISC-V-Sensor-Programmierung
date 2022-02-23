#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "spi.h"
#include "bmi160.h"
#include "print.c"

void delay_ms(uint32_t amount){
  uint64_t end = (amount >> 1) + get_timer_value();
  while( get_timer_value () < end );
}

int main(){
  println("START[Aufgabe-3]\n");
  println("INIT[sensor]");
  struct bmi160_dev sensor;
  println("INIT[SPI]");
  sensor.intf = BMI160_SPI_INTF;
  println("INIT[read & write]");
  sensor.read = &spi_read;
  sensor.write = &spi_write;
  println("INIT[delay_ms]");
  sensor.delay_ms = &delay_ms;
  println("INIT[SPI CLK]");
  spi_begin(PIN_10_OFFSET, 100000);

  int8_t result = bmi160_init(&sensor);
  if(result == BMI160_OK){
    println("BMI160 detected!");
  }else{
    printf("Error, could not detect BMI160, errorcode: %d", result);
  }

  return 0;
}

