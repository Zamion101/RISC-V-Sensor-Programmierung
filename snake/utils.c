#include <stdio.h>
#include <stdint.h>

void println(char* string){
    printf("%s\n", string);
}

void delay_ms(uint32_t delay){
    uint64_t end = (delay >> 1) + get_timer_value();
    while (get_timer_value() < end);
}

void delay(int seconds){
    int i, j, k;
    for(i = 0; i < seconds; i++)
    {
        for(j = 0; j < 10000; j++)
        {
            for(k = 0; k < 30; k++)
            {   
                int volatile t = 120 * j * i + k;
                t = t + 5;
            }
        }
    }
}

