#include "encoding.h"
#include "plic/plic_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "spi.h"
#include "platform.h"
#include "led-matrix.h"
#include "led_control.h"
#include "bmi160.h"
#include "interrupt.h"
#include "utils.h"
#include "snake.h"
#include "rom.h"

struct bmi160_dev sensor;
// Accel data
struct bmi160_sensor_data accel;

int8_t led_matrix[8][32];
int8_t screen_x = 8;
int8_t screen_y = 32;
Snake snake;
Position snake_head_position;
Position fruit;


int8_t is_game_started = false;
int8_t is_game_over = false;

int random_number(){
    int seed = rand();
    if(snake.snake_body->size == 0){
        seed += snake.snake_head.x * snake.snake_head.y;
    }else if(snake.snake_body->size == 1){
        PositionNode *node = snake.snake_body->head;
        seed += node->x * node->y;
    }else{
        PositionNode *node = snake.snake_body->head;
        while(node->next != NULL){
            seed += node->x * node->y;
            node = node->next;
        }
    }
    srand(seed*rand()%RAND_MAX);
    return rand();
}


void init_bmi160_interface(){
  sensor.intf = BMI160_SPI_INTF;
  sensor.read = &spi_read;
  sensor.write = &spi_write;
  sensor.delay_ms = &delay_ms;
  spi_begin(PIN_10_OFFSET, 1000);

  int8_t result = bmi160_init(&sensor);
  if(result == BMI160_OK){
    println("BMI160 detected!");
    printf("Chip ID 0x%X\n", sensor.chip_id);

    /* Select the Output data rate, range of accelerometer sensor */
    sensor.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    sensor.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    sensor.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Select the power mode of accelerometer sensor */
    sensor.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;


    /* Set the sensor configuration */
    result = bmi160_set_sens_conf(&sensor);

  }else{
    printf("Error, could not detect BMI160, errorcode: %d\n", result);
  }
}


void read_accel_data_and_set_direction_of_snake(){
    int8_t rslt;
    rslt = bmi160_get_sensor_data(BMI160_ACCEL_SEL, &accel, NULL, &sensor);
    //printf("Accel Result: %d\n", rslt);
    if(rslt == BMI160_OK){
        //printf("Accel X: %d\tY: %d\n", accel.x, accel.y);

        if(accel.x > 5000 && snake.direction != UP){
            printf("+X\n");
            snake.direction = DOWN;
        }
        else if(accel.y > 5000 && snake.direction != LEFT){
            snake.direction = RIGHT;
            printf("+Y\n");
        }
        else if(accel.x < -5000 && snake.direction != DOWN){
            printf("-X\n");
            snake.direction = UP;
        }
        else if(accel.y < -5000 && snake.direction != RIGHT){
            snake.direction = LEFT;
            printf("-Y\n");
        }
    }
}


void place_fruit(){
    fruit_pos:
        fruit.x = random_number()%8;
        fruit.y = random_number()%32;
        if(collides_with_snake_body(fruit)){
            printf("Fruits collide with snake body!\n");
            goto fruit_pos;
        }
}


void button_0_handler(void){
    println("BTN0");
    if(is_game_started == false){
        println("Starting Game...");
        snake.points = 0;
        snake_head_position.x = screen_x / 2;
        snake_head_position.y = screen_y / 2;

        PositionArray *pArray;
        pArray = (PositionArray *)malloc(sizeof(PositionArray));
        initPositionArray(pArray);
        snake.snake_body = pArray;
        snake.snake_head = snake_head_position;
        snake.direction = RIGHT;
        is_game_started = true;
        is_game_over = false;
        start_game();
    }else{
        snake.direction = RIGHT;
    }
    GPIO_REG(GPIO_RISE_IP) = (1 << BUTTON_0_OFFSET);
}

void end_game(){
    snake.snake_head.x = screen_x /2;
    snake.snake_head.y = screen_y /2;

    is_game_started = false;
    is_game_over = true;
    println("GAME OVER!");
    printf("Score: %d\n", snake.points);
    show_game_over();
}

