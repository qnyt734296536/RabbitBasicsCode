#include "Hal_Temp_Hum.h"

void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef myGPIO_InitStruct;
    myGPIO_InitStruct.Pin = DHT11_DIN_Pin;
    myGPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    myGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_DIN_GPIO_Port, &myGPIO_InitStruct);
}

void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef myGPIO_InitStruct;
    myGPIO_InitStruct.Pin = DHT11_DIN_Pin;
    myGPIO_InitStruct.Pull = GPIO_PULLUP;
    myGPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    myGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_DIN_GPIO_Port, &myGPIO_InitStruct);
}

/**
*复位设备
*/
static void dht11Rst(void)
{
    DHT11_IO_OUT(); 			            //SET OUTPUT
    DHT11_DQ_OUT_0; 			            
    Delay_Ms(20);    			            //Pull down Least 18ms
    DHT11_DQ_OUT_1; 			           
    Delay_Us(30);     			         //Pull up 20~40us
}

/**
*复位后检查
*/
static uint8_t dht11Check(void)
{
    uint8_t retry=0;
    DHT11_IO_IN();                                              //SET INPUT
    while (DHT11_DQ_IN && (retry<100))                          //DHT11 Pull down 40~80us
    {
        retry++;
        Delay_Us(1);
    }

    if(retry >= 100)
    {
        return 1;
    }
    else
    {
        retry=0;
    }

    while (!DHT11_DQ_IN&& (retry < 100))		    //DHT11 Pull up 40~80us
    {
        retry++;
        Delay_Us(1);
    }

    if(retry >= 100)
    {
        return 1;	                        //check error
    }        

    return 0;
}

static uint8_t dht11ReadBit(void)
{
    uint8_t retry=0;
    while(DHT11_DQ_IN && (retry<100))                           //wait become Low level
    {
        retry++;
        Delay_Us(1);
    }

    retry = 0;
    while(!DHT11_DQ_IN && (retry < 100))		    //wait become High level
    {
        retry++;
        Delay_Us(1);
    }

    Delay_Us(30);//wait 40us

    if(DHT11_DQ_IN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static uint8_t dht11ReadByte(void)
{
    uint8_t i,dat;
    dat=0;
    for (i=0; i<8; i++)
    {
        dat<<=1;
        dat |= dht11ReadBit();
    }

    return dat;
}




uint8_t dht11ReadData(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t buf[5];
    uint8_t i;
    dht11Rst(); 
    if(0 == dht11Check()) 
    {
        for(i=0; i<5; i++)
        {
            buf[i] = dht11ReadByte();
        }
        if(buf[4] == (buf[0]+buf[1]+buf[2]+buf[3]))
        {
            *humidity = buf[0];
            *temperature = buf[2];
        }
    }
    else
    {
        return 1;
    }

    return 0;
}





void Dht11_Test(void)
{
	uint8_t temperature =0,humidity = 0;
	static int count =0;
	if(count ==0)
	{
		dht11ReadData(&temperature, &humidity);
		LOG("temperature=%d,valueHumidity=%d\r\n",temperature,humidity);
	}
	
	count++;
	if(count >3000)
	{
		count =0;
	}

}



