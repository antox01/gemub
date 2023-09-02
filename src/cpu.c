#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/cpu.h"

typedef struct {
    union {
        struct {
            uint8_t f; // Flags: ZNHC----
            uint8_t a;
        };
        uint16_t af;
    };
    union {
        struct {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };
    union {
        struct {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };
    union {
        struct {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };
    uint16_t sp;
    uint16_t pc;
} registers_t;

typedef struct {
    char *code;
    int8_t operandSize;
    void *func;
} instruction_t;

struct cpu_s {
    registers_t registers;
    memory_t memory;
};


const instruction_t instructions[256] = {
    { "NOP",                        0, nop},                           // 0x00
	{ "LD BC, 0x%04X",              2, ld_bc},            // 0x01
	{ "LD (BC), A",                 0, ld_bcp_a},               // 0x02
	{ "INC BC",                     0, inc_bc},                     // 0x03
	{ "INC B",                      0, inc_b},                       // 0x04
	{ "DEC B",                      0, dec_b},                       // 0x05
	{ "LD B, 0x%02X",               1, ld_b},               // 0x06
	{ "RLCA",                       0, rlca},                         // 0x07
	{ "LD (0x%04X), SP",            2, ld_add_sp},         // 0x08
	{ "ADD HL, BC",                 0, add_hl_bc},              // 0x09
	{ "LD A, (BC)",                 0, ld_a_bcp},               // 0x0a
	{ "DEC BC",                     0, dec_bc},                     // 0x0b
	{ "INC C",                      0, inc_c},                       // 0x0c
	{ "DEC C",                      0, dec_c},                       // 0x0d
	{ "LD C, 0x%02X",               1, ld_c},               // 0x0e
	{ "RRCA",                       0, rrca},                         // 0x0f
	{ "STOP",                       1, NULL},                         // 0x10
	{ "LD DE, 0x%04X",              2, ld_de},            // 0x11
	{ "LD (DE), A",                 0, ld_dep_a},               // 0x12
	{ "INC DE",                     0, inc_de},                     // 0x13
	{ "INC D",                      0, inc_d},                       // 0x14
	{ "DEC D",                      0, dec_d},                       // 0x15
	{ "LD D, 0x%02X",               1, ld_d},               // 0x16
	{ "RLA",                        0, rla},                           // 0x17
	{ "JR 0x%02X",                  1, jr_val},                    // 0x18
	{ "ADD HL, DE",                 0, add_hl_de},              // 0x19
	{ "LD A, (DE)",                 0, ld_a_dep},               // 0x1a
	{ "DEC DE",                     0, dec_de},                     // 0x1b
	{ "INC E",                      0, inc_e},                       // 0x1c
	{ "DEC E",                      0, dec_e},                       // 0x1d
	{ "LD E, 0x%02X",               1, ld_e},               // 0x1e
	{ "RRA",                        0, rra},                           // 0x1f
	{ "JR NZ, 0x%02X",              1, jr_nz_val},             // 0x20
	{ "LD HL, 0x%04X",              2, ld_hl},            // 0x21
	{ "LDI (HL), A",                0, ldi_hlp_a},             // 0x22
	{ "INC HL",                     0, inc_hl},                     // 0x23
	{ "INC H",                      0, inc_h},                       // 0x24
	{ "DEC H",                      0, dec_h},                       // 0x25
	{ "LD H, 0x%02X",               1, ld_h},               // 0x26
	{ "DAA",                        0, NULL},                           // 0x27
	{ "JR Z, 0x%02X",               1, jr_z_val},               // 0x28
	{ "ADD HL, HL",                 0, add_hl_hl},              // 0x29
	{ "LDI A, (HL)",                0, ldi_a_hlp},             // 0x2a
	{ "DEC HL",                     0, dec_hl},                     // 0x2b
	{ "INC L",                      0, inc_l},                       // 0x2c
	{ "DEC L",                      0, dec_l},                       // 0x2d
	{ "LD L, 0x%02X",               1, ld_l},               // 0x2e
	{ "CPL",                        0, cpl},                           // 0x2f
	{ "JR NC, 0x%02X",              1, jr_nc_val},             // 0x30
	{ "LD SP, 0x%04X",              2, ld_sp},            // 0x31
	{ "LDD (HL), A",                0, ldd_hlp_a},             // 0x32
	{ "INC SP",                     0, inc_sp},                     // 0x33
	{ "INC (HL)",                   0, inc_hlp},                  // 0x34
	{ "DEC (HL)",                   0, dec_hlp},                  // 0x35
	{ "LD (HL), 0x%02X",            1, ld_hlp_val},          // 0x36
	{ "SCF",                        0, scf},                           // 0x37
	{ "JR C, 0x%02X",               1, jr_c_val},               // 0x38
	{ "ADD HL, SP",                 0, add_hl_sp},              // 0x39
	{ "LDD A, (HL)",                0, ldd_a_hlp},             // 0x3a
	{ "DEC SP",                     0, dec_sp},                     // 0x3b
	{ "INC A",                      0, inc_a},                       // 0x3c
	{ "DEC A",                      0, dec_a},                       // 0x3d
	{ "LD A, 0x%02X",               1, ld_a},               // 0x3e
	{ "CCF",                        0, ccf},                           // 0x3f
	{ "LD B, B",                    0, nop},                       // 0x40
	{ "LD B, C",                    0, ld_b_c},                    // 0x41
	{ "LD B, D",                    0, ld_b_d},                    // 0x42
	{ "LD B, E",                    0, ld_b_e},                    // 0x43
	{ "LD B, H",                    0, ld_b_h},                    // 0x44
	{ "LD B, L",                    0, ld_b_l},                    // 0x45
	{ "LD B, (HL)",                 0, ld_b_hlp},               // 0x46
	{ "LD B, A",                    0, ld_b_a},                    // 0x47
	{ "LD C, B",                    0, ld_c_b},                    // 0x48
	{ "LD C, C",                    0, nop},                       // 0x49
	{ "LD C, D",                    0, ld_c_d},                    // 0x4a
	{ "LD C, E",                    0, ld_c_e},                    // 0x4b
	{ "LD C, H",                    0, ld_c_h},                    // 0x4c
	{ "LD C, L",                    0, ld_c_l},                    // 0x4d
	{ "LD C, (HL)",                 0, ld_c_hlp},               // 0x4e
	{ "LD C, A",                    0, ld_c_a},                    // 0x4f
	{ "LD D, B",                    0, ld_d_b},                    // 0x50
	{ "LD D, C",                    0, ld_d_c},                    // 0x51
	{ "LD D, D",                    0, nop},                       // 0x52
	{ "LD D, E",                    0, ld_d_e},                    // 0x53
	{ "LD D, H",                    0, ld_d_h},                    // 0x54
	{ "LD D, L",                    0, ld_d_l},                    // 0x55
	{ "LD D, (HL)",                 0, ld_d_hlp},               // 0x56
	{ "LD D, A",                    0, ld_d_a},                    // 0x57
	{ "LD E, B",                    0, ld_e_b},                    // 0x58
	{ "LD E, C",                    0, ld_e_c},                    // 0x59
	{ "LD E, D",                    0, ld_e_d},                    // 0x5a
	{ "LD E, E",                    0, nop},                       // 0x5b
	{ "LD E, H",                    0, ld_e_h},                    // 0x5c
	{ "LD E, L",                    0, ld_e_l},                    // 0x5d
	{ "LD E, (HL)",                 0, ld_e_hlp},               // 0x5e
	{ "LD E, A",                    0, ld_e_a},                    // 0x5f
	{ "LD H, B",                    0, ld_h_b},                    // 0x60
	{ "LD H, C",                    0, ld_h_c},                    // 0x61
	{ "LD H, D",                    0, ld_h_d},                    // 0x62
	{ "LD H, E",                    0, ld_h_e},                    // 0x63
	{ "LD H, H",                    0, nop},                       // 0x64
	{ "LD H, L",                    0, ld_h_l},                    // 0x65
	{ "LD H, (HL)",                 0, ld_h_hlp},               // 0x66
	{ "LD H, A",                    0, ld_h_a},                    // 0x67
	{ "LD L, B",                    0, ld_l_b},                    // 0x68
	{ "LD L, C",                    0, ld_l_c},                    // 0x69
	{ "LD L, D",                    0, ld_l_d},                    // 0x6a
	{ "LD L, E",                    0, ld_l_e},                    // 0x6b
	{ "LD L, H",                    0, ld_l_h},                    // 0x6c
	{ "LD L, L",                    0, nop},                       // 0x6d
	{ "LD L, (HL)",                 0, ld_l_hlp},               // 0x6e
	{ "LD L, A",                    0, ld_l_a},                    // 0x6f
	{ "LD (HL), B",                 0, ld_hlp_b},               // 0x70
	{ "LD (HL), C",                 0, ld_hlp_c},               // 0x71
	{ "LD (HL), D",                 0, ld_hlp_d},               // 0x72
	{ "LD (HL), E",                 0, ld_hlp_e},               // 0x73
	{ "LD (HL), H",                 0, ld_hlp_h},               // 0x74
	{ "LD (HL), L",                 0, ld_hlp_l},               // 0x75
	{ "HALT",                       0, NULL},               // 0x76
	{ "LD (HL), A",                 0, ld_hlp_a},               // 0x77
	{ "LD A, B",                    0, ld_a_b},                    // 0x78
	{ "LD A, C",                    0, ld_a_c},                    // 0x79
	{ "LD A, D",                    0, ld_a_d},                    // 0x7a
	{ "LD A, E",                    0, ld_a_e},                    // 0x7b
	{ "LD A, H",                    0, ld_a_h},                    // 0x7c
	{ "LD A, L",                    0, ld_a_l},                    // 0x7d
	{ "LD A, (HL)",                 0, ld_a_hlp},               // 0x7e
	{ "LD A, A",                    0, nop},                       // 0x7f
	{ "ADD A, B",                   0, add_a_b},                  // 0x80
	{ "ADD A, C",                   0, add_a_b},                  // 0x81
	{ "ADD A, D",                   0, add_a_b},                  // 0x82
	{ "ADD A, E",                   0, add_a_b},                  // 0x83
	{ "ADD A, H",                   0, add_a_b},                  // 0x84
	{ "ADD A, L",                   0, add_a_b},                  // 0x85
	{ "ADD A, (HL)",                0, add_a_hlp},                     // 0x86
	{ "ADD A",                      0, add_a_b},                     // 0x87
	{ "ADC B",                      0, adc_a_b},                       // 0x88
	{ "ADC C",                      0, adc_a_c},                       // 0x89
	{ "ADC D",                      0, adc_a_d},                       // 0x8a
	{ "ADC E",                      0, adc_a_e},                       // 0x8b
	{ "ADC H",                      0, adc_a_h},                       // 0x8c
	{ "ADC L",                      0, adc_a_l},                       // 0x8d
	{ "ADC (HL)",                   0, adc_a_hlp},                  // 0x8e
	{ "ADC A",                      0, adc_a_a},                       // 0x8f
	{ "SUB B",                      0, sub_b},                       // 0x90
	{ "SUB C",                      0, sub_c},                       // 0x91
	{ "SUB D",                      0, sub_d},                       // 0x92
	{ "SUB E",                      0, sub_e},                       // 0x93
	{ "SUB H",                      0, sub_h},                       // 0x94
	{ "SUB L",                      0, sub_l},                       // 0x95
	{ "SUB (HL)",                   0, sub_hlp},                  // 0x96
	{ "SUB A",                      0, sub_a},                       // 0x97
	{ "SBC B",                      0, sbc_b},                       // 0x98
	{ "SBC C",                      0, sbc_c},                       // 0x99
	{ "SBC D",                      0, sbc_d},                       // 0x9a
	{ "SBC E",                      0, sbc_e},                       // 0x9b
	{ "SBC H",                      0, sbc_h},                       // 0x9c
	{ "SBC L",                      0, sbc_l},                       // 0x9d
	{ "SBC (HL)",                   0, sbc_hlp},                  // 0x9e
	{ "SBC A",                      0, sbc_a},                       // 0x9f
	{ "AND B",                      0, and_b},                       // 0xa0
	{ "AND C",                      0, and_c},                       // 0xa1
	{ "AND D",                      0, and_d},                       // 0xa2
	{ "AND E",                      0, and_e},                       // 0xa3
	{ "AND H",                      0, and_h},                       // 0xa4
	{ "AND L",                      0, and_l},                       // 0xa5
	{ "AND (HL)",                   0, and_hlp},                  // 0xa6
	{ "AND A",                      0, and_a},                       // 0xa7
	{ "XOR B",                      0, xor_b},                       // 0xa8
	{ "XOR C",                      0, xor_c},                       // 0xa9
	{ "XOR D",                      0, xor_d},                       // 0xaa
	{ "XOR E",                      0, xor_e},                       // 0xab
	{ "XOR H",                      0, xor_h},                       // 0xac
	{ "XOR L",                      0, xor_l},                       // 0xad
	{ "XOR (HL)",                   0, xor_hlp},                  // 0xae
	{ "XOR A",                      0, xor_a},                       // 0xaf
	{ "OR B",                       0, or_b},                         // 0xb0
	{ "OR C",                       0, or_c},                         // 0xb1
	{ "OR D",                       0, or_d},                         // 0xb2
	{ "OR E",                       0, or_e},                         // 0xb3
	{ "OR H",                       0, or_h},                         // 0xb4
	{ "OR L",                       0, or_l},                         // 0xb5
	{ "OR (HL)",                    0, or_hlp},                    // 0xb6
	{ "OR A",                       0, or_a},                         // 0xb7
	{ "CP B",                       0, cp_b},                         // 0xb8
	{ "CP C",                       0, cp_c},                         // 0xb9
	{ "CP D",                       0, cp_d},                         // 0xba
	{ "CP E",                       0, cp_e},                         // 0xbb
	{ "CP H",                       0, cp_h},                         // 0xbc
	{ "CP L",                       0, cp_l},                         // 0xbd
	{ "CP (HL)",                    0, cp_hlp},                    // 0xbe
	{ "CP A",                       0, cp_a},                         // 0xbf
	{ "RET NZ",                     0, NULL},                     // 0xc0
	{ "POP BC",                     0, pop_bc},                     // 0xc1
	{ "JP NZ, 0x%04X",              2, jp_nz_val},            // 0xc2
	{ "JP 0x%04X",                  2, jp_val},                   // 0xc3
	{ "CALL NZ, 0x%04X",            2, call_nz_val},        // 0xc4
	{ "PUSH BC",                    0, push_bc},                   // 0xc5
	{ "ADD A, 0x%02X",              1, add_a_val},             // 0xc6
	{ "RST 0x00",                   0, rst_0x00},                    // 0xc7
	{ "RET Z",                      0, NULL},                       // 0xc8
	{ "RET",                        0, NULL},                           // 0xc9
	{ "JP Z, 0x%04X",               2, jp_z_val},              // 0xca
	{ "CB %02X",                    1, NULL},                      // 0xcb
	{ "CALL Z, 0x%04X",             2, call_z_val},          // 0xcc
	{ "CALL 0x%04X",                2, call_val},               // 0xcd
	{ "ADC 0x%02X",                 1, adc_a_val},                  // 0xce
	{ "RST 0x08",                   0, rst_0x08},                   // 0xcf
	{ "RET NC",                     0, NULL},                     // 0xd0
	{ "POP DE",                     0, pop_de},                     // 0xd1
	{ "JP NC, 0x%04X",              2, jp_nc_val},            // 0xd2
	{ "UNKNOWN",                    0, unknown},                 // 0xd3
	{ "CALL NC, 0x%04X",            2, call_nc_val},        // 0xd4
	{ "PUSH DE",                    0, push_de},                   // 0xd5
	{ "SUB 0x%02X",                 1, sub_val},                  // 0xd6
	{ "RST 0x10",                   0, rst_0x10},                   // 0xd7
	{ "RET C",                      0, NULL},                       // 0xd8
	{ "RETI",                       0, NULL},          // 0xd9
	{ "JP C, 0x%04X",               2, jp_c_val},              // 0xda
	{ "UNKNOWN",                    0, unknown},                 // 0xdb
	{ "CALL C, 0x%04X",             2, call_c_val},          // 0xdc
	{ "UNKNOWN",                    0, unknown},                 // 0xdd
	{ "SBC 0x%02X",                 1, sbc_val},                  // 0xde
	{ "RST 0x18",                   0, rst_0x18},                   // 0xdf
	{ "LD (0xFF00 + 0x%02X), A",    1, ld_add8_a},// 0xe0
	{ "POP HL",                     0, pop_hl},                     // 0xe1
	{ "LD (0xFF00 + C), A",         0, ld_cp_a},      // 0xe2
	{ "UNKNOWN",                    0, unknown},                 // 0xe3
	{ "UNKNOWN",                    0, unknown},                 // 0xe4
	{ "PUSH HL",                    0, push_hl},                   // 0xe5
	{ "AND 0x%02X",                 1, and_val},                  // 0xe6
	{ "RST 0x20",                   0, rst_0x20},                   // 0xe7
	{ "ADD SP,0x%02X",              1, add_sp_val},            // 0xe8
	{ "JP HL",                      0, jp_hl},                       // 0xe9
	{ "LD (0x%04X), A",             2, ld_add16_a},           // 0xea
	{ "UNKNOWN",                    0, unknown},                 // 0xeb
	{ "UNKNOWN",                    0, unknown},                 // 0xec
	{ "UNKNOWN",                    0, unknown},                 // 0xed
	{ "XOR 0x%02X",                 1, xor_val},                  // 0xee
	{ "RST 0x28",                   0, rst_0x28},                   // 0xef
	{ "LD A, (0xFF00 + 0x%02X)",    1, ld_a_add8},// 0xf0
	{ "POP AF",                     0, pop_af},                     // 0xf1
	{ "LD A, (0xFF00 + C)",         0, ld_a_cp},      // 0xf2
	{ "DI",                         0, NULL},                        // 0xf3
	{ "UNKNOWN",                    0, unknown},                 // 0xf4
	{ "PUSH AF",                    0, push_af},                   // 0xf5
	{ "OR 0x%02X",                  1, or_val},                    // 0xf6
	{ "RST 0x30",                   0, rst_0x30},                   // 0xf7
	{ "LD HL, SP+0x%02X",           1, ld_hl_spn},       // 0xf8
	{ "LD SP, HL",                  0, ld_sp_hl},                // 0xf9
	{ "LD A, (0x%04X)",             2, ld_a_add16},           // 0xfa
	{ "EI",                         0, NULL},                             // 0xfb
	{ "UNKNOWN",                    0, unknown},                 // 0xfc
	{ "UNKNOWN",                    0, unknown},                 // 0xfd
	{ "CP 0x%02X",                  1, cp_val},                    // 0xfe
	{ "RST 0x38",                   0, rst_0x38},               // 0xff
};

cpu_t initCpu(memory_t memory) {
    cpu_t cpu = (cpu_t) malloc(sizeof(*cpu));
    cpu->memory = memory;
    return cpu;
}

void freeCpu(cpu_t cpu) {
    if(cpu == NULL) {
        return;
    }
    cpu->memory = NULL;
    free(cpu);
}


char cpuOperandSize(uint8_t opcode) {
    return instructions[opcode].operandSize;
}

void cpuPrintInstruction(uint8_t opcode, uint16_t operand) {
    fprintf(stdout, instructions[opcode].code, operand);
    fprintf(stdout, "\n");
}

uint8_t add8(registers_t *registers, uint8_t reg1, uint8_t reg2) {
    uint16_t sum = reg1 + reg2;
    if((uint8_t)sum == 0) FLAG_SET(registers->f, FLAG_ZERO);/* Set the Z flag */
    else FLAG_CLEAR(registers->f, FLAG_ZERO);

    FLAG_CLEAR(registers->f, FLAG_SUB);/* Reset the N flag */
    if((reg1 & 0x0f) + (reg2 & 0x0f) > 0x0f)
        FLAG_SET(registers->f, FLAG_HIGH);/* Set the H flag */
    else
        FLAG_CLEAR(registers->f, FLAG_HIGH);
    if((sum & 0xff00))
        FLAG_SET(registers->f, FLAG_CARRY);/* Set the C flag */
    else
        FLAG_SET(registers->f, FLAG_CARRY);
    return sum;
}

uint16_t add16(registers_t *registers, uint16_t reg1, uint16_t reg2) {
    uint32_t sum = reg1 + reg2;

    FLAG_CLEAR(registers->f, FLAG_SUB);/* Reset the N flag */
    if((reg1 & 0x7ff) + (reg2 & 0x7ff) > 0x7ff)
        FLAG_SET(registers->f, FLAG_HIGH);/* Set the H flag */
    else
     FLAG_CLEAR(registers->f, FLAG_HIGH);
    if((sum & 0xffff0000))
        FLAG_SET(registers->f, FLAG_CARRY);/* Set the C flag */
    else
        FLAG_CLEAR(registers->f, FLAG_CARRY);/* Set the C flag */
    return sum;
}

uint8_t sub(registers_t *registers, uint8_t reg1, uint8_t reg2) {
    uint16_t res = reg1 - reg2;
    if((uint8_t)res == 0) FLAG_SET(registers->f, FLAG_ZERO);/* Set the Z flag */
    else FLAG_CLEAR(registers->f, FLAG_ZERO);

    FLAG_CLEAR(registers->f, FLAG_SUB);/* Reset the N flag */
    if((reg1 & 0x0f) < (reg2 & 0x0f))
        FLAG_SET(registers->f, FLAG_HIGH);/* Set the H flag */
    else
        FLAG_CLEAR(registers->f, FLAG_HIGH);
    if(reg1 < reg2)
        FLAG_SET(registers->f, FLAG_CARRY);/* Set the C flag */
    else
        FLAG_SET(registers->f, FLAG_CARRY);
    return res;
}

void push(cpu_t cpu, uint16_t value) {
    memoryWriteWord(cpu->memory, cpu->registers.sp, value);
    cpu->registers.sp -= 2;
}

uint16_t pop(cpu_t cpu) {
    uint16_t value = memoryReadWord(cpu->memory, cpu->registers.sp);
    cpu->registers.sp += 2;
    return value;
}

void unknown(cpu_t cpu) {
    fprintf(stderr, "Unknown opcode\n");
}

#define DEFINE_FUNC(ret, val1) \
    ret rst_##val1 (cpu_t cpu) {\
        push(cpu, cpu->registers.pc);\
        cpu->registers.pc = val1;\
    }

RST_ADDRESSES
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1, reg2) \
    ret ld_##reg1##_##reg2 (cpu_t cpu) {\
        cpu->registers.reg1 = cpu->registers.reg2;\
    }
