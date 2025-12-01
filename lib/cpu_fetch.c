#include <cpu.h>
#include <bus.h>
#include <emu.h>

extern cpu_context ctx;

void fetch_data() {
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    if (ctx.curr_instr == NULL) {
        return;
    }

    switch(ctx.curr_instr->mode) {
        case AM_IMP: return;

        // if register mode then we want to read from register
        case AM_R:
            ctx.fetch_data = cpu_read_reg(ctx.curr_instr->reg_1);
            return;
        
        case AM_R_R:
            ctx.fetch_data = cpu_read_reg(ctx.curr_instr->reg_2);
            return;

        // read from program counter into 8 bit register
        case AM_R_D8:
            ctx.fetch_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        // reading 2 byte instruction so 2 reads
        case AM_R_D16: // same as below
        case AM_D16: { // could be bug source
            u16 lo = bus_read(ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.pc + 1);
            emu_cycles(1);

            ctx.fetch_data = lo | (hi << 8);
            ctx.regs.pc += 2;
            return;
        }

        case AM_MR_R: // loading register into a memory region
            ctx.fetch_data = cpu_read_reg(ctx.curr_instr->reg_2);
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.dest_is_mem = true;

            if (ctx.curr_instr->reg_1 == RT_C){ // special case
                ctx.mem_dest |= 0xFF00;
            }

            return;

        case AM_R_MR: {// reading from memroy into reg
            u16 addr = cpu_read_reg(ctx.curr_instr->reg_2);

            if (ctx.curr_instr->reg_1 == RT_C){
                addr |= 0xFF00;
            }

            ctx.fetch_data = bus_read(addr);
            emu_cycles(1);

        } return;

        case AM_R_HLI: // read from HL reg then increment it
            ctx.fetch_data = bus_read(cpu_read_reg(ctx.curr_instr->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_R_HLD: // read from HL reg then decrement it
            ctx.fetch_data = bus_read(cpu_read_reg(ctx.curr_instr->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_HLI_R:
            ctx.fetch_data = cpu_read_reg(ctx.curr_instr->reg_2);
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_HLD_R:
            ctx.fetch_data = cpu_read_reg(ctx.curr_instr->reg_2);
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_R_A8: // read from pc and incrememnt
            ctx.fetch_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        case AM_A8_R:
            ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00;
            ctx.dest_is_mem = true;
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        case AM_HL_SPR:
            ctx.fetch_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;
            
        case AM_D8:
            ctx.fetch_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        case AM_A16_R:
        case AM_D16_R: {
            u16 lo = bus_read(ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.pc + 1);
            emu_cycles(1);

            ctx.mem_dest = lo | (hi << 8);
            ctx.dest_is_mem = true;

            ctx.regs.pc += 2;
            ctx.fetch_data = cpu_read_reg(ctx.curr_instr->reg_2);
        } return;

        case AM_MR_D8:
            ctx.fetch_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.dest_is_mem = true;
            return;

        case AM_MR:
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.dest_is_mem = true;
            ctx.fetch_data = bus_read(cpu_read_reg(ctx.curr_instr->reg_1));
            emu_cycles(1);
            return;


        case AM_R_A16: {
            u16 lo = bus_read(ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.pc + 1);
            emu_cycles(1);

            u16 addr = lo | (hi << 8);

            ctx.regs.pc += 2;
            ctx.fetch_data = bus_read(addr);
            emu_cycles(1);

            return;
        } 

        default:
            printf("Unknown Addressing Mode! %d\n", ctx.curr_instr->mode);
            exit(-7);
            return;
    }
}