#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <malloc.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <unistd.h>
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


#define HDMI_BASE_ADDR             0x20902000 //HDMI BASE
#define HDMI_BASE                  ((uint32_t)start_hdmi_addr)
#define HDMI_CORE_REV              ((volatile uint32_t *)(HDMI_BASE + 0x0000))
#define HDMI_SW_RESET_CONTROL      ((volatile uint32_t *)(HDMI_BASE + 0x0004))
#define HDMI_HOTPLUG_INT           ((volatile uint32_t *)(HDMI_BASE + 0x0008))
#define HDMI_HOTPLUG               ((volatile uint32_t *)(HDMI_BASE + 0x000c))
#define HDMI_FIFO_CTL              ((volatile uint32_t *)(HDMI_BASE + 0x005c))
#define HDMI_MAI_CHANNEL_MAP       ((volatile uint32_t *)(HDMI_BASE + 0x0090))
#define HDMI_MAI_CONFIG            ((volatile uint32_t *)(HDMI_BASE + 0x0094))
#define HDMI_MAI_FORMAT            ((volatile uint32_t *)(HDMI_BASE + 0x0098))
#define HDMI_AUDIO_PACKET_CONFIG   ((volatile uint32_t *)(HDMI_BASE + 0x009c))
#define HDMI_RAM_PACKET_CONFIG     ((volatile uint32_t *)(HDMI_BASE + 0x00a0))
#define HDMI_RAM_PACKET_STATUS     ((volatile uint32_t *)(HDMI_BASE + 0x00a4))
#define HDMI_CRP_CFG               ((volatile uint32_t *)(HDMI_BASE + 0x00a8))
#define HDMI_CTS_0                 ((volatile uint32_t *)(HDMI_BASE + 0x00ac))
#define HDMI_CTS_1                 ((volatile uint32_t *)(HDMI_BASE + 0x00b0))
#define HDMI_SCHEDULER_CONTROL     ((volatile uint32_t *)(HDMI_BASE + 0x00c0))
#define HDMI_HORZA                 ((volatile uint32_t *)(HDMI_BASE + 0x00c4))
#define HDMI_HORZB                 ((volatile uint32_t *)(HDMI_BASE + 0x00c8))
#define HDMI_VERTA0                ((volatile uint32_t *)(HDMI_BASE + 0x00cc))
#define HDMI_VERTB0                ((volatile uint32_t *)(HDMI_BASE + 0x00d0))
#define HDMI_VERTA1                ((volatile uint32_t *)(HDMI_BASE + 0x00d4))
#define HDMI_VERTB1                ((volatile uint32_t *)(HDMI_BASE + 0x00d8))
#define HDMI_CEC_CNTRL_1           ((volatile uint32_t *)(HDMI_BASE + 0x00e8))
#define HDMI_CEC_CNTRL_2           ((volatile uint32_t *)(HDMI_BASE + 0x00ec))
#define HDMI_CEC_CNTRL_3           ((volatile uint32_t *)(HDMI_BASE + 0x00f0))
#define HDMI_CEC_CNTRL_4           ((volatile uint32_t *)(HDMI_BASE + 0x00f4))
#define HDMI_CEC_CNTRL_5           ((volatile uint32_t *)(HDMI_BASE + 0x00f8))
#define HDMI_CEC_TX_DATA_1         ((volatile uint32_t *)(HDMI_BASE + 0x00fc))
#define HDMI_CEC_TX_DATA_2         ((volatile uint32_t *)(HDMI_BASE + 0x0100))
#define HDMI_CEC_TX_DATA_3         ((volatile uint32_t *)(HDMI_BASE + 0x0104))
#define HDMI_CEC_TX_DATA_4         ((volatile uint32_t *)(HDMI_BASE + 0x0108))
#define HDMI_CEC_RX_DATA_1         ((volatile uint32_t *)(HDMI_BASE + 0x010c))
#define HDMI_CEC_RX_DATA_2         ((volatile uint32_t *)(HDMI_BASE + 0x0110))
#define HDMI_CEC_RX_DATA_3         ((volatile uint32_t *)(HDMI_BASE + 0x0114))
#define HDMI_CEC_RX_DATA_4         ((volatile uint32_t *)(HDMI_BASE + 0x0118))
#define HDMI_TX_PHY_RESET_CTL      ((volatile uint32_t *)(HDMI_BASE + 0x02c0))
#define HDMI_TX_PHY_CTL_0          ((volatile uint32_t *)(HDMI_BASE + 0x02c4))
#define HDMI_CEC_CPU_STATUS        ((volatile uint32_t *)(HDMI_BASE + 0x0340))
#define HDMI_CEC_CPU_SET           ((volatile uint32_t *)(HDMI_BASE + 0x0344))
#define HDMI_CEC_CPU_CLEAR         ((volatile uint32_t *)(HDMI_BASE + 0x0348))
#define HDMI_CEC_CPU_MASK_STATUS   ((volatile uint32_t *)(HDMI_BASE + 0x034c))
#define HDMI_CEC_CPU_MASK_SET      ((volatile uint32_t *)(HDMI_BASE + 0x0350))
#define HDMI_CEC_CPU_MASK_CLEAR    ((volatile uint32_t *)(HDMI_BASE + 0x0354))
#define HDMI_RAM_PACKET_START      ((volatile uint32_t *)(HDMI_BASE + 0x0400))
#define HDMI_RAM_PACKET(ch, x) ((volatile uint32_t *)(HDMI_BASE + (0x24 * ch) + 0x0400 + (4 * (x))))

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
	printf("HDMI_M_CTL       =0x%08X\n", * HDMI_M_CTL     ); 
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