REGISTERS_COMBINATION
DEFINE_FUNC(void, sp, hl)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_##reg1##_hlp (cpu_t cpu) {\
        uint8_t value = memoryReadByte(cpu->memory, cpu->registers.hl);\
        cpu->registers.reg1 = value;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_hlp_##reg1 (cpu_t cpu) {\
        memoryWriteByte(cpu->memory, cpu->registers.hl, cpu->registers.reg1);\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_##reg1 (cpu_t cpu, uint8_t value) {\
        cpu->registers.reg1 = value;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret ld_##reg1 (cpu_t cpu, uint16_t value) {\
        cpu->registers.reg1 = value;\
    }

DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1, reg2) \
    ret add_##reg1##_##reg2 (cpu_t cpu) {\
        cpu->registers.reg1 = add8(&(cpu->registers), cpu->registers.reg1, cpu->registers.reg2);\
    }
A_COMBINATION
DEFINE_FUNC(void, a, a)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret add_hl_##reg1 (cpu_t cpu) {\
        cpu->registers.hl = add16(&cpu->registers, cpu->registers.hl, cpu->registers.reg1);\
    }
DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1, reg2) \
    ret adc_##reg1##_##reg2 (cpu_t cpu) {\
        uint8_t carry = (cpu->registers.f & 1<<4) >> 4;\
        cpu->registers.reg1 = add8(&cpu->registers, cpu->registers.reg1, cpu->registers.reg2 + carry);\
    }
