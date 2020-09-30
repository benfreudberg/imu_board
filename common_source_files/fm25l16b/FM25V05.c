/*
 * FM25V05.c
 *
 *  Created on: Jul 16, 2019
 *      Author: Ben Freudberg
 */

#include "stdint.h"
#include "FM25V05.h"
#include "string.h"


/*PRIVATE FUNCTIONS*/
static void FM_WriteEnable(fm25v05_t* module) {
	const uint8_t opcode = FM25V05_WREN;
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, (uint8_t*) &opcode, 1, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);
}

static void FM_WriteDisable(fm25v05_t* module) {
	const uint8_t opcode = FM25V05_WRDI;
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, (uint8_t*) &opcode, 1, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);
}

/*PUBLIC FUNCTIONS*/
void FM_ReadData(fm25v05_t* module, uint16_t addr, uint8_t* rx_data, uint16_t num_bytes) {
	const uint8_t opcode = FM25V05_READ;
	uint8_t tx_data_temp[FM_MAX_BYTES + 3] = {0};
	uint8_t rx_data_temp[FM_MAX_BYTES + 3] = {0};
	tx_data_temp[0] = opcode;
	tx_data_temp[1] = (addr >> 8) & 0xff;
	tx_data_temp[2] = (addr >> 0) & 0xff;

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(module->SPI_Bus, tx_data_temp, rx_data_temp, num_bytes + 3, num_bytes/1000 + 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	memcpy(rx_data, &rx_data_temp[3], num_bytes);
}

void FM_WriteData(fm25v05_t* module, uint16_t addr, uint8_t* tx_data, uint16_t num_bytes) {
	const uint8_t opcode = FM25V05_WRITE;
	uint8_t tx_data_temp[FM_MAX_BYTES + 3] = {0};
	tx_data_temp[0] = opcode;
	tx_data_temp[1] = (addr >> 8) & 0xff;
	tx_data_temp[2] = (addr >> 0) & 0xff;
	memcpy(&tx_data_temp[3], tx_data, num_bytes);

	FM_WriteEnable(module);

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, tx_data_temp, num_bytes + 3, num_bytes/1000 + 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	FM_WriteDisable(module);
}

void FM_WriteSR(fm25v05_t* module, uint8_t sr_data) {
	const uint8_t opcode = FM25V05_WRSR;
	uint8_t tx_data_temp[2] = {0};
	tx_data_temp[0] = opcode;
	tx_data_temp[1] = sr_data;

	FM_WriteEnable(module);

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, tx_data_temp, 2, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	FM_WriteDisable(module);
}

uint8_t FM_ReadSR(fm25v05_t* module) {
	const uint8_t opcode = FM25V05_RDSR;
	uint8_t tx_data_temp[2] = {0};
	uint8_t rx_data_temp[2] = {0};
	tx_data_temp[0] = opcode;

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(module->SPI_Bus, tx_data_temp, rx_data_temp, 2, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	return rx_data_temp[1];
}

void FM_GoToSleep(fm25v05_t* module) {
	const uint8_t opcode = FM25V05_SLEEP;
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, (uint8_t*) &opcode, 1, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);
}

void FM_ReadID(fm25v05_t* module, uint8_t* id_data) {
	const uint8_t opcode = FM25V05_RDID;
	uint8_t tx_data_temp[10] = {0};
	uint8_t rx_data_temp[10] = {0};
	tx_data_temp[0] = opcode;

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(module->SPI_Bus, tx_data_temp, rx_data_temp, 10, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	memcpy(id_data, &rx_data_temp[1], 9);
}

void FM_ClearData(fm25v05_t* module) {
	const uint8_t opcode = FM25V05_WRITE;
	const uint16_t mem_size = MEMORY_SIZE;
	uint16_t addr = 0x0000;
	uint8_t tx_data_temp[mem_size/2 + 3];
	for (uint32_t i = 0; i < sizeof(tx_data_temp); i++) {
		tx_data_temp[i] = 0;
	}
	tx_data_temp[0] = opcode;
	memcpy(&tx_data_temp[1], &addr, sizeof(addr));

	FM_WriteEnable(module);

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, tx_data_temp, sizeof(tx_data_temp), sizeof(tx_data_temp)/1000 + 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	addr = 0x00FF;
	memcpy(&tx_data_temp[1], &addr, sizeof(addr));
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, tx_data_temp, sizeof(tx_data_temp), sizeof(tx_data_temp)/1000 + 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	FM_WriteDisable(module);
}
