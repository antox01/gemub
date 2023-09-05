#include <stdlib.h>
#include <string.h>

#include "../includes/memory.h"
#include "../includes/rom.h"

#define DEFAULT_SIZE 1024


struct memory_s {
    rom_t rom;
    uint8_t *externalRam;
    uint8_t *workRam;
    uint8_t *oam;
    uint8_t *highRam;

    uint8_t ime;
};

memory_t initMemory(FILE *fin) {
    uint32_t ramSize;
    memory_t memory = (memory_t) malloc(sizeof(*memory));
    if (memory == NULL) {
        fprintf(stderr, "Could not initialize memory\n");
        return NULL;
    }
    memory->rom = loadRom(fin);
    ramSize = romReadByte(memory->rom, 0x149);
    if(ramSize > 0) {
        memory->externalRam = malloc(ramSize * (1 << ramSize) * sizeof(uint8_t));
    } else {
        memory->externalRam = NULL;
    }
    memory->workRam = malloc((ECHO_RAM_OFFSET - WORK_RAM_OFFSET) * sizeof(uint8_t));
    memory->oam = malloc((UNUSABLE_OFFSET - OAM_OFFSET) * sizeof(uint8_t));
    memory->highRam = malloc((INTERRUPT_OFFSET - HIGH_RAM_OFFSET) * sizeof(uint8_t));

    return memory;
}

void freeMemory(memory_t memory) {
    if(memory == NULL)
        return;
    freeRom(memory->rom);
    if(memory->oam != NULL)
        free(memory->oam);
    if(memory->externalRam != NULL)
        free(memory->externalRam);
    if(memory->workRam != NULL)
        free(memory->workRam);
    free(memory);
}

uint8_t memoryReadByte(memory_t memory, uint16_t address) {
    if (address == INTERRUPT_OFFSET) {
        //fprintf(stderr, "Interrupt: Not implemented\n");
        return memory->ime;
    }
    if (address >= HIGH_RAM_OFFSET) {
        return memory->highRam[address - HIGH_RAM_OFFSET];
    }
    if (address >= UNUSABLE_OFFSET) {
        fprintf(stderr, "Forbidden offset\n");
        return 0x0;
    }
    if (address >= OAM_OFFSET) {
        return memory->oam[address - OAM_OFFSET];
    }
    if (address >= ECHO_RAM_OFFSET) {
        //fprintf(stderr, "Echo ram: Not implemented\n");
        return memory->workRam[address - ECHO_RAM_OFFSET];
    }
    if (address >= WORK_RAM_OFFSET) {
        return memory->workRam[address - WORK_RAM_OFFSET];
    }
    if (address >= EXTERNAL_RAM_OFFSET) {
        fprintf(stderr, "External Ram: Not implemented\n");
        return 0x0;
    }
    if (address >= VIDEO_RAM_OFFSET) {
        fprintf(stderr, "Video Ram: Not implemented\n");
        return 0x0;
    }
    return romReadByte(memory->rom, address);
}

uint16_t memoryReadWord(memory_t memory, uint16_t address) {
    return memoryReadByte(memory, address) | (memoryReadByte(memory, address + 1) << 8);
}

void memoryWriteByte(memory_t memory, uint16_t address, uint8_t value) {
    if (address == INTERRUPT_OFFSET) {
        fprintf(stderr, "Interrupt: Not implemented\n");
        memory->ime = value;
        return;
    }
    if (address >= ECHO_RAM_OFFSET) {
        //fprintf(stderr, "Not implemented\n");
        memory->workRam[address - ECHO_RAM_OFFSET] = value;
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

void memoryWriteWord(memory_t memory, uint16_t address, uint16_t value) {
    memoryWriteByte(memory, address, value);
    memoryWriteByte(memory, address + 1, (value & 0xff00) >> 8);
}

uint8_t memoryHasAddress(memory_t memory, uint16_t address) {
    if (address >= ECHO_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return 1;
    }
    if (address >= WORK_RAM_OFFSET) {
        return 1;
    }
    if (address >= EXTERNAL_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return 0;
    }
    if (address >= VIDEO_RAM_OFFSET) {
        fprintf(stderr, "Not implemented\n");
        return 0;
    }
    
    return 1;
}

void memoryDumpRom(memory_t memory) {
    romDump(memory->rom);
}
