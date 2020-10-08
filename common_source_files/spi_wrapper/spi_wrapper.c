/*
 * spi_wrapper.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#include "spi_wrapper.h"

volatile bool SPI1_busy = false;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  if (hspi == &hspi1) {
    HAL_GPIO_WritePin(CS_ICM0_GPIO_Port, CS_ICM0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_FRAM0_GPIO_Port, CS_FRAM0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_MMC0_GPIO_Port, CS_MMC0_Pin, GPIO_PIN_SET);
    SPI1_busy = false;
  }
}

void SPI_TransmitReceive_DMA_Blocking(SPI_HandleTypeDef *hspi,
                                      uint8_t *pTxData,
                                      uint8_t *pRxData,
                                      uint16_t Size) {
  volatile bool *SPI_busy;
  if (hspi == &hspi1) {
    SPI_busy = &SPI1_busy;
  } else return;

  while(*SPI_busy);
  *SPI_busy = true;
  HAL_SPI_TransmitReceive_DMA(hspi, pTxData, pRxData, Size);
  while(*SPI_busy);
}

void SPI_TransmitReceive_DMA_NonBlocking(SPI_HandleTypeDef *hspi,
                                      uint8_t *pTxData,
                                      uint8_t *pRxData,
                                      uint16_t Size) {
  volatile bool *SPI_busy;
  if (hspi == &hspi1) {
    SPI_busy = &SPI1_busy;
  } else return;

  while(*SPI_busy);
  *SPI_busy = true;
  HAL_SPI_TransmitReceive_DMA(hspi, pTxData, pRxData, Size);
}
