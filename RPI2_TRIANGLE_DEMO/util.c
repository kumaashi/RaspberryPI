#include "util.h"

static int CHAR_COLOR = 0xFFFFFFFF;

volatile static uint8_t table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

int16_t sintable[SIN_TABLE_MAX] = {
	0, 0, 1, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 10, 11, 
	12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 19, 20, 20, 21, 22, 
	22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 28, 29, 
	29, 29, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
	32, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 29, 
	29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 
	22, 22, 21, 20, 20, 19, 19, 18, 17, 17, 16, 15, 15, 14, 13, 12, 
	12, 11, 10, 10, 9, 8, 7, 7, 6, 5, 4, 3, 3, 2, 1, 0, 
	0, 0, -1, -2, -3, -3, -4, -5, -6, -7, -7, -8, -9, -10, -10, -11, 
	-12, -12, -13, -14, -15, -15, -16, -17, -17, -18, -19, -19, -20, -20, -21, -22, 
	-22, -23, -23, -24, -24, -25, -25, -26, -26, -27, -27, -27, -28, -28, -28, -29, 
	-29, -29, -30, -30, -30, -30, -31, -31, -31, -31, -31, -31, -31, -31, -31, -31, 
	-32, -31, -31, -31, -31, -31, -31, -31, -31, -31, -31, -30, -30, -30, -30, -29, 
	-29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -25, -25, -24, -24, -23, -23, 
	-22, -22, -21, -20, -20, -19, -19, -18, -17, -17, -16, -15, -15, -14, -13, -12, 
	-12, -11, -10, -10, -9, -8, -7, -7, -6, -5, -4, -3, -3, -2, -1, 0, 
};


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
	uint8_t *data = get_fontdata16x16();
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
	draw_char( table[(value >> 28) & 0xF], x++, y);
	draw_char( table[(value >> 24) & 0xF], x++, y);
	draw_char( table[(value >> 20) & 0xF], x++, y);
	draw_char( table[(value >> 16) & 0xF], x++, y);
	draw_char( table[(value >> 12) & 0xF], x++, y);
	draw_char( table[(value >>  8) & 0xF], x++, y);
	draw_char( table[(value >>  4) & 0xF], x++, y);
	draw_char( table[(value >>  0) & 0xF], x++, y);
}

void uart_put_dword(uint32_t value) {
	uart_putc( table[(value >> 28) & 0xF]);
	uart_putc( table[(value >> 24) & 0xF]);
	uart_putc( table[(value >> 20) & 0xF]);
	uart_putc( table[(value >> 16) & 0xF]);
	uart_putc( table[(value >> 12) & 0xF]);
	uart_putc( table[(value >>  8) & 0xF]);
	uart_putc( table[(value >>  4) & 0xF]);
	uart_putc( table[(value >>  0) & 0xF]);
}

void uart_char_putc(uint8_t value) {
	uart_putc( table[(value >>  4) & 0xF]);
	uart_putc( table[(value >>  0) & 0xF]);
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
			draw_char( table[ (0xF0 & (buf[index])) >> 4], x, j);
			x++;
			draw_char( table[buf[index] & 0x0F], x, j);
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

uint32_t memtest() {
	volatile uint32_t sum    = 0;
	volatile uint32_t *start = (volatile uint32_t *)0x0;
	volatile uint32_t *end   = (volatile uint32_t *)MEM_MAX;
	while(start != end) {
		sum += *start++;
	}
	return sum;
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


