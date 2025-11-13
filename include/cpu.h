#pragma once

#include <common.h>
#include <instructions.h>

/* CPU REGISTERS
16-bit	Hi	Lo	Name/Function
AF	    A	-	Accumulator & Flags
BC	    B	C	BC
DE	    D	E	DE
HL	    H	L	HL
SP	    -	-	Stack Pointer
PC	    -	-	Program Counter/Pointer
*/

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
} cpu_registers;

typedef struct {
    cpu_registers regs;
    // curr fetch
    u16 fetch_data;
    u16 mem_dest;
    bool dest_is_mem;
    u8 curr_opcode;
    instruction *curr_instr;

    bool halted;
    bool stepping;
} cpu_context;

void cpu_init();
bool cpu_step();

u16 cpu_read_reg(reg_type rt);

typedef void (*IN_PROC)(cpu_context *);
IN_PROC inst_get_processor(in_type type);

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 7)
