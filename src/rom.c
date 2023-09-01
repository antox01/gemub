#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/rom.h"

#define DEFAULT_SIZE 32*1024
#define HEADER_OFFSET 0x100
#define BANK1_OFFSET 0x4000
#define BANK0_OFFSET HEADER_OFFSET+sizeof(romHeader_t)

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
    uint8_t globalChecksum;
} romHeader_t ;


struct rom_s {
    uint8_t reservedMemory[0x100];
    romHeader_t header;
    uint8_t *content;
    uint8_t *bank0;
    uint8_t *bank1;
};

rom_t loadRom(FILE* fin) {
    uint32_t contentSize;
    rom_t rom = (rom_t) malloc(sizeof(*rom));

    fread(&(rom->reservedMemory), sizeof(uint8_t), HEADER_OFFSET, fin);
    fread(&(rom->header), sizeof(romHeader_t), 1, fin);
    contentSize = DEFAULT_SIZE * (1 << rom->header.romSize) - HEADER_OFFSET - sizeof(romHeader_t);

    rom->content = (uint8_t *) malloc(contentSize * sizeof(uint8_t));
    fread(rom->content, contentSize, 1, fin);

    rom->bank0 = rom->content;
    rom->bank1 = rom->content + BANK1_OFFSET;

    return rom;
}

void freeRom(rom_t rom) {
    if(rom == NULL)
        return;

    if(rom->content != NULL)
        free(rom->content);
    free(rom);
}

uint8_t romGetByte(rom_t rom, uint16_t address) {
    if(address >= BANK1_OFFSET) {
        return rom->bank1[address - BANK1_OFFSET];
    }
    if(address >= BANK0_OFFSET) {
        return rom->bank0[address - BANK0_OFFSET];
    }
    if(address >= HEADER_OFFSET) {
        uint8_t *headerMap = (uint8_t *)&(rom->header);
        return headerMap[address - HEADER_OFFSET];
    }
    return 0x0;
}

