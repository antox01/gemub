#ifndef CPU_DEF_H
#define CPU_DEF_H

#include "memory.h"
enum flag_e {
    FLAG_CARRY = 1 << 4,
    FLAG_HIGH = 1 << 5,
    FLAG_SUB = 1<<6,
    FLAG_ZERO = 1<<7
};

typedef struct cpu_s *cpu_t;

#define FLAG_SET(f, x) f |= x
#define FLAG_CLEAR(f, x) f &= ~(x)

#define A_COMBINATION \
    DEFINE_FUNC(void, a, b) \
    DEFINE_FUNC(void, a, c) \
    DEFINE_FUNC(void, a, d) \
    DEFINE_FUNC(void, a, e) \
    DEFINE_FUNC(void, a, h) \
    DEFINE_FUNC(void, a, l)


#define REGISTERS_COMBINATION \
    A_COMBINATION \
    DEFINE_FUNC(void, b, a) \
    DEFINE_FUNC(void, b, c) \
    DEFINE_FUNC(void, b, d) \
    DEFINE_FUNC(void, b, e) \
    DEFINE_FUNC(void, b, h) \
    DEFINE_FUNC(void, b, l) \
    DEFINE_FUNC(void, c, a) \
    DEFINE_FUNC(void, c, b) \
    DEFINE_FUNC(void, c, d) \
    DEFINE_FUNC(void, c, e) \
    DEFINE_FUNC(void, c, h) \
    DEFINE_FUNC(void, c, l) \
    DEFINE_FUNC(void, d, a) \
    DEFINE_FUNC(void, d, b) \
    DEFINE_FUNC(void, d, c) \
    DEFINE_FUNC(void, d, e) \
    DEFINE_FUNC(void, d, h) \
    DEFINE_FUNC(void, d, l) \
    DEFINE_FUNC(void, e, a) \
    DEFINE_FUNC(void, e, b) \
    DEFINE_FUNC(void, e, c) \
    DEFINE_FUNC(void, e, d) \
    DEFINE_FUNC(void, e, h) \
    DEFINE_FUNC(void, e, l) \
    DEFINE_FUNC(void, h, a) \
    DEFINE_FUNC(void, h, b) \
    DEFINE_FUNC(void, h, c) \
    DEFINE_FUNC(void, h, d) \
    DEFINE_FUNC(void, h, e) \
    DEFINE_FUNC(void, h, l) \
    DEFINE_FUNC(void, l, a) \
    DEFINE_FUNC(void, l, b) \
    DEFINE_FUNC(void, l, c) \
    DEFINE_FUNC(void, l, d) \
    DEFINE_FUNC(void, l, e) \
    DEFINE_FUNC(void, l, h) \


#define REGISTERS_LIST \
    DEFINE_FUNC(void, a) \
    DEFINE_FUNC(void, b) \
    DEFINE_FUNC(void, c) \
    DEFINE_FUNC(void, d) \
    DEFINE_FUNC(void, e) \
    DEFINE_FUNC(void, h) \
    DEFINE_FUNC(void, l)

#define DOUBLE_REGISTERS_LIST \
    DEFINE_FUNC(void, bc) \
    DEFINE_FUNC(void, de) \
    DEFINE_FUNC(void, hl)


/**** LD 2 registers functions definitions ****/
#define DEFINE_FUNC(ret, reg1, reg2) \
    ret ld_##reg1##_##reg2 (cpu_t cpu);

REGISTERS_COMBINATION
DEFINE_FUNC(void, sp, hl)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_##reg1##_hlp (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_hlp_##reg1 (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_##reg1 (cpu_t cpu, uint8_t value);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** LD double_registers-immediate functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret ld_##reg1 (cpu_t cpu, uint16_t value);

DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

/**** ADD 2 registers functions definitions ****/
#define DEFINE_FUNC(ret, reg1, reg2) \
    ret add_##reg1##_##reg2 (cpu_t cpu);

A_COMBINATION
DEFINE_FUNC(void, a, a)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret add_hl_##reg1 (cpu_t cpu);

DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1, reg2) \
    ret adc_##reg1##_##reg2 (cpu_t cpu);

DEFINE_FUNC(void, a, a)
A_COMBINATION
#undef DEFINE_FUNC

/**** SUB functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret sub_##reg1 (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** SBC functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret sbc_##reg1 (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** INC functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret inc_##reg1 (cpu_t cpu);

REGISTERS_LIST
DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

/**** DEC functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret dec_##reg1 (cpu_t cpu);

REGISTERS_LIST
DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

/**** AND functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret and_##reg1 (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** OR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret or_##reg1 (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** XOR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret xor_##reg1 (cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** CP functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret cp_##reg1(cpu_t cpu);

REGISTERS_LIST
#undef DEFINE_FUNC

/**** PUSH functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret push_##reg1(cpu_t cpu);

DOUBLE_REGISTERS_LIST
#undef DEFINE_FUNC

/**** POP functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret pop_##reg1(cpu_t cpu);

DOUBLE_REGISTERS_LIST
#undef DEFINE_FUNC

void nop(cpu_t cpu);
void cpl(cpu_t cpu);

void and_hlp(cpu_t cpu);
void or_hlp(cpu_t cpu);
void xor_hlp(cpu_t cpu);
void cp_hlp(cpu_t cpu);
void ld_hl_spn(cpu_t cpu, uint8_t val);

void add_a_hlp (cpu_t cpu);
void add_a_val (cpu_t cpu, uint8_t val);
void adc_a_hlp (cpu_t cpu);
void adc_a_val (cpu_t cpu, uint8_t val);

void cp_val(cpu_t cpu, uint8_t val);
void cp_hlp(cpu_t cpu);

cpu_t initCpu(memory_t memory);
char cpuOperandSize(unsigned char opcode);
void cpuPrintInstruction(unsigned char opcode, unsigned short operand);

#endif