A_COMBINATION
DEFINE_FUNC(void, a, a)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret sub_##reg1 (cpu_t cpu) {\
        cpu->registers.a = sub(&cpu->registers, cpu->registers.a, cpu->registers.reg1);\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret sbc_##reg1 (cpu_t cpu) {\
        uint8_t carry = (cpu->registers.f & 1<<4) >> 4;\
        cpu->registers.a = sub(&cpu->registers, cpu->registers.a, cpu->registers.reg1 + carry);\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret inc_##reg1 (cpu_t cpu) {\
        cpu->registers.reg1 += 1;\
        if(cpu->registers.reg1 == 0) FLAG_SET(cpu->registers.f, FLAG_ZERO);/* Set the Z flag */\
        FLAG_CLEAR(cpu->registers.f, FLAG_SUB);/* Reset the N flag */\
        if((cpu->registers.reg1 & 0x0f) == 0) FLAG_SET(cpu->registers.f, FLAG_HIGH);/* Set the H flag */\
    }

REGISTERS_LIST
DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret dec_##reg1 (cpu_t cpu) {\
        if((cpu->registers.reg1 & 0xf) == 0) FLAG_SET(cpu->registers.f, FLAG_HIGH);\
        cpu->registers.reg1 -= 1;\
        if(cpu->registers.reg1 == 0) FLAG_SET(cpu->registers.f, FLAG_ZERO);\
        FLAG_SET(cpu->registers.f, FLAG_SUB);\
    }

