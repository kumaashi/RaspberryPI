#include "diag.h"

/*
3322 2222 2222 1111 1111 1100 0000 0000
1098 7654 3210 9876 5432 1098 7654 3210
*/
void dispDiag(uint32_t offset)
{
	draw_dword(IO_READ(V3D_CT0CS),       0,  1 + 1); //10
	draw_dword(IO_READ(V3D_CT1CS),       0,  2 + 1); //118
	draw_dword(IO_READ(V3D_ERRSTAT),     0,  3 + 1); //1000
	
	draw_dword(IO_READ(V3D_BFC),         0,  5 + 1); //1
	draw_dword(IO_READ(V3D_RFC),         0,  6 + 1); //0
	draw_dword(IO_READ(V3D_PCS),         0,  7 + 1); //4
	draw_dword(IO_READ(V3D_FDBGO),       0,  8 + 1); //0
	draw_dword(IO_READ(V3D_INTCTL),      0,  9 + 1); //6
	
	draw_dword(IO_READ(V3D_VPACNTL),     0, 10 + 1);
	draw_dword(IO_READ(V3D_BPOS),        0, 11 + 1);
	draw_dword(IO_READ(V3D_BPOA),        0, 12 + 1);
	draw_dword(IO_READ(V3D_BPCS),        0, 13 + 1);
	draw_dword(IO_READ(V3D_IDENT2),      0, 14 + 1); //0x121
	draw_dword(IO_READ(UART0_DR),        0, 15 + 1); //0x121
	draw_dword(IO_READ(UART0_FR),        0, 16 + 1); //0x121
	for(int i = 0 ; i < 24; i++) {
		draw_dword(IO_READ(offset + i * 4), 27, i);
	}
}

void uartDiag(uint32_t offset)
{
	uart_puts("------------------------------------------------------\n");
	uart_debug_puts("IO_READ(V3D_CT0CS)    =\n", IO_READ(V3D_CT0CS)   );
	uart_debug_puts("IO_READ(V3D_CT1CS)    =\n", IO_READ(V3D_CT1CS)   );
	uart_debug_puts("IO_READ(V3D_ERRSTAT)  =\n", IO_READ(V3D_ERRSTAT) );
	uart_debug_puts("IO_READ(V3D_BFC)      =\n", IO_READ(V3D_BFC)     );
	uart_debug_puts("IO_READ(V3D_RFC)      =\n", IO_READ(V3D_RFC)     );
	uart_debug_puts("IO_READ(V3D_PCS)      =\n", IO_READ(V3D_PCS)     );
	uart_debug_puts("IO_READ(V3D_FDBGO)    =\n", IO_READ(V3D_FDBGO)   );
	uart_debug_puts("IO_READ(V3D_INTCTL)   =\n", IO_READ(V3D_INTCTL)  );
	uart_debug_puts("IO_READ(V3D_VPACNTL)  =\n", IO_READ(V3D_VPACNTL) );
	uart_debug_puts("IO_READ(V3D_BPOS)     =\n", IO_READ(V3D_BPOS)    );
	uart_debug_puts("IO_READ(V3D_BPOA)     =\n", IO_READ(V3D_BPOA)    );
	uart_debug_puts("IO_READ(V3D_BPCS)     =\n", IO_READ(V3D_BPCS)    );
	uart_debug_puts("IO_READ(V3D_IDENT2)   =\n", IO_READ(V3D_IDENT2)  );
	uart_puts("------------------------------------------------------\n");
	/*
	for(int i = 0 ; i < 24; i++) {
		draw_dword(IO_READ(offset + i * 4), 27, i);
	}
	*/
}