int8_t move_snake_head(){
    // Get snake direction
    if(snake.direction == UP){
        snake.snake_head.x--;
        if(snake.snake_head.x < 0){
            return false;
        }
        return true;
    }else if(snake.direction == DOWN){
        snake.snake_head.x++;
        if(snake.snake_head.x > 7){
            return false;
        }
        return true;
    }else if(snake.direction == LEFT){
        snake.snake_head.y--;
        if(snake.snake_head.y < 0){
            return false;
        }
        return true;
    }else if(snake.direction == RIGHT){
        snake.snake_head.y++;
        if(snake.snake_head.y > 31){
            return false;
        }
        return true;
    }
}

int eat_fruit(){
    Position mouth = snake.snake_head;
    if((mouth.x == fruit.x) && (mouth.y == fruit.y)){
        snake.points += 1;
        printf("Snake Points: %d\n", snake.points);
        place_fruit();
        printf("Place Fruit\n");
        return true;
    }
    return false;
}

int collides_with_snake_body(Position pos){
    if(snake.snake_body->size == 0){
        return false;
    }else if(snake.snake_body->size == 1){
        PositionNode *node = snake.snake_body->head;
        if(node->x == pos.x && node->y == pos.y){
            return true;
        }
        return false;
    }else{
        PositionNode *node = snake.snake_body->head;
        if(node->x == pos.x && node->y == pos.y){
            return true;
        }
        while(node->next != NULL){
            node = node->next;
            if(node->x == pos.x && node->y == pos.y){
                return true;
            }
        }
        return false;
    }
}

void start_game(){
    place_fruit();
    while(is_game_started){
        clean_screen(&led_matrix);
        set_led_at(&led_matrix, fruit.x, fruit.y, 1);
        
        // Move Snake to direction
        read_accel_data_and_set_direction_of_snake();
        
        Position prevHead = snake.snake_head;
        int8_t rslt =  move_snake_head();
        int is_fruit_ate = eat_fruit();
        addAtHead(snake.snake_body, prevHead);
        //printf("Snake X: %d\tY: %d\n", snake.snake_head.x, snake.snake_head.y);
        set_led_at(&led_matrix, snake.snake_head.x, snake.snake_head.y, 1);
        if(is_fruit_ate == false){
            if(snake.snake_body->size > 0){
                delFromTail(snake.snake_body);
            }
        }
        if(rslt == false){
            end_game();
            return;
        }else{
            if(collides_with_snake_body(snake.snake_head) == true){
                end_game();
                return;
            }
            PositionNode *pos = snake.snake_body->head;
            // printf("head: %x\n",snake.snake_body->head);
            if(pos != NULL){
                // printf("pos: %x\t pos->x: %d\t pos->y: %d\n", pos, pos->x, pos->y);
                set_led_at(&led_matrix, pos->x, pos->y, 1);
                // printf("pos->next: %x\n", pos->next);
                while(pos->next > 0){
                    pos = pos->next;
                    // printf("new pos: %x\n", pos);
                    set_led_at(&led_matrix, pos->x, pos->y, 1);
                }
                // printf("while pass?\n");
            }
            draw_screen(&led_matrix);
        }
        //delay_ms(0xCCFFFF);
        delay(2);
    }
}

void place_to_screen(int8_t rom_data[25], int8_t x, int8_t y){
    for(int tx = 0; tx < 5; tx++){
        for(int ty = 0; ty < 5; ty++){
            set_led_at(&led_matrix, x+tx, y+ty, rom_data[(tx*5)+ty]);
        }
    }
}

