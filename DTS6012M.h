/*
 * 6012m.h
 *
 *  Created on: Mar 25, 2025
 *      Author: eggsack
 *
 *  Basic library to interact with the DTS6012M TOF module.
 *  Reads DTS6012M data via I2C
 *  Functions to enable/disable DTS6012M
 */

#ifndef INC_DTS6012M_H_
#define INC_DTS6012M_H_

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_i2c.h"

/*
 * I2C Address defines
 */
#define DTS6012M_ADD (0x51 << 1) // DTS6012M default address (programmable)
#define MSB_ADD 0x00 // data MSB
#define LSB_ADD 0x01 // data LSB
#define LASER_EN_ADD 0x02 // Enable laser: 1 = start, 0 = stop

/*
 * Scaling factor to convert raw reading into distance in meters and offset amount
 */
#define DTS6012M_SCALING_FACTOR 0.001
#define DTS6012M_OFFSET_DISTANCE 0.0

/*
 * DTS6012M structure
 */
typedef struct
{
	I2C_HandleTypeDef* i2c_handle;
	uint16_t raw_distance;
}DTS6012M;

/*
 * Public function prototypes
 */
void DTS6012M_Init(DTS6012M* dev, I2C_HandleTypeDef* i2c);
void DTS6012M_StartLaser(DTS6012M* dev);
void DTS6012M_StopLaser(DTS6012M* dev);
float DTS6012M_GetDistance(DTS6012M* dev);

#endif /* INC_DTS6012M_H_ */
