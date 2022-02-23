#include "encoding.h"
#include "plic/plic_driver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "platform.h"
#include "interrupt.h"

#ifndef MIE_MEIE
#define MIE_MEIE (1 << 11)
#endif

#ifndef MIE_MTIE
#define MIE_MTIE (1 << 7)
#endif

typedef void(*function_ptr_t)(void);
void no_interrupt_handler(void) {}

function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];

plic_instance_t g_plic;

void handle_m_ext_interrupt()
{
  plic_source int_num = PLIC_claim_interrupt(&g_plic);
  if ((int_num >= 1) && (int_num < PLIC_NUM_INTERRUPTS))
  {
    g_ext_interrupt_handlers[int_num]();
  }
  else
  {
    exit(1);
  }
  PLIC_complete_interrupt(&g_plic, int_num);
}

void clear_interrupts(){
    PLIC_init(&g_plic, PLIC_CTRL_ADDR, PLIC_NUM_INTERRUPTS,
            PLIC_NUM_PRIORITIES);
    clear_csr(mie, MIE_MEIE);
    clear_csr(mie, MIE_MTIE);
    for (int ii = 0; ii < PLIC_NUM_INTERRUPTS; ii++)
    {
        g_ext_interrupt_handlers[ii] = no_interrupt_handler;
    }
}

void register_interrupt(uint8_t button_offset, uint8_t int_device_button, function_ptr_t handler){
    GPIO_REG(GPIO_OUTPUT_EN) &= ~(1 << button_offset);
    GPIO_REG(GPIO_INPUT_EN) |= (1 << button_offset);
    GPIO_REG(GPIO_PULLUP_EN) &= ~(1 << button_offset);
    GPIO_REG(GPIO_RISE_IE) |= (1 << button_offset);
    
    g_ext_interrupt_handlers[int_device_button] = handler;
    PLIC_enable_interrupt(&g_plic, int_device_button);
    PLIC_set_priority(&g_plic, int_device_button, 1);
}

void finalize_interrupts(){
  set_csr(mie, MIE_MEIE);
  set_csr(mstatus, MSTATUS_MIE);
}