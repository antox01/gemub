#include <stdlib.h>

#include "../includes/cpu.h"
#include "../includes/interrupt.h"

struct interrupt_s {
    uint8_t flag;
    uint8_t enable;
    uint8_t master;

    cpu_t cpu;
};

interrupt_t initInterrupt() {
    interrupt_t interrupt = malloc(sizeof(*interrupt));
    interrupt->flag = 0;
    interrupt->master = 1;
    interrupt->enable = 0xFF;

    return interrupt;
}

void freeInterrupt(interrupt_t interrupt) {
    if (interrupt == NULL) {
        return;
    }
    free(interrupt);
}

void interruptStep(interrupt_t interrupt) {
    uint8_t request;
    if(interrupt->master && interrupt->enable && interrupt->flag) {
        request = interrupt->enable & interrupt->flag;
        if (request & VBLANK_INTERRUPT) {
            interrupt->flag &= ~VBLANK_INTERRUPT;
            vblank(interrupt);
        }
        if (request & LCD_STAT_INTERRUPT) {
            interrupt->flag &= ~LCD_STAT_INTERRUPT;
        }
        if (request & TIMER_INTERRUPT) {
            interrupt->flag &= ~TIMER_INTERRUPT;
        }
        if (request & SERIAL_INTERRUPT) {
            interrupt->flag &= ~SERIAL_INTERRUPT;
        }
        if (request & JOYPAD_INTERRUPT) {
            interrupt->flag &= ~JOYPAD_INTERRUPT;
        }
    }
}

void interruptSetEnable(interrupt_t interrupt, uint8_t value) {
    interrupt->enable = value;
}

uint8_t interruptGetEnable(interrupt_t interrupt) {
    return interrupt->enable;
}

void interruptRequest(interrupt_t interrupt, uint8_t bit) {
    interrupt->enable |= bit;
}

uint8_t interruptGetFlag(interrupt_t interrupt) {
    return interrupt->enable;
}


void interruptEnable(interrupt_t interrupt) {
    interrupt->master = 1;
}

void interruptDisable(interrupt_t interrupt) {
    interrupt->master = 0;
}

void interruptSetCpu(interrupt_t interrupt, void* cpu) {
    interrupt->cpu = (cpu_t)cpu;
}

void vblank(interrupt_t interrupt) {
    interrupt->master = 0;
    cpuInt(interrupt->cpu, 0x40);
}

void lcdStat(interrupt_t interrupt) {
    interrupt->master = 0;
    cpuInt(interrupt->cpu, 0x48);
}

void timer(interrupt_t interrupt) {
    interrupt->master = 0;
    cpuInt(interrupt->cpu, 0x50);
}

void serial(interrupt_t interrupt) {
    interrupt->master = 0;
    cpuInt(interrupt->cpu, 0x58);
}

void joypad(interrupt_t interrupt) {
    interrupt->master = 0;
    cpuInt(interrupt->cpu, 0x60);
}
