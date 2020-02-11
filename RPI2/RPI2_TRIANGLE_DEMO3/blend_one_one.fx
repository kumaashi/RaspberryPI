/* Exported Symbols */
#define qpu_symbol_entry V3DWrite32(ctx,0x00000000

/* Assembled Program */
/* entry: */
/* V3DWrite32(ctx,0x00000000: */ V3DWrite32(ctx,0x958efdbf); V3DWrite32(ctx,0xd1724823); /* mov r0); vary; mov r3.8d); 0.5 */
/* V3DWrite32(ctx,0x00000008: */ V3DWrite32(ctx,0x818e7176); V3DWrite32(ctx,0x40024821); /* fadd r0); r0); r5; mov r1); vary; sbwait */
/* V3DWrite32(ctx,0x00000010: */ V3DWrite32(ctx,0x818e7376); V3DWrite32(ctx,0x10024862); /* fadd r1); r1); r5; mov r2); vary */
/* V3DWrite32(ctx,0x00000018: */ V3DWrite32(ctx,0x819e7540); V3DWrite32(ctx,0x114248a3); /* fadd r2); r2); r5; mov r3.8a); r0 */
/* V3DWrite32(ctx,0x00000020: */ V3DWrite32(ctx,0x809e7009); V3DWrite32(ctx,0x115049e3); /* nop; mov r3.8b); r1 */
/* V3DWrite32(ctx,0x00000028: */ V3DWrite32(ctx,0x809e7012); V3DWrite32(ctx,0x116049e3); /* nop; mov r3.8c); r2 */
/* V3DWrite32(ctx,0x00000030: */ V3DWrite32(ctx,0x009e7000); V3DWrite32(ctx,0x800009e7); /* nop; nop; loadc */
/* V3DWrite32(ctx,0x00000038: */ V3DWrite32(ctx,0x1e9e7700); V3DWrite32(ctx,0x100208e7); /* v8adds r3); r3); r4; nop */
/* V3DWrite32(ctx,0x00000040: */ V3DWrite32(ctx,0x159e76c0); V3DWrite32(ctx,0x30020ba7); /* mov tlbc); r3; nop; thrend */
/* V3DWrite32(ctx,0x00000048: */ V3DWrite32(ctx,0x009e7000); V3DWrite32(ctx,0x100009e7); /* nop */
/* V3DWrite32(ctx,0x00000050: */ V3DWrite32(ctx,0x009e7000); V3DWrite32(ctx,0x500009e7); /* nop; nop; sbdone */
