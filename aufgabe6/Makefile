TARGET = aufgabe6
C_SRCS += aufgabe6.c interrupt.c print.c spi.c led_control.c led-matrix.c
CFLAGS += -O2 -fno-builtin-printf -DUSE_PLIC

BSP_BASE = ../../bsp
C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c
include $(BSP_BASE)/env/common.mk
