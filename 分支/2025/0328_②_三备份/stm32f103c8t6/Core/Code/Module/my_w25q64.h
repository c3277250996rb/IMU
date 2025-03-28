#include "__module.h"
#include "my_spi.h"

#define W25Q64_Device_ID
#define W25Q64_JEDEC_ID
#define W25Q64_Dummy_Byte



uint32_t W25Q64_Read_Device_ID(void);
uint32_t W25Q64_Read_JEDEC_ID(void);

void W25Q64_Erase_Sector(uint32_t SectorAddress);
void W25Q64_Erase_Chip(void);


void my_w25q64_self_test(void);
