#include "util.h"
#include "sintable.inl"

static int CHAR_COLOR = 0xFFFFFFFF;

uint8_t char_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


void set_char_color(uint32_t col) {
	CHAR_COLOR = col;
}

void reset_char_color() {
	CHAR_COLOR = 0xFFFFFFFF;
}

void draw_char(int c, int x, int y) {
	if(c < 0 || c > 0x100) return;
	x = x % 64;
	y = y % 64;
	const uint8_t *data = get_fontdata16x16();
	int start_x = x * FONT_SIZE;
	int start_y = y * FONT_SIZE;
	int end_x   = start_x + FONT_SIZE;
	int end_y   = start_y + FONT_SIZE;
	int index   = 0;
	int X       = c % FONT_SIZE;
	int Y       = c / FONT_SIZE;
	data += (FONT_SIZE * X) + (FONT_SIZE * FONT_RECT_SIZE * Y);
	for(;start_y < end_y; start_y++) {
		start_x = x * FONT_SIZE;
		for(;start_x < end_x; start_x++) {
			mailbox_fb_set_pixel(data[index++] ? CHAR_COLOR : 0, start_x, start_y);
		}
		index = 0;
		data += FONT_RECT_SIZE;
	}
}

void draw_text(char *str, int x, int y) {
	while(*str) {
		draw_char(*str, x, y);
		str++;
		x++;
	}
}

void draw_dword(uint32_t value, int x, int y) {
	draw_char( char_table[(value >> 28) & 0xF], x++, y);
	draw_char( char_table[(value >> 24) & 0xF], x++, y);
	draw_char( char_table[(value >> 20) & 0xF], x++, y);
	draw_char( char_table[(value >> 16) & 0xF], x++, y);
	draw_char( char_table[(value >> 12) & 0xF], x++, y);
	draw_char( char_table[(value >>  8) & 0xF], x++, y);
	draw_char( char_table[(value >>  4) & 0xF], x++, y);
	draw_char( char_table[(value >>  0) & 0xF], x++, y);
}






void draw_dump_data(uint8_t *buf) {
	int i = 0;
	int j = 0;
	int x = 0;
	int index = 0;
	for(j = 0 ; j < 28; j++) {
		x = 0;
		for(i = 0 ; i < 16; i++) {
			if((i % 4) == 0) x++;
			draw_char( char_table[ (0xF0 & (buf[index])) >> 4], x, j);
			x++;
			draw_char( char_table[buf[index] & 0x0F], x, j);
			x++;
			index++;
		}
	}
}

static uint32_t rand_a = 1;
static uint32_t rand_b = 23456;
static uint32_t rand_c = 7890123;
void init_random() {
	rand_a = 1;
	rand_b = 23456;
	rand_c = 7890123;
}

uint32_t random() {
	rand_a += rand_b;
	rand_b += rand_c;
	rand_c += rand_a;
	return (rand_a >> 16);
}

void usleep(int32_t count) {
	volatile int32_t k = count;
	while(k > 0) {
		k--;
	}
}

void memset_t(void *dest, uint8_t c, uint32_t size) {
	if(size & 0x3) {
		volatile uint8_t *p = (uint8_t *)dest;
		for(int i = 0 ; i < size; i++) {
			p[i] = c;
		}
	} else {
		volatile uint32_t *p = (uint32_t *)dest;
		size >>= 2;
		uint32_t data = c | c << 8 | c << 16 | c << 24;
		for(int i = 0 ; i < size; i++) {
			p[i] = data;
		}
	}
}


uint64_t get_systime(void) {
	uint64_t t;
	volatile uint32_t chi = IO_READ(SYST_CHI);
	volatile uint32_t clo = IO_READ(SYST_CLO);
	if(chi != IO_READ(SYST_CHI)) {
		chi = IO_READ(SYST_CHI);
		clo = IO_READ(SYST_CLO);
	}
	t  = chi;
	t <<= 32;
	t += clo;
	return t;
}

uint32_t get_systime_ms(void) {
	uint64_t t;
	volatile uint32_t chi = IO_READ(SYST_CHI);
	volatile uint32_t clo = IO_READ(SYST_CLO);
	if(chi != IO_READ(SYST_CHI)) {
		chi = IO_READ(SYST_CHI);
		clo = IO_READ(SYST_CLO);
	}
	t  = chi;
	t <<= 32;
	t += clo;
	return (uint32_t)((float)clo / 1000.0f);
}

volatile uint32_t get_systime32(void) {
	return IO_READ(SYST_CLO);
}


void msleep(uint32_t ms) {
	uint64_t at = get_systime() + ms * 1000;
	while(get_systime() < at);
	return;
}

void msleep32(uint32_t ms) {
	uint32_t at = get_systime32() + ms * 1000;
	while(get_systime32() < at);
	return;
}


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