int moving_snake_path[60][2] = {
    // x, y
    {0, 4}, {1,4}, {2,4}, {3,4}, {3,3},
    {3,2}, {3,1}, {4,1}, {5,1}, {6,1},
    {6,2}, {6,3}, {6,4}, {6,5}, {6,6},
    {7,6}, {7,7}, {7,8}, {7,9}, {6,9},
    {5,9}, {5,10}, {5,11}, {5,12}, {6,12},
    {7,12}, {7,13}, {7,14}, {7,15}, {7,16},
    {6,16}, {6,17}, {5,17}, {5,18}, {5,19},
    {6,19}, {6,20}, {6,21}, {6,22}, {7,22},
    {7,23}, {7,24}, {7,25}, {7,26}, {7,27},
    {6,27}, {5,27}, {4,27}, {3,27}, {2,27},
    {1, 27}, {1,28}, {1,29}, {2,29}, {3,29},
    {4,29}, {5,29}, {6,29}, {6,30}, {6,31}
};

int moving_snake_forward = true;

void show_start_screen(){
    place_to_screen(rom[0], 0, 0+5);
    place_to_screen(rom[1], 0, 4+5);
    place_to_screen(rom[2], 0, 9+5);
    place_to_screen(rom[3], 0, 13+5);
    place_to_screen(rom[4], 0, 17+5);
    int step = 0;
    int8_t val = 1;
    int r = random_number()%60;
    set_led_at(&led_matrix, moving_snake_path[r][0],moving_snake_path[r][1], 1);
    while(!is_game_started && !is_game_over){
        if(moving_snake_forward == true){
            val = 1;
        }else{
            val = 0;
        }
        set_led_at(&led_matrix, moving_snake_path[step][0], moving_snake_path[step][1], val);
        if(step == 59){
            moving_snake_forward = !moving_snake_forward;

            if(moving_snake_forward == true){
                r = random_number()%60;
                set_led_at(&led_matrix, moving_snake_path[r][0],moving_snake_path[r][1], 1);
            }
        }
        step = (step+1)%60;

        draw_screen(&led_matrix);
        delay(1);
    }
}

int game_over_state = 0;


void delayed_invert_matrix(int _delay){
    invert_matrix(&led_matrix);
    delay(_delay);
    draw_screen(&led_matrix);
}


void show_game_over(){
    clean_screen(&led_matrix);
    while(!is_game_started && is_game_over){
        
        place_to_screen(rom[5], 1, 0+7);
        place_to_screen(rom[2], 1, 4+7);
        place_to_screen(rom[6], 1, 9+7);
        place_to_screen(rom[4], 1, 14+7);
        
        draw_screen(&led_matrix);
        delay(3);

        delayed_invert_matrix(2);
        delayed_invert_matrix(2);

        clean_screen(&led_matrix);
        place_to_screen(rom[10], 1, 1+7);
        place_to_screen(rom[8], 1, 5+7);
        place_to_screen(rom[4], 1, 9+7);
        place_to_screen(rom[9], 1, 13+7);
        
        draw_screen(&led_matrix);
        delay(3);

        delayed_invert_matrix(2);
        delayed_invert_matrix(2);
        
        clean_screen(&led_matrix);
        place_to_screen(rom[10+(snake.points/100)], 1, 10);
        place_to_screen(rom[10+((snake.points%100)/10)], 1, 14);
        place_to_screen(rom[10+(snake.points%10)], 1, 18);
        
        draw_screen(&led_matrix);
        delay(4);

        delayed_invert_matrix(2);
        delayed_invert_matrix(2);
        
        clean_screen(&led_matrix);
    }
}



int main(){
    // Clear Interrupts
    clear_interrupts();
    // Register new Interrupts
    register_interrupt(BUTTON_0_OFFSET, INT_DEVICE_BUTTON_0, &button_0_handler);
    // Finalize new Interrupts
    finalize_interrupts();
    
    // Init BMI160 Sensor
    init_bmi160_interface();

    // Init , clear and draw LED Matrix
    init_led_matrix(&led_matrix);
    set_intensity(128);
    clean_screen(&led_matrix);
    show_start_screen();
    draw_screen(&led_matrix);

    // Draw Snake to screen
    println("To start game press BTN0...");

    return 0;
}