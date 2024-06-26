/*
 * MIT License
 *
 * Copyright (c) 2023 Dozingfiretruck
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "nes.h"

/* https://www.nesdev.org/wiki/UxROM */

static void nes_mapper_init(nes_t* nes){
    const int mirror = nes->nes_rom.prg_rom_size * 0x02;
    // CPU $8000-$BFFF: 16 KB switchable PRG ROM bank
    nes_load_prgrom_8k(nes,0, 0);
    nes_load_prgrom_8k(nes,1, 1);
    // CPU $C000-$FFFF: 16 KB PRG ROM bank, fixed to the last bank
    nes_load_prgrom_8k(nes,2, mirror-2);
    nes_load_prgrom_8k(nes,3, mirror-1);
    // CHR capacity: 8 KiB ROM
    for (int i = 0; i < 8; i++){
        nes_load_chrrom_1k(nes,i,i);
    }
}
/*
    7  bit  0
    ---- ----
    xxxx pPPP
         ||||
         ++++- Select 16 KB PRG ROM bank for CPU $8000-$BFFF
                (UNROM uses bits 2-0; UOROM uses bits 3-0)
*/
static void nes_mapper_write(nes_t* nes, uint16_t address, uint8_t date) {
    const int bank = (date % nes->nes_rom.prg_rom_size) * 0x02;
    nes_load_prgrom_8k(nes, 0, bank);
    nes_load_prgrom_8k(nes, 1, bank + 1);
}



int nes_mapper2_init(nes_t* nes){
    nes->nes_mapper.mapper_init = nes_mapper_init;
    nes->nes_mapper.mapper_write = nes_mapper_write;
    return 0;
}

