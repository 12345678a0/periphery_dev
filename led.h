#ifndef		__LED_H__
#define		__LED_H__

#include "configLed.h"

#define LED_TWINKLE_TIMES_1    1
#define LED_TWINKLE_TIMES_2    2
#define LED_TWINKLE_TIMES_3    3
#define LED_TWINKLE_TIMES_4    4
#define LED_TWINKLE_TIMES_5    5
#define LED_TWINKLE_FOREVER    0xFF


typedef enum {
	LED_ID_1,
	LED_ID_2,
	LED_ID_3,
	
	LED_ID_MAX
} led_id_en;

void led_handle();
void led_run_tick(uint8_t base);
void led_bind_gpio(led_id_en id, gpio_num_en num);
void led_on(led_id_en id);
void led_off(led_id_en id);
void led_twinkle_set(led_id_en id, uint16_t interval, uint8_t times);

#endif

