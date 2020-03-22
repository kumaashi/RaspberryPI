#ifndef _PCIE_H_
#define _PCIE_H_

#include "std.h"

uint32_t pcie_read(int index, uint32_t addr);
void pcie_init();

#endif //_PCIE_H_
