#ifndef _HAL_HEMP_HUM_H_
#define _HAL_HEMP_HUM_H_

#include "common.h"

#define DHT11_DIN_Pin GPIO_PIN_0 //ÎÂÊª¶È´«¸ÐÆ÷ GPIOB0
#define DHT11_DIN_GPIO_Port GPIOB


#define	DHT11_DQ_OUT_1	 HAL_GPIO_WritePin(DHT11_DIN_GPIO_Port, DHT11_DIN_Pin, GPIO_PIN_SET)
#define	DHT11_DQ_OUT_0 	 HAL_GPIO_WritePin(DHT11_DIN_GPIO_Port, DHT11_DIN_Pin, GPIO_PIN_RESET)
#define	DHT11_DQ_IN   	 HAL_GPIO_ReadPin(DHT11_DIN_GPIO_Port, DHT11_DIN_Pin)


uint8_t dht11Read(uint8_t *temperature, uint8_t *humidity); 
 
void Dht11_Test(void);

#endif 

