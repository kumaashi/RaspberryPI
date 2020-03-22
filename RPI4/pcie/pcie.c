#include "std.h"
#include "uart.h"
#include "pcie.h"
#include "hwio.h"


uint32_t 
pcie_read(int index, uint32_t addr) {
	IO_WRITE(0xFD509000, index << 20);
	volatile uint32_t ret = IO_READ(0xFD508000 + addr);
	return ret;
}

void
pcie_init()
{
	//RESET ASSERT
	IO_WRITE(0xFD509210, 0x00000003);
	usleep(1000000);

	//RESET DEASSERT
	IO_WRITE(0xFD509210, 0x00000001);
	usleep(1000000);
	IO_WRITE( 0xFD504308, 0xFFFFFFFF);
	IO_WRITE( 0xFD504310, 0xFFFFFFFF);
	
	//done setup
	IO_WRITE(0xFD509210, 0x00000000);
	usleep(1000000);
	
	//pci to pci
	IO_WRITE(0xFD50043C, 0x0060400);
	
	//pbus
	IO_WRITE(0xFD500018, 0x00010100);
}