REGISTERS_LIST
DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, sp)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret and_##reg1 (cpu_t cpu) {\
        cpu->registers.a &= cpu->registers.reg1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

/**** OR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret or_##reg1 (cpu_t cpu) {\
        cpu->registers.a |= cpu->registers.reg1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

/**** XOR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret xor_##reg1 (cpu_t cpu) {\
        cpu->registers.a ^= cpu->registers.reg1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret cp_##reg1(cpu_t cpu) {\
        sub(&cpu->registers, cpu->registers.a, cpu->registers.reg1);\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret push_##reg1(cpu_t cpu) {\
        push(cpu, cpu->registers.reg1);\
    }

DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, af)
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret pop_##reg1(cpu_t cpu) {\
        cpu->registers.reg1 = pop(cpu);\
    }

DOUBLE_REGISTERS_LIST
DEFINE_FUNC(void, af)
#undef DEFINE_FUNC

void nop(cpu_t cpu) { }

void cpl(cpu_t cpu) {
    cpu->registers.a = ~cpu->registers.a;
}

void and_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a &= val;
}

void or_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a |= val;
}

void xor_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a ^= val;
}

void and_val(cpu_t cpu, uint8_t val) {
    cpu->registers.a &= val;
}

void or_val(cpu_t cpu, uint8_t val) {
    cpu->registers.a |= val;
}

void xor_val(cpu_t cpu, uint8_t val) {
    cpu->registers.a ^= val;
}

void cp_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    sub(&cpu->registers, cpu->registers.a, val);
}

void cp_val(cpu_t cpu, uint8_t val) {
    sub(&cpu->registers, cpu->registers.a, val);
}

void ld_add_sp(cpu_t cpu, uint16_t address) {
    memoryWriteByte(cpu->memory, address, cpu->registers.sp);
}

void ld_hl_spn(cpu_t cpu, uint8_t val) {
    cpu->registers.hl = cpu->registers.sp + val;
}

void ld_hlp_val(cpu_t cpu, uint8_t val) {
    memoryWriteByte(cpu->memory, cpu->registers.hl, val);
}

void ld_a_bcp(cpu_t cpu) {
    memoryWriteByte(cpu->memory, cpu->registers.bc, cpu->registers.a);
}

void ld_bcp_a(cpu_t cpu) {
    cpu->registers.a = memoryReadByte(cpu->memory, cpu->registers.bc);
}

void ld_dep_a(cpu_t cpu) {
    memoryWriteByte(cpu->memory, cpu->registers.de, cpu->registers.a);
}

void ld_cp_a(cpu_t cpu) {
    memoryWriteByte(cpu->memory, 0xFF00 + cpu->registers.c, cpu->registers.a);
}

void ld_a_cp(cpu_t cpu) {
    cpu->registers.a = memoryReadByte(cpu->memory, 0xFF00 + cpu->registers.c);
}

void ld_a_dep(cpu_t cpu) {
    cpu->registers.a = memoryReadByte(cpu->memory, cpu->registers.de);
}

void ld_a_add16(cpu_t cpu, uint16_t address) {
    cpu->registers.a = memoryReadByte(cpu->memory, address);
}

void ld_add16_a(cpu_t cpu, uint16_t address) {
    memoryWriteByte(cpu->memory, address, cpu->registers.a);
}

void ld_a_add8(cpu_t cpu, uint8_t address) {
    cpu->registers.a = memoryReadByte(cpu->memory, 0xFF00 + address);
}

void ld_add8_a(cpu_t cpu, uint8_t address) {
    memoryWriteByte(cpu->memory, 0xff00 + address, cpu->registers.a);
}

void ldi_hlp_a(cpu_t cpu) {
    ld_hlp_a(cpu);
    inc_hl(cpu);
}

void ldi_a_hlp(cpu_t cpu) {
    ld_a_hlp(cpu);
    inc_hl(cpu);
}

void ldd_hlp_a(cpu_t cpu) {
    ld_hlp_a(cpu);
    dec_hl(cpu);
}

void ldd_a_hlp(cpu_t cpu) {
    ld_a_hlp(cpu);
    dec_hl(cpu);
}


void add_a_hlp (cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a = add8(&cpu->registers, cpu->registers.a, val);
}

void add_a_val (cpu_t cpu, uint8_t val) {
    cpu->registers.a = add8(&cpu->registers, cpu->registers.a, val);
}

void add_sp_val (cpu_t cpu, int8_t val) {
    cpu->registers.sp = add8(&cpu->registers, cpu->registers.sp, val);
}

void adc_a_hlp (cpu_t cpu) {
    uint8_t carry = (cpu->registers.f & 1<<4) >> 4;
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a = add8(&cpu->registers, cpu->registers.a, val + carry);
}

void adc_a_val (cpu_t cpu, uint8_t val) {
    uint8_t carry = (cpu->registers.f & 1<<4) >> 4;
    cpu->registers.a = add8(&cpu->registers, cpu->registers.a, val + carry);
}

void sub_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a = sub(&cpu->registers, cpu->registers.a, val);
}

void sbc_hlp(cpu_t cpu) {
    uint8_t carry = (cpu->registers.f & 1<<4) >> 4;
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    cpu->registers.a = sub(&cpu->registers, cpu->registers.a, val + carry);
}

void sub_val(cpu_t cpu, uint8_t val) {
    cpu->registers.a = sub(&cpu->registers, cpu->registers.a, val);
}

void sbc_val(cpu_t cpu, uint8_t val) {
    uint8_t carry = (cpu->registers.f & 1<<4) >> 4;
    cpu->registers.a = sub(&cpu->registers, cpu->registers.a, val + carry);
}

void inc_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    val++;
    if(val == 0)
        FLAG_SET(cpu->registers.f, FLAG_ZERO);/* Set the Z flag */\
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_ZERO);/* Set the Z flag */\
    FLAG_CLEAR(cpu->registers.f, FLAG_SUB);/* Reset the N flag */\
    if((val & 0x0f) == 0)
        FLAG_SET(cpu->registers.f, FLAG_HIGH);/* Set the H flag */\
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_HIGH);
    memoryWriteByte(cpu->memory, cpu->registers.hl, val);
}

