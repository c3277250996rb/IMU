#include "main.h"



#ifndef USE_EMBEDFIRE_SPI


  /*********** */
 /* MY_W25Q64 */
/*********** */


#include "__module.h"
#include "my_spi.h"

#define W25Q64_Device_ID
#define W25Q64_JEDEC_ID
#define W25Q64_Dummy_Byte


uint32_t W25Q64_Read_Device_ID(void);
uint32_t W25Q64_Read_JEDEC_ID(void);

void W25Q64_Erase_Sector(uint32_t SectorAddress);
void W25Q64_Erase_Chip(void);


  /*********** */
 /* MY_W25Q64 */
/*********** */


#endif

