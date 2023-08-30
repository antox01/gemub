#include <stdint.h>
#include <stdio.h>

#include "../includes/cpu.h"

struct {
    union {
        struct {
            uint8_t f;
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
} registers;

typedef struct {
    char *code;
    int8_t operandSize;
    void (*func)(void);
} instruction_t;

#define A_COMBINATION \
    DEFINE_FUNC(void, a, a) \
    DEFINE_FUNC(void, a, b) \
    DEFINE_FUNC(void, a, c) \
    DEFINE_FUNC(void, a, d) \
    DEFINE_FUNC(void, a, e) \
    DEFINE_FUNC(void, a, h) \
    DEFINE_FUNC(void, a, l)


#define REGISTERS_COMBINATION \
    A_COMBINATION \
    DEFINE_FUNC(void, b, a) \
    DEFINE_FUNC(void, b, b) \
    DEFINE_FUNC(void, b, c) \
    DEFINE_FUNC(void, b, d) \
    DEFINE_FUNC(void, b, e) \
    DEFINE_FUNC(void, b, h) \
    DEFINE_FUNC(void, b, l) \
    DEFINE_FUNC(void, c, a) \
    DEFINE_FUNC(void, c, b) \
    DEFINE_FUNC(void, c, c) \
    DEFINE_FUNC(void, c, d) \
    DEFINE_FUNC(void, c, e) \
    DEFINE_FUNC(void, c, h) \
    DEFINE_FUNC(void, c, l) \
    DEFINE_FUNC(void, d, a) \
    DEFINE_FUNC(void, d, b) \
    DEFINE_FUNC(void, d, c) \
    DEFINE_FUNC(void, d, d) \
    DEFINE_FUNC(void, d, e) \
    DEFINE_FUNC(void, d, h) \
    DEFINE_FUNC(void, d, l) \
    DEFINE_FUNC(void, e, a) \
    DEFINE_FUNC(void, e, b) \
    DEFINE_FUNC(void, e, c) \
    DEFINE_FUNC(void, e, d) \
    DEFINE_FUNC(void, e, e) \
    DEFINE_FUNC(void, e, h) \
    DEFINE_FUNC(void, e, l) \
    DEFINE_FUNC(void, h, a) \
    DEFINE_FUNC(void, h, b) \
    DEFINE_FUNC(void, h, c) \
    DEFINE_FUNC(void, h, d) \
    DEFINE_FUNC(void, h, e) \
    DEFINE_FUNC(void, h, h) \
    DEFINE_FUNC(void, h, l) \
    DEFINE_FUNC(void, l, a) \
    DEFINE_FUNC(void, l, b) \
    DEFINE_FUNC(void, l, c) \
    DEFINE_FUNC(void, l, d) \
    DEFINE_FUNC(void, l, e) \
    DEFINE_FUNC(void, l, h) \
    DEFINE_FUNC(void, l, l) \


#define REGISTERS_LIST \
    DEFINE_FUNC(void, a) \
    DEFINE_FUNC(void, b) \
    DEFINE_FUNC(void, c) \
    DEFINE_FUNC(void, d) \
    DEFINE_FUNC(void, e) \
    DEFINE_FUNC(void, h) \
    DEFINE_FUNC(void, l)

#define DOUBLE_REGISTERS_LIST \
    DEFINE_FUNC(void, af) \
    DEFINE_FUNC(void, bc) \
    DEFINE_FUNC(void, de) \
    DEFINE_FUNC(void, hl) \


/**** LD 2 registers functions definitions ****/
#define DEFINE_FUNC(ret, reg1, reg2) \
    ret ld_##reg1##_##reg2 ();

REGISTERS_COMBINATION
#undef DEFINE_FUNC

/**** ADD 2 registers functions definitions ****/
#define DEFINE_FUNC(ret, reg1, reg2) \
    ret add_##reg1##_##reg2 ();

A_COMBINATION
#undef DEFINE_FUNC

/**** INC functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret inc_##reg1 ();

REGISTERS_LIST
#undef DEFINE_FUNC

/**** DEC functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret dec_##reg1 ();

REGISTERS_LIST
#undef DEFINE_FUNC

/**** AND functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret and_##reg1 ();

REGISTERS_LIST
#undef DEFINE_FUNC

/**** OR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret or_##reg1 ();

REGISTERS_LIST
#undef DEFINE_FUNC

/**** XOR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret xor_##reg1 ();

REGISTERS_LIST
#undef DEFINE_FUNC

const instruction_t instructions[256] = {
    { "NOP", 0},                           // 0x00
	{ "LD BC, 0x%04X", 2},            // 0x01
	{ "LD (BC), A", 0},               // 0x02
	{ "INC BC", 0},                     // 0x03
	{ "INC B", 0},                       // 0x04
	{ "DEC B", 0},                       // 0x05
	{ "LD B, 0x%02X", 1},               // 0x06
	{ "RLCA", 0},                         // 0x07
	{ "LD (0x%04X), SP", 2},         // 0x08
	{ "ADD HL, BC", 0},              // 0x09
	{ "LD A, (BC)", 0},               // 0x0a
	{ "DEC BC", 0},                     // 0x0b
	{ "INC C", 0},                       // 0x0c
	{ "DEC C", 0},                       // 0x0d
	{ "LD C, 0x%02X", 1},               // 0x0e
	{ "RRCA", 0},                         // 0x0f
	{ "STOP", 1},                         // 0x10
	{ "LD DE, 0x%04X", 2},            // 0x11
	{ "LD (DE), A", 0},               // 0x12
	{ "INC DE", 0},                     // 0x13
	{ "INC D", 0},                       // 0x14
	{ "DEC D", 0},                       // 0x15
	{ "LD D, 0x%02X", 1},               // 0x16
	{ "RLA", 0},                           // 0x17
	{ "JR 0x%02X", 1},                    // 0x18
	{ "ADD HL, DE", 0},              // 0x19
	{ "LD A, (DE)", 0},               // 0x1a
	{ "DEC DE", 0},                     // 0x1b
	{ "INC E", 0},                       // 0x1c
	{ "DEC E", 0},                       // 0x1d
	{ "LD E, 0x%02X", 1},               // 0x1e
	{ "RRA", 0},                           // 0x1f
	{ "JR NZ, 0x%02X", 1},             // 0x20
	{ "LD HL, 0x%04X", 2},            // 0x21
	{ "LDI (HL), A", 0},             // 0x22
	{ "INC HL", 0},                     // 0x23
	{ "INC H", 0},                       // 0x24
	{ "DEC H", 0},                       // 0x25
	{ "LD H, 0x%02X", 1},               // 0x26
	{ "DAA", 0},                           // 0x27
	{ "JR Z, 0x%02X", 1},               // 0x28
	{ "ADD HL, HL", 0},              // 0x29
	{ "LDI A, (HL)", 0},             // 0x2a
	{ "DEC HL", 0},                     // 0x2b
	{ "INC L", 0},                       // 0x2c
	{ "DEC L", 0},                       // 0x2d
	{ "LD L, 0x%02X", 1},               // 0x2e
	{ "CPL", 0},                           // 0x2f
	{ "JR NC, 0x%02X", 1},             // 0x30
	{ "LD SP, 0x%04X", 2},            // 0x31
	{ "LDD (HL), A", 0},             // 0x32
	{ "INC SP", 0},                     // 0x33
	{ "INC (HL)", 0},                  // 0x34
	{ "DEC (HL)", 0},                  // 0x35
	{ "LD (HL), 0x%02X", 1},          // 0x36
	{ "SCF", 0},                           // 0x37
	{ "JR C, 0x%02X", 1},               // 0x38
	{ "ADD HL, SP", 0},              // 0x39
	{ "LDD A, (HL)", 0},             // 0x3a
	{ "DEC SP", 0},                     // 0x3b
	{ "INC A", 0},                       // 0x3c
	{ "DEC A", 0},                       // 0x3d
	{ "LD A, 0x%02X", 1},               // 0x3e
	{ "CCF", 0},                           // 0x3f
	{ "LD B, B", 0, ld_b_b},                       // 0x40
	{ "LD B, C", 0, ld_b_c},                    // 0x41
	{ "LD B, D", 0, ld_b_d},                    // 0x42
	{ "LD B, E", 0, ld_b_e},                    // 0x43
	{ "LD B, H", 0, ld_b_h},                    // 0x44
	{ "LD B, L", 0, ld_b_l},                    // 0x45
	{ "LD B, (HL)", 0},               // 0x46
	{ "LD B, A", 0, ld_b_a},                    // 0x47
	{ "LD C, B", 0, ld_c_b},                    // 0x48
	{ "LD C, C", 0, ld_c_c},                       // 0x49
	{ "LD C, D", 0, ld_c_d},                    // 0x4a
	{ "LD C, E", 0, ld_c_e},                    // 0x4b
	{ "LD C, H", 0, ld_c_h},                    // 0x4c
	{ "LD C, L", 0, ld_c_l},                    // 0x4d
	{ "LD C, (HL)", 0},               // 0x4e
	{ "LD C, A", 0, ld_c_a},                    // 0x4f
	{ "LD D, B", 0, ld_d_b},                    // 0x50
	{ "LD D, C", 0, ld_d_c},                    // 0x51
	{ "LD D, D", 0, ld_d_d},                       // 0x52
	{ "LD D, E", 0, ld_d_e},                    // 0x53
	{ "LD D, H", 0, ld_d_h},                    // 0x54
	{ "LD D, L", 0, ld_d_l},                    // 0x55
	{ "LD D, (HL)", 0},               // 0x56
	{ "LD D, A", 0, ld_d_a},                    // 0x57
	{ "LD E, B", 0, ld_e_b},                    // 0x58
	{ "LD E, C", 0, ld_e_c},                    // 0x59
	{ "LD E, D", 0, ld_e_d},                    // 0x5a
	{ "LD E, E", 0, ld_e_e},                       // 0x5b
	{ "LD E, H", 0, ld_e_h},                    // 0x5c
	{ "LD E, L", 0, ld_e_l},                    // 0x5d
	{ "LD E, (HL)", 0},               // 0x5e
	{ "LD E, A", 0, ld_e_a},                    // 0x5f
	{ "LD H, B", 0, ld_h_b},                    // 0x60
	{ "LD H, C", 0, ld_h_c},                    // 0x61
	{ "LD H, D", 0, ld_h_d},                    // 0x62
	{ "LD H, E", 0, ld_h_e},                    // 0x63
	{ "LD H, H", 0, ld_h_h},                       // 0x64
	{ "LD H, L", 0, ld_h_l},                    // 0x65
	{ "LD H, (HL)", 0},               // 0x66
	{ "LD H, A", 0, ld_h_a},                    // 0x67
	{ "LD L, B", 0, ld_l_b},                    // 0x68
	{ "LD L, C", 0, ld_l_c},                    // 0x69
	{ "LD L, D", 0, ld_l_d},                    // 0x6a
	{ "LD L, E", 0, ld_l_e},                    // 0x6b
	{ "LD L, H", 0, ld_l_h},                    // 0x6c
	{ "LD L, L", 0, ld_l_l},                       // 0x6d
	{ "LD L, (HL)", 0},               // 0x6e
	{ "LD L, A", 0, ld_l_a},                    // 0x6f
	{ "LD (HL), B", 0},               // 0x70
	{ "LD (HL), C", 0},               // 0x71
	{ "LD (HL), D", 0},               // 0x72
	{ "LD (HL), E", 0},               // 0x73
	{ "LD (HL), H", 0},               // 0x74
	{ "LD (HL), L", 0},               // 0x75
	{ "HALT", 0},                         // 0x76
	{ "LD (HL), A", 0},               // 0x77
	{ "LD A, B", 0, ld_a_b},                    // 0x78
	{ "LD A, C", 0, ld_a_c},                    // 0x79
	{ "LD A, D", 0, ld_a_d},                    // 0x7a
	{ "LD A, E", 0, ld_a_e},                    // 0x7b
	{ "LD A, H", 0, ld_a_h},                    // 0x7c
	{ "LD A, L", 0, ld_a_l},                    // 0x7d
	{ "LD A, (HL)", 0},               // 0x7e
	{ "LD A, A", 0, ld_a_a},                       // 0x7f
	{ "ADD A, B", 0, add_a_b},                  // 0x80
	{ "ADD A, C", 0, add_a_b},                  // 0x81
	{ "ADD A, D", 0, add_a_b},                  // 0x82
	{ "ADD A, E", 0, add_a_b},                  // 0x83
	{ "ADD A, H", 0, add_a_b},                  // 0x84
	{ "ADD A, L", 0, add_a_b},                  // 0x85
	{ "ADD A, (HL)", 0},             // 0x86
	{ "ADD A", 0, add_a_b},                     // 0x87
	{ "ADC B", 0},                       // 0x88
	{ "ADC C", 0},                       // 0x89
	{ "ADC D", 0},                       // 0x8a
	{ "ADC E", 0},                       // 0x8b
	{ "ADC H", 0},                       // 0x8c
	{ "ADC L", 0},                       // 0x8d
	{ "ADC (HL)", 0},                  // 0x8e
	{ "ADC A", 0},                       // 0x8f
	{ "SUB B", 0},                       // 0x90
	{ "SUB C", 0},                       // 0x91
	{ "SUB D", 0},                       // 0x92
	{ "SUB E", 0},                       // 0x93
	{ "SUB H", 0},                       // 0x94
	{ "SUB L", 0},                       // 0x95
	{ "SUB (HL)", 0},                  // 0x96
	{ "SUB A", 0},                       // 0x97
	{ "SBC B", 0},                       // 0x98
	{ "SBC C", 0},                       // 0x99
	{ "SBC D", 0},                       // 0x9a
	{ "SBC E", 0},                       // 0x9b
	{ "SBC H", 0},                       // 0x9c
	{ "SBC L", 0},                       // 0x9d
	{ "SBC (HL)", 0},                  // 0x9e
	{ "SBC A", 0},                       // 0x9f
	{ "AND B", 0, and_b},                       // 0xa0
	{ "AND C", 0, and_c},                       // 0xa1
	{ "AND D", 0, and_d},                       // 0xa2
	{ "AND E", 0, and_e},                       // 0xa3
	{ "AND H", 0, and_h},                       // 0xa4
	{ "AND L", 0, and_l},                       // 0xa5
	{ "AND (HL)", 0},                  // 0xa6
	{ "AND A", 0, and_a},                       // 0xa7
	{ "XOR B", 0, xor_b},                       // 0xa8
	{ "XOR C", 0, xor_c},                       // 0xa9
	{ "XOR D", 0, xor_d},                       // 0xaa
	{ "XOR E", 0, xor_e},                       // 0xab
	{ "XOR H", 0, xor_h},                       // 0xac
	{ "XOR L", 0, xor_l},                       // 0xad
	{ "XOR (HL)", 0},                  // 0xae
	{ "XOR A", 0, xor_a},                       // 0xaf
	{ "OR B", 0, or_b},                         // 0xb0
	{ "OR C", 0, or_c},                         // 0xb1
	{ "OR D", 0, or_d},                         // 0xb2
	{ "OR E", 0, or_e},                         // 0xb3
	{ "OR H", 0, or_h},                         // 0xb4
	{ "OR L", 0, or_l},                         // 0xb5
	{ "OR (HL)", 0},                    // 0xb6
	{ "OR A", 0, or_a},                         // 0xb7
	{ "CP B", 0},                         // 0xb8
	{ "CP C", 0},                         // 0xb9
	{ "CP D", 0},                         // 0xba
	{ "CP E", 0},                         // 0xbb
	{ "CP H", 0},                         // 0xbc
	{ "CP L", 0},                         // 0xbd
	{ "CP (HL)", 0},                    // 0xbe
	{ "CP A", 0},                         // 0xbf
	{ "RET NZ", 0},                     // 0xc0
	{ "POP BC", 0},                     // 0xc1
	{ "JP NZ, 0x%04X", 2},            // 0xc2
	{ "JP 0x%04X", 2},                   // 0xc3
	{ "CALL NZ, 0x%04X", 2},        // 0xc4
	{ "PUSH BC", 0},                   // 0xc5
	{ "ADD A, 0x%02X", 1},             // 0xc6
	{ "RST 0x00", 0},                    // 0xc7
	{ "RET Z", 0},                       // 0xc8
	{ "RET", 0},                           // 0xc9
	{ "JP Z, 0x%04X", 2},              // 0xca
	{ "CB %02X", 1},                      // 0xcb
	{ "CALL Z, 0x%04X", 2},          // 0xcc
	{ "CALL 0x%04X", 2},               // 0xcd
	{ "ADC 0x%02X", 1},                  // 0xce
	{ "RST 0x08", 0},                   // 0xcf
	{ "RET NC", 0},                     // 0xd0
	{ "POP DE", 0},                     // 0xd1
	{ "JP NC, 0x%04X", 2},            // 0xd2
	{ "UNKNOWN", 0},                 // 0xd3
	{ "CALL NC, 0x%04X", 2},        // 0xd4
	{ "PUSH DE", 0},                   // 0xd5
	{ "SUB 0x%02X", 1},                  // 0xd6
	{ "RST 0x10", 0},                   // 0xd7
	{ "RET C", 0},                       // 0xd8
	{ "RETI", 0},          // 0xd9
	{ "JP C, 0x%04X", 2},              // 0xda
	{ "UNKNOWN", 0},                 // 0xdb
	{ "CALL C, 0x%04X", 2},          // 0xdc
	{ "UNKNOWN", 0},                 // 0xdd
	{ "SBC 0x%02X", 1},                  // 0xde
	{ "RST 0x18", 0},                   // 0xdf
	{ "LD (0xFF00 + 0x%02X), A", 1},// 0xe0
	{ "POP HL", 0},                     // 0xe1
	{ "LD (0xFF00 + C), A", 0},      // 0xe2
	{ "UNKNOWN", 0},                 // 0xe3
	{ "UNKNOWN", 0},                 // 0xe4
	{ "PUSH HL", 0},                   // 0xe5
	{ "AND 0x%02X", 1},                  // 0xe6
	{ "RST 0x20", 0},                   // 0xe7
	{ "ADD SP,0x%02X", 1},            // 0xe8
	{ "JP HL", 0},                       // 0xe9
	{ "LD (0x%04X), A", 2},           // 0xea
	{ "UNKNOWN", 0},                 // 0xeb
	{ "UNKNOWN", 0},                 // 0xec
	{ "UNKNOWN", 0},                 // 0xed
	{ "XOR 0x%02X", 1},                  // 0xee
	{ "RST 0x28", 0},                   // 0xef
	{ "LD A, (0xFF00 + 0x%02X)", 1},// 0xf0
	{ "POP AF", 0},                     // 0xf1
	{ "LD A, (0xFF00 + C)", 0},      // 0xf2
	{ "DI", 0},                        // 0xf3
	{ "UNKNOWN", 0},                 // 0xf4
	{ "PUSH AF", 0},                   // 0xf5
	{ "OR 0x%02X", 1},                    // 0xf6
	{ "RST 0x30", 0},                   // 0xf7
	{ "LD HL, SP+0x%02X", 1},       // 0xf8
	{ "LD SP, HL", 0},                // 0xf9
	{ "LD A, (0x%04X)", 2},           // 0xfa
	{ "EI", 0},                             // 0xfb
	{ "UNKNOWN", 0},                 // 0xfc
	{ "UNKNOWN", 0},                 // 0xfd
	{ "CP 0x%02X", 1},                    // 0xfe
	{ "RST 0x38", 0},
};


char cpuOperandSize(uint8_t opcode) {
    return instructions[opcode].operandSize;
}

void cpuPrintInstruction(uint8_t opcode, char operand) {
    fprintf(stdout, instructions[opcode].code, operand);
    fprintf(stdout, "\n");
}

#define DEFINE_FUNC(ret, reg1, reg2) \
    ret ld_##reg1##_##reg2 () {\
        registers.reg1 = registers.reg2;\
    }
REGISTERS_COMBINATION
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1, reg2) \
    ret add_##reg1##_##reg2 () {\
        registers.reg1 += registers.reg2;\
    }
A_COMBINATION
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret inc_##reg1 () {\
        registers.reg1 += 1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret dec_##reg1 () {\
        registers.reg1 -= 1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

#define DEFINE_FUNC(ret, reg1) \
    ret and_##reg1 () {\
        registers.a &= registers.reg1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

/**** OR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret or_##reg1 () {\
        registers.a |= registers.reg1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC

/**** XOR functions definitions ****/
#define DEFINE_FUNC(ret, reg1) \
    ret xor_##reg1 () {\
        registers.a ^= registers.reg1;\
    }

REGISTERS_LIST
#undef DEFINE_FUNC