void dec_hlp(cpu_t cpu) {
    uint8_t val = memoryReadByte(cpu->memory, cpu->registers.hl);
    if((val & 0xf) == 0)
        FLAG_SET(cpu->registers.f, FLAG_HIGH);
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_HIGH);
    val--;
    if(val == 0)
        FLAG_SET(cpu->registers.f, FLAG_ZERO);
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_ZERO);/* Set the Z flag */\
    FLAG_SET(cpu->registers.f, FLAG_SUB);
    memoryWriteByte(cpu->memory, cpu->registers.hl, val);
}

void ccf(cpu_t cpu) {
    if(isSetFlag(cpu->registers.f, FLAG_CARRY))
        FLAG_CLEAR(cpu->registers.f, FLAG_CARRY);
    else
        FLAG_SET(cpu->registers.f, FLAG_CARRY);
    FLAG_CLEAR(cpu->registers.f, FLAG_SUB);
    FLAG_CLEAR(cpu->registers.f, FLAG_HIGH);
}

void scf(cpu_t cpu) {
    FLAG_SET(cpu->registers.f, FLAG_CARRY);
    FLAG_CLEAR(cpu->registers.f, FLAG_SUB);
    FLAG_CLEAR(cpu->registers.f, FLAG_HIGH);
}

void jr_val(cpu_t cpu, uint8_t val) {
    cpu->registers.pc += val;
}

