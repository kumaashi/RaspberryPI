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
int main() {
	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	void *map_base = mmap(NULL,
			4096,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			mem_fd,
			0x20980000);	// phys_addr should be page-aligned.	

	uint32_t *virt_addr = (uint32_t *)map_base;
	uint32_t *virt_mdio = (uint32_t *)map_base;
	virt_mdio += (0x88 / 4);

	uint32_t *virt_hprt0 = (uint32_t *)map_base;
	virt_hprt0 += (0x440 / 4);

	for(int i = 0 ; i < 40; i++) {
		printf("%08X\n", *virt_addr);
		virt_addr++;
	}
	printf("MDIO = %08X\n",*virt_mdio);
	printf("HPRT0 = %08X\n",*virt_hprt0);

	return 0;
}

