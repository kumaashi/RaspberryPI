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

#define BASE_ADDR 0x20902000 //HDMI

#define HDMI_BASE                  ((uint32_t)start_addr)
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

int main() {
	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	void *map_base = mmap(NULL,
			4096,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			mem_fd,
			BASE_ADDR );	// phys_addr should be page-aligned.	
	uint32_t *start_addr = (uint32_t *)map_base;
	uint32_t *virt_addr = (uint32_t *)map_base;
	start_addr = (uint32_t *)map_base;
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
	for(int ch = 0 ; ch <= 8; ch++) {
		for(int i = 0 ; i < 9; i++) {
			printf("HDMI_RAM_PACKET(%d, %d)=0x%08X\n", ch, i, *HDMI_RAM_PACKET(ch, i));
		}
	}


	*HDMI_TX_PHY_CTL_0 = 0x8C000000;
	printf("\n");
	return 0;
}