void jr_c_val(cpu_t cpu, uint8_t val) {
    if(!isSetFlag(cpu->registers.f, FLAG_CARRY))
        return;
    cpu->registers.pc += val;
}

void jr_nc_val(cpu_t cpu, uint8_t val) {
    if(isSetFlag(cpu->registers.f, FLAG_CARRY))
        return;
    cpu->registers.pc += val;
}

void jr_z_val(cpu_t cpu, uint8_t val) {
    if(!isSetFlag(cpu->registers.f, FLAG_ZERO))
        return;
    cpu->registers.pc += val;
}

void jr_nz_val(cpu_t cpu, uint8_t val) {
    if(isSetFlag(cpu->registers.f, FLAG_ZERO))
        return;
    cpu->registers.pc += val;
}

void jp_val(cpu_t cpu, uint16_t val) {
    cpu->registers.pc = val;
}

void jp_hl(cpu_t cpu) {
    cpu->registers.pc = cpu->registers.hl;
}

void jp_c_val(cpu_t cpu, uint16_t val) {
    if(!isSetFlag(cpu->registers.f, FLAG_CARRY))
        return;
    cpu->registers.pc = val;
}

void jp_nc_val(cpu_t cpu, uint16_t val) {
    if(isSetFlag(cpu->registers.f, FLAG_CARRY))
        return;
    cpu->registers.pc = val;
}

