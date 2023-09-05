#ifndef MEMORY_DEF_H
#define MEMORY_DEF_H

#include <stdint.h>
#include <stdio.h>

#define VIDEO_RAM_OFFSET 0x8000
#define EXTERNAL_RAM_OFFSET 0xa000
#define WORK_RAM_OFFSET 0xc000
#define ECHO_RAM_OFFSET 0xe000
#define OAM_OFFSET 0xfe00
#define UNUSABLE_OFFSET 0xfea0
#define IO_OFFSET 0xff00
#define HIGH_RAM_OFFSET 0xff80
#define INTERRUPT_OFFSET 0xffff

typedef struct memory_s *memory_t;

memory_t initMemory(FILE *fin);
void freeMemory(memory_t memory);
uint8_t memoryReadByte(memory_t memory, uint16_t address);
uint16_t memoryReadWord(memory_t memory, uint16_t address);
void memoryWriteByte(memory_t memory, uint16_t address, uint8_t value);
void memoryWriteWord(memory_t memory, uint16_t address, uint16_t value);
uint8_t memoryHasAddress(memory_t memory, uint16_t address);
void memoryDumpRom(memory_t memory);

#endif
