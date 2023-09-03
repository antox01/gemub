# Gemub
Another Gameboy emulator in C.

## Roadmap
- [ ] CPU
    - [X] Instruction decoding
    - [X] Registers implementation
    - [ ] ISA implementation
        - [X] LD
        - [X] ADD, ADC, SUB, SBC, INC, DEC
        - [X] AND, OR, XOR
        - [X] JP, JR, CALL
        - [X] PUSH, POP, RST
        - [ ] RET
        - [ ] CB 0x%04x (instructions with CB opcode)
        - [ ] Interrupt based instructions
    - [ ] Enabling simulation
- [ ] Memory
    - [X] Load rom from file
    - [X] Implementing read operation on ROM
    - [ ] Managing MBC
    - [X] Implementing basic memory functionality (read from/write to memory)
- [ ] Output video

## Resources
- [Let's write a game boy emulator in python](https://www.inspiredpython.com/course/game-boy-emulator/let-s-write-a-game-boy-emulator-in-python)
- [Gameboy development website](https://gbdev.io/)
- [Gameboy CPU Manual](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf)
- [To understand DAA](https://forums.nesdev.org/viewtopic.php?t=15944)