void jp_z_val(cpu_t cpu, uint16_t val) {
    if(!isSetFlag(cpu->registers.f, FLAG_ZERO))
        return;
    cpu->registers.pc = val;
}

void jp_nz_val(cpu_t cpu, uint16_t val) {
    if(isSetFlag(cpu->registers.f, FLAG_ZERO))
        return;
    cpu->registers.pc = val;
}

void call_val(cpu_t cpu, uint16_t val) {
    // nextAddress is given by adding to the current pc the value 3
    // because 2 byte are read for the value, and the next one is the
    // next instruction
    uint16_t nextAddress = cpu->registers.pc + 3;
    push(cpu, nextAddress);
    cpu->registers.pc = val;
}

void call_c_val(cpu_t cpu, uint16_t val) {
    if(!isSetFlag(cpu->registers.f, FLAG_CARRY))
        return;
    uint16_t nextAddress = cpu->registers.pc + 3;
    push(cpu, nextAddress);
    cpu->registers.pc = val;
}

void call_nc_val(cpu_t cpu, uint16_t val) {
    if(isSetFlag(cpu->registers.f, FLAG_CARRY))
        return;
    uint16_t nextAddress = cpu->registers.pc + 3;
    push(cpu, nextAddress);
    cpu->registers.pc = val;
}

