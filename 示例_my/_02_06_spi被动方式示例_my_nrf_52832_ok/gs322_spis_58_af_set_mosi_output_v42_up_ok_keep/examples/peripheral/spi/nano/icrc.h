#ifndef ICRC_H
#define ICRC_H

#include "typedef.h"


//extern void CRC_Init(void);
//extern u16 get_crc_data( u8 *buffer ,u8 len);


static uint16_t crc16_byte(uint16_t crc, const uint8_t data);
uint16_t crc16(uint16_t crc, uint8_t const *buffer, uint32_t len);



#endif








