TARGET = snake
C_SRCS +=  utils.c rom.c snake_game.c interrupt.c spi.c led-matrix.c led_control.c bmi160.c snake.c
CFLAGS += -O2 -fno-builtin-printf -DUSE_PLIC

BSP_BASE = ../../bsp
C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c
include $(BSP_BASE)/env/common.mk