void call_z_val(cpu_t cpu, uint16_t val) {
    if(!isSetFlag(cpu->registers.f, FLAG_ZERO))
        return;
    uint16_t nextAddress = cpu->registers.pc + 3;
    push(cpu, nextAddress);
    cpu->registers.pc = val;
}

void call_nz_val(cpu_t cpu, uint16_t val) {
    if(isSetFlag(cpu->registers.f, FLAG_ZERO))
        return;
    uint16_t nextAddress = cpu->registers.pc + 3;
    push(cpu, nextAddress);
    cpu->registers.pc = val;
}

void rla(cpu_t cpu) {
    uint8_t carry = (isSetFlag(cpu->registers.f, FLAG_CARRY)) ? 1 : 0;
    uint8_t oldBit = cpu->registers.a & 0x80;

    if(oldBit)
        FLAG_SET(cpu->registers.f, FLAG_CARRY);
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_CARRY);

    cpu->registers.a <<= 1;
    if(carry) cpu->registers.a |= 0x1;

    FLAG_CLEAR(cpu->registers.f, FLAG_ZERO | FLAG_HIGH | FLAG_SUB);
}

void rlca(cpu_t cpu) {
    uint8_t oldBit = cpu->registers.a & 0x80;

    if(oldBit)
        FLAG_SET(cpu->registers.f, FLAG_CARRY);
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_CARRY);

    cpu->registers.a <<= 1;
    if(oldBit) cpu->registers.a |= 0x1;

    FLAG_CLEAR(cpu->registers.f, FLAG_ZERO | FLAG_HIGH | FLAG_SUB);
}

void rra(cpu_t cpu) {
    uint8_t carry = (isSetFlag(cpu->registers.f, FLAG_CARRY)) ? 1 : 0;
    uint8_t oldBit = cpu->registers.a & 0x1;

    if(oldBit)
        FLAG_SET(cpu->registers.f, FLAG_CARRY);
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_CARRY);

    cpu->registers.a >>= 1;
    if(carry) cpu->registers.a |= 0x80;

    FLAG_CLEAR(cpu->registers.f, FLAG_ZERO | FLAG_HIGH | FLAG_SUB);
}

void rrca(cpu_t cpu) {
    uint8_t oldBit = cpu->registers.a & 0x1;

    if(oldBit)
        FLAG_SET(cpu->registers.f, FLAG_CARRY);
    else
        FLAG_CLEAR(cpu->registers.f, FLAG_CARRY);

    cpu->registers.a >>= 1;
    if(oldBit) cpu->registers.a |= 0x80;

    FLAG_CLEAR(cpu->registers.f, FLAG_ZERO | FLAG_HIGH | FLAG_SUB);
}

