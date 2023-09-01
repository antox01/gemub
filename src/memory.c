#include <stdlib.h>

#include "../includes/memory.h"
#include "../includes/rom.h"

#define DEFAULT_SIZE 1024


struct memory_s {
    rom_t rom;
    uint8_t *externalRam;
    uint8_t *workRam;
};

memory_t initMemory(FILE *fin) {
    uint32_t ramSize;
    memory_t memory = (memory_t) malloc(sizeof(*memory));
    if (memory == NULL) {
        fprintf(stderr, "Could not initialize memory\n");
        return NULL;
    }
    memory->rom = loadRom(fin);
    ramSize = romGetByte(memory->rom, 0x149);
    if(ramSize > 0) {
        memory->externalRam = malloc(ramSize * (1 << ramSize) * sizeof(uint8_t));
    } else {
        memory->externalRam = NULL;
    }
    memory->workRam = malloc((ECHO_RAM_OFFSET - WORK_RAM_OFFSET) * sizeof(uint8_t));

    return memory;
}

void freeMemory(memory_t memory) {
    if(memory == NULL)
        return;
    freeRom(memory->rom);
    if(memory->externalRam != NULL)
        free(memory->externalRam);
    if(memory->workRam != NULL)
        free(memory->workRam);
    free(memory);
}

uint8_t memoryReadByte(memory_t memory, uint16_t address) {
    if (address >= ECHO_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return 0x0;
    }
    if (address >= WORK_RAM_OFFSET) {
        return memory->workRam[address - WORK_RAM_OFFSET];
    }
    if (address >= EXTERNAL_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return 0x0;
    }
    if (address >= VIDEO_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return 0x0;
    }
    return romGetByte(memory->rom, address);
}

void memoryWriteByte(memory_t memory, uint16_t address, uint8_t value) {
    if (address >= ECHO_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return;
    }
    if (address >= WORK_RAM_OFFSET) {
        memory->workRam[address - WORK_RAM_OFFSET] = value;
        return;
    }
    if (address >= EXTERNAL_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return;
    }
    if (address >= VIDEO_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return;
    }
}
