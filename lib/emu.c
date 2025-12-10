#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <dma.h>
#include <ui.h>
#include <ppu.h>
#include <pthread.h>
#include <unistd.h>
#include <timer.h>

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

void *cpu_run(void *p){
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
            return 0;
        }
    }
    return 0;
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
    ui_init();

    pthread_t t1;

    if (pthread_create(&t1, NULL, cpu_run, NULL)){
        fprintf(stderr, "FAILED TO START CPU MAIN THREAD");
        return -1;
    }

    u32 prev_frame = 0;

    while (!ctx.die) {
        usleep(1000);
        ui_handle_events();

        if (prev_frame != ppu_get_context()->current_frame) {
            ui_update();
        }
        prev_frame = ppu_get_context()->current_frame;
    }

    return 0;
}

void emu_cycles(int cpu_cycles){
    for (int i = 0; i < cpu_cycles; i++) {
        for (int n = 0; n < 4; n++) {
            ctx.ticks++;
            timer_tick();
            ppu_tick();
        }

        dma_tick();
    }
}
