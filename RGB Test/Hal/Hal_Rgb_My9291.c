#include "Hal_Rgb_My9291.h"

/******************************************************************************
 * Copyright 2015 Ai-Thinker Co.,Ltd.
 *
 * FileName: my9291.c
 *
 * Description: MY9291 Driver
 *
 * Modification history:
 *     2015/09/10, v1.0 create this file.
*******************************************************************************/
#include "Hal_Rgb_My9291.h"

#define RGB_DI_PIN GPIO_PIN_9 //DI PB9
#define RGB_DI_GPIO_TYPE GPIOB

#define RGB_DCLK_PIN GPIO_PIN_8 //DCLK PB8
#define RGB_DCLK_GPIO_TYPE GPIOB

my9291_cmd_t my9291_commands[1];
#define PIN_DCKI    0


void my9291_di_pulse(uint16_t times)
{
    uint16_t i;
    for (i = 0; i < times; i++)
    {
			HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_SET);
      HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
    }
}

void my9291_dcki_pulse(uint16_t times)
{
    uint16_t i;
    for (i = 0; i < times; i++)
    {	
			HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_RESET);
    }
}

//-----------------------------------------------------------------------------
// Command Data
//-----------------------------------------------------------------------------
void my9291_send_command(my9291_cmd_t command)
{
    uint8_t i;
    uint8_t command_data = *(uint8_t *)(&command);
    my9291_commands[PIN_DCKI] = command;


    Delay_Us(12);

    my9291_di_pulse(12);

    Delay_Us(12);


    for (i = 0; i < 4; i++)
    {
        // DCK = 0;
        HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_RESET);
        if (command_data & 0x80)
        {
            // DI = 1;
            HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_SET);
        }
        else
        {
            // DI = 0;
            HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
        }
        // DCK = 1;
        HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_SET);
				
        command_data = command_data << 1;
				
        if (command_data & 0x80)
        {
            // DI = 1;
            HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_SET);
        }
        else
        {
            // DI = 0;
            HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
        }
        // DCK = 0;
        HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_RESET);
        // DI = 0;
        HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
        command_data = command_data << 1;
    }

    // TStart > 12us. Delay 12 us.
    Delay_Us(12);
    // Send 16 DI pulse£¬at 14 pulse's falling edge store CMD data, and
    // at 16 pulse's falling edge convert to duty mode.
    my9291_di_pulse(16);
    // TStop > 12us.
    Delay_Us(12);
    // ets_intr_unlock();
}

//-----------------------------------------------------------------------------
// Image Data
//-----------------------------------------------------------------------------
void My9291_Send_Duty(uint16_t duty_r, uint16_t duty_g, uint16_t duty_b, uint16_t duty_w)
{
    uint8_t i = 0;
    uint8_t channel = 0;
    uint8_t bit_length = 8;
    uint16_t duty_current = 0;

    // Definition for RGBW channels
    uint16_t duty[4] = { duty_r, duty_g, duty_b, duty_w };

    switch (my9291_commands[PIN_DCKI].bit_width)
    {
    case my9291_cmd_bit_width_16:
        bit_length = 16;
        break;
    case my9291_cmd_bit_width_14:
        bit_length = 14;
        break;
    case my9291_cmd_bit_width_12:
        bit_length = 12;
        break;
    case my9291_cmd_bit_width_8:
        bit_length = 8;
        break;
    default:
        bit_length = 8;
        break;
    }

    // ets_intr_lock();
    // TStop > 12us.
    Delay_Us(12);

    for (channel = 0; channel < 4; channel++) //RGBW 4CH
    {
        // RGBW Channel
        duty_current = duty[channel];
        // Send 8bit/12bit/14bit/16bit Data
        for (i = 0; i < bit_length / 2; i++)
        {
            // DCK = 0;
            HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_RESET);
            if (duty_current & (0x01 << (bit_length - 1)))
            {
                // DI = 1;
                HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_SET);
            }
            else
            {
                // DI = 0;
                HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
            }
            // DCK = 1;
            HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_SET);
            duty_current = duty_current << 1;
            if (duty_current & (0x01 << (bit_length - 1)))
            {
                // DI = 1;
                HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_SET);
            }
            else
            {
                // DI = 0;
                HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
            }
            //DCK = 0;
            HAL_GPIO_WritePin(RGB_DCLK_GPIO_TYPE, RGB_DCLK_PIN, GPIO_PIN_RESET);
            //DI = 0;
            HAL_GPIO_WritePin(RGB_DI_GPIO_TYPE, RGB_DI_PIN, GPIO_PIN_RESET);
            duty_current = duty_current << 1;
        }
    }

    // TStart > 12us. Ready for send DI pulse.
    Delay_Us(12);
    // Send 8 DI pulse. After 8 pulse falling edge, store old data.
    my9291_di_pulse(8);
    // TStop > 12us.
    Delay_Us(12);
    // ets_intr_unlock();
}

void Init_My9291(void)
{
	my9291_cmd_t command = {
  .scatter = my9291_cmd_scatter_apdm,
  .frequency = my9291_cmd_frequency_divide_4,
  .bit_width = my9291_cmd_bit_width_12,
  .reaction = my9291_cmd_reaction_fast,
  .one_shot = my9291_cmd_one_shot_disable,
  .resv = 0,
  };
  
   my9291_dcki_pulse(64 / 2);
   my9291_send_command(command);
	 My9291_Send_Duty(0,0,0,0);
}



void Test_Rgb(void)
{
		static int count =0;
		if(count ==1)
		{
			My9291_Send_Duty(4095,0,0,0);
		}
		else if(count ==1000)
		{
			My9291_Send_Duty(0,4095,0,0);
		}
		else if(count ==2000)
		{
			My9291_Send_Duty(0,0,4095,0);
		}
		else if(count >4000)
		{
			count =0;
		}
		count++;
}
