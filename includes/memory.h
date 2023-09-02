#ifndef MEMORY_DEF_H
#define MEMORY_DEF_H

#include <stdint.h>
#include <stdio.h>

#define VIDEO_RAM_OFFSET 0x8000
#define EXTERNAL_RAM_OFFSET 0xa000
#define WORK_RAM_OFFSET 0xc000
#define ECHO_RAM_OFFSET 0xe000

typedef struct memory_s *memory_t;

memory_t initMemory(FILE *fin);
void freeMemory(memory_t memory);
uint8_t memoryReadByte(memory_t memory, uint16_t address);
uint16_t memoryReadWord(memory_t memory, uint16_t address);
void memoryWriteByte(memory_t memory, uint16_t address, uint8_t value);
void memoryWriteWord(memory_t memory, uint16_t address, uint16_t value);
#endif
