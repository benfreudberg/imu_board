/*
 * spi_wrapper.h
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#ifndef SPI_WRAPPER_H_
#define SPI_WRAPPER_H_

#include "spi.h"
#include <stdbool.h>

void SPI_TransmitReceive_DMA_Blocking(SPI_HandleTypeDef *hspi,
                                      uint8_t *pTxData,
                                      uint8_t *pRxData,
                                      uint16_t Size);
void SPI_TransmitReceive_DMA_NonBlocking(SPI_HandleTypeDef *hspi,
                                      uint8_t *pTxData,
                                      uint8_t *pRxData,
                                      uint16_t Size);

#endif /* SPI_WRAPPER_H_ */
