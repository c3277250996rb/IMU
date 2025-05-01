#include "my_w25q64.h"


uint32_t W25Q64_Read_Device_ID(void);
uint32_t W25Q64_Read_JEDEC_ID(void);

void W25Q64_Erase_Sector(uint32_t SectorAddress);
void W25Q64_Erase_Chip(void);

