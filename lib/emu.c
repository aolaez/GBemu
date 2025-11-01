#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*
    Emu Components:

    \Cart| - Load cartridge and read/write data to/from
    |CPU| - Emulating instructions and registers
    |Address Bus| - For reading and writing to registers
    |PPU| - Pixel Processing Unit, draws everything
    |Timer| - For timing everything
*/

static emu_context ctx;

emu_context *emu_get_context() {
    return &ctx;
}

void delay(u32 ms){
    SDL_Delay(ms);
}

int emu_run(int argc, char **argv){
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }
    if (!cart_load(argv[1])){ // if no cartridge argument passed
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart loaded...\n");

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init(); // fonts
    printf("TTF INIT\n");

    cpu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while(ctx.running){
        if (ctx.paused){
            delay(10);
            continue;
        }

        if (!cpu_step()){ // do one cpu step and if it fails, exit
            printf("CPU Stopped\n");
            return -3;
        }

        ctx.ticks++; // if CPU step doesnt fail increment ticks
    }

    return 0;
}

void emu_cycles(int cpu_cycles){
    //TODO
}
