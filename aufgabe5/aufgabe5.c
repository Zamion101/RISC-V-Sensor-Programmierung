#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "spi.h"
#include "bmi160.h"
#include "led_control.h"


#ifndef true
  #define true 1
#endif
#ifndef false
  #define false 0
#endif


// LED Matrix
uint8_t led_matrix[8][32];

// Translated LED Matrix X and Y from BMI160
uint8_t tx, ty;


// BMI160 Sensor
struct bmi160_dev sensor;
// Accel data
struct bmi160_sensor_data accel;
// Gryo data
struct bmi160_sensor_data gyro;


void delay_ms(uint32_t amount){
  uint64_t end = (amount >> 1) + get_timer_value();
  while( get_timer_value () < end );
}


void init_bmi160_interface(){
  println("INIT[SPI]");
  sensor.intf = BMI160_SPI_INTF;
  println("INIT[read & write]");
  sensor.read = &spi_read;
  sensor.write = &spi_write;
  println("INIT[delay_ms]");
  sensor.delay_ms = &delay_ms;
  println("INIT[SPI CLK]");
  spi_begin(PIN_10_OFFSET, 1000);

  int8_t result = bmi160_init(&sensor);
  if(result == BMI160_OK){
    println("BMI160 detected!");
    printf("Chip ID 0x%X\n", sensor.chip_id);
    
    /* Select the Output data rate, range of accelerometer sensor */
    sensor.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    sensor.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
    sensor.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Select the power mode of accelerometer sensor */
    sensor.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    /* Select the Output data rate, range of Gyroscope sensor */
    sensor.gyro_cfg.odr = BMI160_GYRO_ODR_3200HZ;
    sensor.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    sensor.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    sensor.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

    /* Set the sensor configuration */
    result = bmi160_set_sens_conf(&sensor);

  }else{
    printf("Error, could not detect BMI160, errorcode: %d", result);
  }
}


void read_accel_and_gyro_data(){
  bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &accel, &gyro, &sensor);
  // Accel Data
  printf("ax:%d\tay:%d\taz:%d\n", accel.x, accel.y, accel.z);
  // Gyro Data
  printf("gx:%d\tgy:%d\tgz:%d\n", gyro.x, gyro.y, gyro.z);
  fflush(stdout);
}

int main(){
  int8_t times_read = 0;
   // Init BMI160 Interface
  init_bmi160_interface();

  int led_x = 5, led_y = 15;


  // Init LED Matrix
  init_led_matrix(&led_matrix);

  while(true){
    // Read Sensor Data
    read_accel_and_gyro_data();
    if(accel.x > 1000){
      led_x = (led_x - 1);
    }else if(accel.x < -1000){
      led_x = (led_x + 1);
    }
    if(led_x < 0){
      led_x = 0;
    }
    if(led_x > 8){
      led_x = 8;
    }

    if(accel.y > 1000){
      led_y = (led_y - 1);
    }else if(accel.y < -1000){
      led_y = (led_y + 1);
    }
    if(led_y < 0){
      led_y = 1;
    } 
    if(led_y > 32){
      led_y = 32;
    }

    printf("X: %d\tY: %d\n", led_x, led_y);
    set_led_at(&led_matrix, led_x-1, led_y-1, 1);
    draw_screen(&led_matrix);
    set_led_at(&led_matrix, led_x-1, led_y-1, 0);
    delay_ms(0x55FFFF);
  }
     
  return 0;
}