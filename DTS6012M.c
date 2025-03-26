/*
 * 6012m.c
 *
 *  Created on: Mar 25, 2025
 *      Author: yizha
 */

#include "DTS6012M.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * I2C Mem read & write functions
 */
__STATIC_INLINE HAL_StatusTypeDef DTS6012M_ReadRegisters(DTS6012M* dev, uint16_t reg, uint8_t* buff, uint16_t length)
{
	return HAL_I2C_Mem_Read(dev->i2c_handle, DTS6012M_ADD, reg, I2C_MEMADD_SIZE_8BIT, buff, length, 100);
}

__STATIC_INLINE HAL_StatusTypeDef DTS6012M_WriteRegister(DTS6012M*dev, uint16_t reg, uint8_t data)
{
	return HAL_I2C_Mem_Write(dev->i2c_handle, DTS6012M_ADD, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

/*
 * @brief Turns ON laser (starts measurement)
 * @param DTS6012M* sensor
 */
void DTS6012M_StartLaser(DTS6012M* dev)
{
	DTS6012M_WriteRegister(dev, LASER_EN_ADD, 1);
}

/*
 * @brief Turns OFF laser (stops measurement)
 * @param DTS6012M* sensor
 */
void DTS6012M_StopLaser(DTS6012M* dev)
{
	DTS6012M_WriteRegister(dev, LASER_EN_ADD, 0);
}

/*
 * @brief Initialize sensor object with I2C handle
 * @param DTS6012M* sensor
 * @param I2C_HandleTypeDef* hi2cx
 * @note Checks for presence of sensor, sets sensor i2c handle to NULL if no sensor detected
 */
void DTS6012M_Init(DTS6012M* dev, I2C_HandleTypeDef* i2c)
{
	dev->i2c_handle = i2c;
	dev->raw_distance = 0;

	/* Check if sensor is available */
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(dev->i2c_handle, DTS6012M_ADD, 10, 1000);

	if(status != HAL_OK)
	{
		dev->i2c_handle = NULL;
		return;
	}

	/* Enables laser */
	DTS6012M_StartLaser(dev);
}

/*
 * @brief Returns the distance measured in meters
 * @param DTS6012M* sensor
 */
float DTS6012M_GetDistance(DTS6012M* dev)
{
	uint8_t bits[2];
	HAL_StatusTypeDef status = DTS6012M_ReadRegisters(dev, MSB_ADD, bits, 2);

	if(status != HAL_OK)
	{
		return 0;
	}

	uint16_t data = (((bits[0] & 0xFF) << 8) | bits[1]);
	dev->raw_distance = data;
	return (float)data * DTS6012M_SCALING_FACTOR + (float)DTS6012M_OFFSET_DISTANCE;
}

#ifdef __cplusplus
}
#endif

