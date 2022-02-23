#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "spi.h"
#include "bmi160.h"


#ifndef true
  #define true 1
#endif
#ifndef false
  #define false 0
#endif


struct bmi160_dev sensor;
// Accel data
struct bmi160_sensor_data accel;
struct bmi160_sensor_data gyro;
uint8_t temp0;
uint8_t temp1;


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

void read_accel_data(int8_t until_read_count){
  int8_t times_read = 0;
  while(times_read < 100){
    bmi160_get_sensor_data(BMI160_ACCEL_SEL, &accel, NULL, &sensor);
    delay_ms(2000000);
    // Accel Data
    printf("ax:%d\tay:%d\taz:%d\n", accel.x, accel.y, accel.z);
    fflush(stdout);
    times_read += 1;
  }
}

void read_gyro_data(int8_t until_read_count){
  int8_t times_read = 0;
  while(times_read < 100){
    bmi160_get_sensor_data(BMI160_GYRO_SEL, NULL, &gyro, &sensor);
    delay_ms(2000000);
    // Gyro Data
    printf("gx:%d\tgy:%d\tgz:%d\n", gyro.x, gyro.y, gyro.z);
    fflush(stdout);
    times_read += 1;
  }
}

void read_temperature_data(){
  bmi160_get_regs(0x20, &temp0, 2, &sensor);
  bmi160_get_regs(0x21, &temp1, 2, &sensor);
  printf("TEMPERATURE_0: %d\n", temp0);
  printf("TEMPERATURE_1: %d\n", temp1);
}



int main(){
  int8_t times_read = 0;
   // Init BMI160 Interface
  init_bmi160_interface();
  
  // Read Sensor Data
  read_accel_data(100);
  
  // Read Temperature Data
  read_temperature_data();
    return 0;
}