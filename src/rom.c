#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/rom.h"

#define DEFAULT_SIZE 32*1024
#define HEADER_OFFSET 0x100
#define BANK1_OFFSET 0x4000
#define BANK0_OFFSET (HEADER_OFFSET+sizeof(romHeader_t))
#define ROM_LIMIT 0x8000

typedef struct {
    uint8_t entryPoint[0x4];
    uint8_t nintendoLogo[0x30];
    union {
        struct {
            uint8_t titleNew[11];
            uint8_t manufacturerCode[4];
            uint8_t cgbFlag;
        };
        uint8_t titleOld[16];
    };
    uint8_t newLicenceCode[2];
    uint8_t sgbFlag;
    uint8_t cartridgeType;
    uint8_t romSize; // The size is equal to 32KiB * (1<< <value>)
    uint8_t ramSize;
    uint8_t destinationCode;
    uint8_t OldLicenseCode; // 0x33 to use the new instead
    uint8_t maskRomVersionNumber;
    uint8_t headerChecksum;
    uint8_t globalChecksum[2];
} romHeader_t ;


struct rom_s {
    uint8_t reservedMemory[0x100];
    romHeader_t header;
    uint8_t *content;

    uint32_t contentSize;
};

rom_t loadRom(FILE* fin) {
    rom_t rom = (rom_t) malloc(sizeof(*rom));
    uint32_t counter;

    fread(rom->reservedMemory, sizeof(uint8_t), HEADER_OFFSET, fin);
    fread(&(rom->header), sizeof(romHeader_t), 1, fin);

    rom->contentSize = DEFAULT_SIZE * (1 << rom->header.romSize) - BANK0_OFFSET;

    rom->content = (uint8_t *) malloc(rom->contentSize * sizeof(uint8_t));
    if(rom->content == NULL) {
        fprintf(stdout, "Could not allocate memory\n");
        freeRom(rom);
        return NULL;
    }
    //fread(&rom->content[0], sizeof(uint8_t), rom->contentSize, fin);
    for(counter = 0; counter < rom->contentSize; counter++) {
        uint8_t tmp;
        fread(&tmp, sizeof(uint8_t), 1, fin);
        //fprintf(stdout, "%02x", tmp);
        rom->content[counter] = tmp;
    }

    return rom;
}

void freeRom(rom_t rom) {
    if(rom == NULL)
        return;

    if(rom->content != NULL)
        free(rom->content);
    free(rom);
}

uint8_t romReadByte(rom_t rom, uint16_t address) {
    if(address >= BANK1_OFFSET) {
        //fprintf(stderr, "%04x\n", address);
        return rom->content[address - BANK0_OFFSET];
    }
    if(address >= BANK0_OFFSET) {
        return rom->content[address - BANK0_OFFSET];
    }
    if(address >= HEADER_OFFSET) {
        uint8_t *headerMap = (uint8_t *)&(rom->header);
        return headerMap[address - HEADER_OFFSET];
    }
    return 0x0;
}

uint16_t romReadWord(rom_t rom, uint16_t address) {
    if(address >= BANK1_OFFSET) {
        return rom->content[address - BANK1_OFFSET];
    }
    if(address >= BANK0_OFFSET) {
        return *((uint16_t *)&rom->content[address - BANK0_OFFSET]);
    }
    if(address >= HEADER_OFFSET) {
        uint8_t *headerMap = (uint8_t *)&(rom->header);
        return *((uint16_t*)&headerMap[address - HEADER_OFFSET]);
    }
    return 0x0;
}

void romDump(rom_t rom) {
    uint8_t *preContent = rom->reservedMemory;
    uint32_t counter;
    for(counter = 0; counter < BANK0_OFFSET; counter++) {
        if((counter & 0x0f) == 0) fprintf(stdout, "\n%04x: ", counter);
        fprintf(stdout, "%02x", preContent[counter]);
        if(counter & 0x01) fprintf(stdout, " ");
    }

    //fprintf(stderr, "%04x\n", rom->contentSize);
    for(counter = 0; counter < rom->contentSize; counter++) {
        if(((counter + BANK0_OFFSET) & 0x0f) == 0)
            fprintf(stdout, "\n%04lx: ", counter + BANK0_OFFSET);
        fprintf(stdout, "%02x", rom->content[counter]);
        if((counter + BANK0_OFFSET) & 0x01) fprintf(stdout, " ");
    }
}

uint8_t romHasAddress(rom_t rom, uint16_t address) {
    if(address >= ROM_LIMIT) {
        return 0;
    }
    return 1;
}
