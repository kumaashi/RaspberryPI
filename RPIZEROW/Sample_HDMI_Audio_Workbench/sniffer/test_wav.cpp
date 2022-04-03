#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <vector>

#define BASE_ADDR                  0x20808000 //HD
#define HD_BASE                    ((uint32_t)start_addr)
#define HDMI_M_CTL                 ((volatile uint32_t *)(HD_BASE + 0x000c))
#define HDMI_MAI_CTL               ((volatile uint32_t *)(HD_BASE + 0x0014))
#define HDMI_MAI_THR               ((volatile uint32_t *)(HD_BASE + 0x0018))
#define HDMI_MAI_FMT               ((volatile uint32_t *)(HD_BASE + 0x001c))
#define HDMI_MAI_DATA              ((volatile uint32_t *)(HD_BASE + 0x0020))
#define HDMI_MAI_DATA1             ((volatile uint32_t *)(HD_BASE + 0x0024))
#define HDMI_MAI_SMP               ((volatile uint32_t *)(HD_BASE + 0x002c))
#define HDMI_VID_CTL               ((volatile uint32_t *)(HD_BASE + 0x0038))
#define HDMI_CSC_CTL               ((volatile uint32_t *)(HD_BASE + 0x0040))
#define HDMI_CSC_12_11             ((volatile uint32_t *)(HD_BASE + 0x0044))
#define HDMI_CSC_14_13             ((volatile uint32_t *)(HD_BASE + 0x0048))
#define HDMI_CSC_22_21             ((volatile uint32_t *)(HD_BASE + 0x004c))
#define HDMI_CSC_24_23             ((volatile uint32_t *)(HD_BASE + 0x0050))
#define HDMI_CSC_32_31             ((volatile uint32_t *)(HD_BASE + 0x0054))
#define HDMI_CSC_34_33             ((volatile uint32_t *)(HD_BASE + 0x0058))
#define HDMI_FRAME_COUNT           ((volatile uint32_t *)(HD_BASE + 0x0068))

uint32_t arandom() {
	static uint32_t a = 1;
	static uint32_t b = 12038971;
	static uint32_t c = 48590871;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

static unsigned int iec958_parity(unsigned int data)
{
	unsigned int parity;
	int bit;

	data >>= 4;     /* start from bit 4 */
	parity = 0;
	for (bit = 4; bit <= 30; bit++) {
		if (data & 1)
			parity++;
		data >>= 1;
	}
	return (parity & 1);
}

void print_regs(uint32_t *start_addr) {
	printf("HDMI_MAI_CTL     =0x%08X\n", * HDMI_MAI_CTL     ); 
	printf("HDMI_MAI_THR     =0x%08X\n", * HDMI_MAI_THR     ); 
	printf("HDMI_MAI_FMT     =0x%08X\n", * HDMI_MAI_FMT     ); 
	printf("HDMI_MAI_DATA    =0x%08X\n", * HDMI_MAI_DATA    ); 
	printf("HDMI_MAI_DATA1   =0x%08X\n", * HDMI_MAI_DATA1   ); 
	printf("HDMI_MAI_SMP     =0x%08X\n", * HDMI_MAI_SMP     ); 
	printf("HDMI_VID_CTL     =0x%08X\n", * HDMI_VID_CTL     ); 
	printf("HDMI_CSC_CTL     =0x%08X\n", * HDMI_CSC_CTL     ); 
	printf("HDMI_CSC_12_11   =0x%08X\n", * HDMI_CSC_12_11   ); 
	printf("HDMI_CSC_14_13   =0x%08X\n", * HDMI_CSC_14_13   ); 
	printf("HDMI_CSC_22_21   =0x%08X\n", * HDMI_CSC_22_21   ); 
	printf("HDMI_CSC_24_23   =0x%08X\n", * HDMI_CSC_24_23   ); 
	printf("HDMI_CSC_32_31   =0x%08X\n", * HDMI_CSC_32_31   ); 
	printf("HDMI_CSC_34_33   =0x%08X\n", * HDMI_CSC_34_33   ); 
	printf("HDMI_FRAME_COUNT =0x%08X\n", * HDMI_FRAME_COUNT ); 
}

int main() {
	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	void *map_base = mmap(NULL,
			4096,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			mem_fd,
			BASE_ADDR);	// phys_addr should be page-aligned.	
	uint32_t *start_addr = (uint32_t *)map_base;
	uint32_t *virt_addr = (uint32_t *)map_base;
	print_regs(start_addr);

	/*
	 */
	*HDMI_CSC_CTL   = 0;
	*HDMI_CSC_12_11 = 0;
	*HDMI_CSC_14_13 = 0;
	*HDMI_CSC_22_21 = 0;
	*HDMI_CSC_24_23 = 0;
	*HDMI_CSC_32_31 = 0;
	*HDMI_CSC_34_33 = 0;

	//*HDMI_MAI_CTL |= ~1;
	//*HDMI_MAI_FMT = 0x20800;
	//*HDMI_MAI_FMT = 0x20500;
	//*HDMI_VID_CTL = 0xC0080000;
	*HDMI_MAI_THR = 0x10101010;
	//*HDMI_MAI_THR = 0x02020202;
	*HDMI_MAI_CTL = 0x3228; 
	//*HDMI_MAI_CTL |= 1; 
	return 0;

	//file read
	printf("start file read\n");
	std::vector<uint32_t> vdata;
	FILE *fp = fopen("/boot/ipa.wav", "rb");
	if(fp) {
		while(1) {
			uint32_t data = 0;
			int ret = fread(&data, 1, sizeof(uint16_t), fp);
			if(ret <= 0)
				break;
			data <<= 16;
			vdata.push_back(data);
		}
	}
	fclose(fp);
	printf("end file read\n");

	int global_count = 0;
	int counter = 0;
	int ch = 0;
	uint32_t *pdata = vdata.data();
	while(1) {
		global_count++;
		uint32_t data = *pdata++;
		counter %= 192;
		ch %= 2;

		unsigned int byte = counter >> 3;
		unsigned int mask = 1 << (counter - (byte << 3));

		data >>= 4;
		data &= ~0xF;

		//data |= 0x40000000;
		//data |= 0x10000000;

		//todo status
		if(iec958_parity(data))
			data |= 0x80000000;

		/* 0x08, 0x02, 0x04 /* Z, X, Y */
		if(counter == 0) {
			data |= 0x08; //B
		} else if(ch == 1) {
			data |= 0x04; //W
		} else {
			data |= 0x02; //M
		}

		//todo
		int loop_count = 0;
		int busy_th = 0x100;
		if((global_count % 16) == 0) {
			volatile int wait_cnt = 0x100;
			while(wait_cnt--);
		}
		while(*HDMI_MAI_CTL & (1 << 11)) {
			loop_count++;
			if(loop_count > busy_th) {
				loop_count = 0;
				busy_th <<= 1;
				//printf("BUSY global_count=%d, %08X\n", global_count, *HDMI_MAI_CTL);
				*HDMI_MAI_CTL |= (1 << 2 );
				*HDMI_MAI_CTL |= (1 << 1 );
			}
		}
		*HDMI_MAI_DATA = data;

		counter++;
		ch++;
	}

	printf("\n");
	return 0;
}

