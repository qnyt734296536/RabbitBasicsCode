#ifndef _HAL_RGB_MY9291_H_
#define _HAL_RGB_MY9291_H_

#include "common.h"

typedef enum my9291_cmd_one_shot_t {
    my9291_cmd_one_shot_disable = 0x00,
    my9291_cmd_one_shot_enforce = 0x01,
} my9291_cmd_one_shot_t;

typedef enum my9291_cmd_reaction_t {
    my9291_cmd_reaction_fast = 0x00,
    my9291_cmd_reaction_slow = 0x01,
} my9291_cmd_reaction_t;

typedef enum my9291_cmd_bit_width_t {
    my9291_cmd_bit_width_16 = 0x00,
    my9291_cmd_bit_width_14 = 0x01,
    my9291_cmd_bit_width_12 = 0x02,
    my9291_cmd_bit_width_8 = 0x03,
} my9291_cmd_bit_width_t;

typedef enum my9291_cmd_frequency_t {
    my9291_cmd_frequency_divide_1 = 0x00,
    my9291_cmd_frequency_divide_4 = 0x01,
    my9291_cmd_frequency_divide_16 = 0x02,
    my9291_cmd_frequency_divide_64 = 0x03,
} my9291_cmd_frequency_t;

typedef enum my9291_cmd_scatter_t {
    my9291_cmd_scatter_apdm = 0x00,
    my9291_cmd_scatter_pwm = 0x01,
} my9291_cmd_scatter_t;

typedef struct my9291_cmd_t {
    my9291_cmd_scatter_t scatter: 1;
    my9291_cmd_frequency_t frequency: 2;
    my9291_cmd_bit_width_t bit_width: 2;
    my9291_cmd_reaction_t reaction: 1;
    my9291_cmd_one_shot_t one_shot: 1;
    uint8_t resv: 1;
} __attribute__((aligned(1), packed)) my9291_cmd_t;

#define MY9291_COMMAND_DEFAULT \
{ \
    scatter = my9291_cmd_scatter_apdm, \
    frequency = my9291_cmd_frequency_divide_1, \
    bit_width = my9291_cmd_bit_width_8, \
    reaction = my9291_cmd_reaction_fast, \
    one_shot = my9291_cmd_one_shot_disable, \
    resv = 0, \
}


void my9291_di_pulse(uint16_t times);
void my9291_dcki_pulse(uint16_t times);
void my9291_send_command(my9291_cmd_t command);
void My9291_Send_Duty(uint16_t duty_r, uint16_t duty_g, uint16_t duty_b, uint16_t duty_w);
void Init_My9291(void);
void Test_Rgb(void);

#endif


