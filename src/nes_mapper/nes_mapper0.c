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

/* https://www.nesdev.org/wiki/NROM */

static void nes_mapper_init(nes_t* nes){
    // $6000-$7FFF: Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB window, write protectable with an external switch

    // PRG-ROM 16k or 32k, set mirror
    const int mirror = nes->nes_rom.prg_rom_size & 0x02;
    // CPU $8000-$BFFF: First 16 KB of ROM.
    nes_load_prgrom_8k(nes,0, 0);
    nes_load_prgrom_8k(nes,1, 1);
    // CPU $C000-$FFFF: Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
    nes_load_prgrom_8k(nes,2, mirror+0);
    nes_load_prgrom_8k(nes,3, mirror+1);
    // CHR capacity: 8 KiB ROM (DIP-28 standard pinout) but most emulators support RAM
    for (int i = 0; i < 8; i++){
        nes_load_chrrom_1k(nes,i,i);
    }
}

int nes_mapper0_init(nes_t* nes){
    nes->nes_mapper.mapper_init = nes_mapper_init;
    return 0;
}

