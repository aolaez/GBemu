#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = {0};

void cpu_init(){
    ctx.regs.pc = 0x100;
}

static void fetch_instruction() {
    ctx.curr_opcode = bus_read(ctx.regs.pc++); // read current opcode from program counter and increment program counter
    ctx.curr_instr = instruction_by_opcode(ctx.curr_opcode);
}

static void fetch_data() {
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

        // read from program counter into 8 bit register
        case AM_R_D8:
            ctx.fetch_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        // reading 2 byte instruction so 2 reads
        case AM_D16: { // could be bug source
            u16 lo = bus_read(ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.pc + 1);
            emu_cycles(1);

            ctx.fetch_data = lo | (hi << 8);
            ctx.regs.pc += 2;
            return;
        }

        default:
            printf("Unknown Addressing Mode! %d\n", ctx.curr_instr->mode);
            exit(-7);
            return;
    }
}

static void execute() {
    IN_PROC proc = inst_get_processor(ctx.curr_instr->type);

    if (!proc){
        NOT_IMPL
    }

    proc(&ctx);
}

bool cpu_step() {
    if(!ctx.halted){
        u16 pc = ctx.regs.pc;
        fetch_instruction();
        fetch_data();

        printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n", 
            pc, inst_name(ctx.curr_instr->type), ctx.curr_opcode,
            bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, ctx.regs.b, ctx.regs.c);

        execute();
    }

    if (ctx.curr_instr == NULL) {
        printf("Unknown Instruction: %02X\n", ctx.curr_opcode);
        exit(-7);
    }
    return true;
}
