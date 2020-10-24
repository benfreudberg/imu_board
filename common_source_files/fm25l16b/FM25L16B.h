/*
 * FM25L16B.h
 *
 *  Created on: Jul 16, 2019
 *      Author: Ben Freudberg
 */

#ifndef FM25L16B_H_
#define FM25L16B_H_

#include "stdint.h"
#include "stm32g4xx_hal.h"

#define FM_MAX_BYTES    256	 //max bytes for a transmit or receive (this is an arbitrary limit)
#define MEMORY_SIZE     0x0800

//Opcodes
#define FM25L16B_WREN   0x06 //Set write enable latch
#define FM25L16B_WRDI   0x04 //Reset write enable latch
#define FM25L16B_RDSR   0x05 //Read status register
#define FM25L16B_WRSR   0x01 //Write status register
#define FM25L16B_READ   0x03 //Read memory data
#define FM25L16B_WRITE  0x02 //Write memory data

typedef struct {
	SPI_HandleTypeDef*  SPI_Bus;
	uint16_t            cs_pin;
	GPIO_TypeDef*       cs_port;
} FM25L16B_t;

void FM_ReadData(FM25L16B_t* module, uint32_t addr, uint8_t* rx_data, uint16_t num_bytes);
void FM_WriteData(FM25L16B_t* module, uint32_t addr, uint8_t* tx_data, uint16_t num_bytes);
void FM_WriteSR(FM25L16B_t* module, uint8_t sr_data);
uint8_t FM_ReadSR(FM25L16B_t* module);
void FM_ClearData(FM25L16B_t* module); //writes all 0s

#endif /* FM25L16B_H_ */
