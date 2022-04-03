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

//#define BASE_ADDR 0x20902000 //HDMI
#define BASE_ADDR 0x20808000 //HD
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

	*HDMI_MAI_FMT = 0x20200;
	printf("\n");
	return 0;
}

