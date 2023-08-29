#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

#include "../includes/cpu.h"

/**** Macros section ****/
#define ASSERT(cond, ...) if(!(cond)) { fprintf(stderr, __VA_ARGS__); exit(1); }

/**** Constants section ****/
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

void readInstruction(FILE* fin);

int main(void) {
    ASSERT(!SDL_Init(SDL_INIT_VIDEO), "SDL failed to initialize %s\n", SDL_GetError());
    
    SDL_Window *window = SDL_CreateWindow("gemub", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    ASSERT(window, "SDL failed to create window %s\n", SDL_GetError());

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    int8_t quit = 0;
    FILE* fin = fopen("./roms/Tetris.gb", "rb");
    ASSERT(fin != NULL, "Error opening rom\n");

    while(!quit) {
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
            }
        }
        readInstruction(fin);
        quit = 1;
    }

    fclose(fin);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void readInstruction(FILE* fin) {
    uint8_t opcode;
    uint16_t operand, operandSize = 0;
    fread(&opcode, 1, 1, fin);
    operandSize = cpuOperandSize(opcode);
    if(operandSize > 0) fread(&operand, operandSize, 1, fin);
    cpuPrintInstruction(opcode, operand);
}
