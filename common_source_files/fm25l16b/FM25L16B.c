/*
 * FM25L16B.c
 *
 *  Created on: Jul 16, 2019
 *      Author: Ben Freudberg
 */

#include "FM25L16B.h"
#include "stdint.h"
#include "string.h"


/*PRIVATE FUNCTIONS*/
static void FM_WriteEnable(FM25L16B_t* module) {
	const uint8_t opcode = FM25L16B_WREN;
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, (uint8_t*) &opcode, 1, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);
}

static void FM_WriteDisable(FM25L16B_t* module) {
	const uint8_t opcode = FM25L16B_WRDI;
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, (uint8_t*) &opcode, 1, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);
}

/*PUBLIC FUNCTIONS*/
void FM_ReadData(FM25L16B_t* module, uint32_t addr, uint8_t* rx_data, uint16_t num_bytes) {
	const uint8_t opcode = FM25L16B_READ;
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

void FM_WriteData(FM25L16B_t* module, uint32_t addr, uint8_t* tx_data, uint16_t num_bytes) {
	const uint8_t opcode = FM25L16B_WRITE;
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

void FM_WriteSR(FM25L16B_t* module, uint8_t sr_data) {
	const uint8_t opcode = FM25L16B_WRSR;
	uint8_t tx_data_temp[2] = {0};
	tx_data_temp[0] = opcode;
	tx_data_temp[1] = sr_data;

	FM_WriteEnable(module);

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, tx_data_temp, 2, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	FM_WriteDisable(module);
}

uint8_t FM_ReadSR(FM25L16B_t* module) {
	const uint8_t opcode = FM25L16B_RDSR;
	uint8_t tx_data_temp[2] = {0};
	uint8_t rx_data_temp[2] = {0};
	tx_data_temp[0] = opcode;

	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(module->SPI_Bus, tx_data_temp, rx_data_temp, 2, 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	return rx_data_temp[1];
}

void FM_ClearData(FM25L16B_t* module) {
	const uint8_t opcode = FM25L16B_WRITE;
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

	addr = mem_size/2;
	memcpy(&tx_data_temp[1], &addr, sizeof(addr));
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(module->SPI_Bus, tx_data_temp, sizeof(tx_data_temp), sizeof(tx_data_temp)/1000 + 1);
	HAL_GPIO_WritePin(module->cs_port, module->cs_pin, GPIO_PIN_SET);

	FM_WriteDisable(module);
}
