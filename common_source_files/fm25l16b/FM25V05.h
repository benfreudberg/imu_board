/*
 * FM25V05.h
 *
 *  Created on: Jul 16, 2019
 *      Author: Ben Freudberg
 */

#ifndef FM25V05_H_
#define FM25V05_H_

#include "stdint.h"
#include "stm32g4xx_hal.h"

#define FM_MAX_BYTES		256	 //max bytes for a transmit or receive (this is an arbitrary limit)
#define MEMORY_SIZE			0xFFFF

//Opcodes
#define FM25V05_WREN		0x06 //Set write enable latch
#define FM25V05_WRDI		0x04 //Reset write enable latch
#define FM25V05_RDSR		0x05 //Read status register
#define FM25V05_WRSR		0x01 //Write status register
#define FM25V05_READ		0x03 //Read memory data
#define FM25V05_FSTRD		0x0B //Fast read memory data
#define FM25V05_WRITE		0x02 //Write memory data
#define FM25V05_SLEEP		0xB9 //Enter sleep mode
#define FM25V05_RDID		0x9F //Read device ID

typedef struct {
	SPI_HandleTypeDef* 	SPI_Bus;
	uint16_t 						cs_pin;
	GPIO_TypeDef*				cs_port;
} fm25v05_t;

void FM_ReadData(fm25v05_t* module, uint16_t addr, uint8_t* rx_data, uint16_t num_bytes);
void FM_WriteData(fm25v05_t* module, uint16_t addr, uint8_t* tx_data, uint16_t num_bytes);
void FM_WriteSR(fm25v05_t* module, uint8_t sr_data);
uint8_t FM_ReadSR(fm25v05_t* module);
void FM_GoToSleep(fm25v05_t* module);
void FM_ReadID(fm25v05_t* module, uint8_t* id_data); //id_data must point to uint8_t[9+]
void FM_ClearData(fm25v05_t* module); //writes all 0s

#endif /* FM25V05_H_ */