void debug_hdmi(uint32_t *start_hdmi_addr) {
	printf("HDMI_CORE_REV           = 0x%08X\n",*HDMI_CORE_REV            );
	printf("HDMI_SW_RESET_CONTROL   = 0x%08X\n",*HDMI_SW_RESET_CONTROL    );
	printf("HDMI_HOTPLUG_INT        = 0x%08X\n",*HDMI_HOTPLUG_INT         );
	printf("HDMI_HOTPLUG            = 0x%08X\n",*HDMI_HOTPLUG             );
	printf("HDMI_FIFO_CTL           = 0x%08X\n",*HDMI_FIFO_CTL            );
	printf("HDMI_MAI_CHANNEL_MAP    = 0x%08X\n",*HDMI_MAI_CHANNEL_MAP     );
	printf("HDMI_MAI_CONFIG         = 0x%08X\n",*HDMI_MAI_CONFIG          );
	printf("HDMI_MAI_FORMAT         = 0x%08X\n",*HDMI_MAI_FORMAT          );
	printf("HDMI_AUDIO_PACKET_CONFIG= 0x%08X\n",*HDMI_AUDIO_PACKET_CONFIG );
	printf("HDMI_RAM_PACKET_CONFIG  = 0x%08X\n",*HDMI_RAM_PACKET_CONFIG   );
	printf("HDMI_RAM_PACKET_STATUS  = 0x%08X\n",*HDMI_RAM_PACKET_STATUS   );
	printf("HDMI_CRP_CFG            = 0x%08X\n",*HDMI_CRP_CFG             );
	printf("HDMI_CTS_0              = 0x%08X\n",*HDMI_CTS_0               );
	printf("HDMI_CTS_1              = 0x%08X\n",*HDMI_CTS_1               );
	printf("HDMI_SCHEDULER_CONTROL  = 0x%08X\n",*HDMI_SCHEDULER_CONTROL   );
	printf("HDMI_HORZA              = 0x%08X\n",*HDMI_HORZA               );
	printf("HDMI_HORZB              = 0x%08X\n",*HDMI_HORZB               );
	printf("HDMI_VERTA0             = 0x%08X\n",*HDMI_VERTA0              );
	printf("HDMI_VERTB0             = 0x%08X\n",*HDMI_VERTB0              );
	printf("HDMI_VERTA1             = 0x%08X\n",*HDMI_VERTA1              );
	printf("HDMI_VERTB1             = 0x%08X\n",*HDMI_VERTB1              );
	printf("HDMI_CEC_CNTRL_1        = 0x%08X\n",*HDMI_CEC_CNTRL_1         );
	printf("HDMI_CEC_CNTRL_2        = 0x%08X\n",*HDMI_CEC_CNTRL_2         );
	printf("HDMI_CEC_CNTRL_3        = 0x%08X\n",*HDMI_CEC_CNTRL_3         );
	printf("HDMI_CEC_CNTRL_4        = 0x%08X\n",*HDMI_CEC_CNTRL_4         );
	printf("HDMI_CEC_CNTRL_5        = 0x%08X\n",*HDMI_CEC_CNTRL_5         );
	printf("HDMI_CEC_TX_DATA_1      = 0x%08X\n",*HDMI_CEC_TX_DATA_1       );
	printf("HDMI_CEC_TX_DATA_2      = 0x%08X\n",*HDMI_CEC_TX_DATA_2       );
	printf("HDMI_CEC_TX_DATA_3      = 0x%08X\n",*HDMI_CEC_TX_DATA_3       );
	printf("HDMI_CEC_TX_DATA_4      = 0x%08X\n",*HDMI_CEC_TX_DATA_4       );
	printf("HDMI_CEC_RX_DATA_1      = 0x%08X\n",*HDMI_CEC_RX_DATA_1       );
	printf("HDMI_CEC_RX_DATA_2      = 0x%08X\n",*HDMI_CEC_RX_DATA_2       );
	printf("HDMI_CEC_RX_DATA_3      = 0x%08X\n",*HDMI_CEC_RX_DATA_3       );
	printf("HDMI_CEC_RX_DATA_4      = 0x%08X\n",*HDMI_CEC_RX_DATA_4       );
	printf("HDMI_TX_PHY_RESET_CTL   = 0x%08X\n",*HDMI_TX_PHY_RESET_CTL    );
	printf("HDMI_TX_PHY_CTL_0       = 0x%08X\n",*HDMI_TX_PHY_CTL_0        );
	printf("HDMI_CEC_CPU_STATUS     = 0x%08X\n",*HDMI_CEC_CPU_STATUS      );
	printf("HDMI_CEC_CPU_SET        = 0x%08X\n",*HDMI_CEC_CPU_SET         );
	printf("HDMI_CEC_CPU_CLEAR      = 0x%08X\n",*HDMI_CEC_CPU_CLEAR       );
	printf("HDMI_CEC_CPU_MASK_STATUS= 0x%08X\n",*HDMI_CEC_CPU_MASK_STATUS );
	printf("HDMI_CEC_CPU_MASK_SET   = 0x%08X\n",*HDMI_CEC_CPU_MASK_SET    );
	printf("HDMI_CEC_CPU_MASK_CLEAR = 0x%08X\n",*HDMI_CEC_CPU_MASK_CLEAR  );
	printf("HDMI_RAM_PACKET_START   = 0x%08X\n",*HDMI_RAM_PACKET_START    );
	for(int i = 0 ; i < 128;i++)
		printf("%d : %08X\n", i, *HDMI_RAM_PACKET(0, i));
}

int main() {
	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	void *map_base = mmap(NULL,
			4096,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			mem_fd,
			BASE_ADDR);	// phys_addr should be page-aligned.	
	int mem_fd_hdmi = open("/dev/mem", O_RDWR | O_SYNC);
	void *map_base_hdmi = mmap(NULL,
			4096,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			mem_fd_hdmi,
			HDMI_BASE_ADDR );
	uint32_t *start_addr = (uint32_t *)map_base;
	uint32_t *virt_addr = (uint32_t *)map_base;

	uint32_t *start_hdmi_addr = (uint32_t *)map_base_hdmi;
	print_regs(start_addr);
	debug_hdmi(start_hdmi_addr);

	//*HDMI_CSC_CTL = 0x00000000;
	//*HDMI_CTS_0 = 0;
	//*HDMI_CTS_1 = 0;
	*HDMI_RAM_PACKET_CONFIG &= ~4;
	*HDMI_VID_CTL  =0xC0000000;
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
