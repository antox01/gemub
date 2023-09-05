#ifndef INTERRUPT_DEF_H
#define INTERRUPT_DEF_H

#include <stdint.h>

#define VBLANK_INTERRUPT (1<<0)
#define LCD_STAT_INTERRUPT (1<<1)
#define TIMER_INTERRUPT (1<<2)
#define SERIAL_INTERRUPT (1<<3) 
#define JOYPAD_INTERRUPT (1<<4)

typedef struct interrupt_s *interrupt_t;

interrupt_t initInterrupt();
void freeInterrupt(interrupt_t interrupt);

void interruptStep(interrupt_t interrupt);

uint8_t interruptGetEnable(interrupt_t interrupt);
void interruptSetEnable(interrupt_t interrupt, uint8_t value);
uint8_t interruptGetFlag(interrupt_t interrupt);
void interruptRequest(interrupt_t interrupt, uint8_t bit);

void interruptEnable(interrupt_t interrupt);
void interruptDisable(interrupt_t interrupt);

void interruptSetCpu(interrupt_t interrupt, void* cpu);

void vblank(interrupt_t interrupt);
void lcdStat(interrupt_t interrupt);
void timer(interrupt_t interrupt);
void serial(interrupt_t interrupt);
void joypad(interrupt_t interrupt);
#endif
