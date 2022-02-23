#include "encoding.h"
#include "plic/plic_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "platform.h"

#ifndef MIE_MEIE
#define MIE_MEIE (1 << 11)
#endif

#ifndef MIE_MTIE
#define MIE_MTIE (1 << 7)
#endif

typedef void(*function_ptr_t)(void);

void clear_interrupts();
void register_interrupt(uint8_t button_offset, uint8_t int_device_button, function_ptr_t handler);

void finalize_interrupts();