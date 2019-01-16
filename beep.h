#ifndef		__BEEP_H__
#define		__BEEP_H__

#include "configBeep.h"

#define BEEP_RING_TIMES_1    1
#define BEEP_RING_TIMES_2    2
#define BEEP_RING_TIMES_3    3
#define BEEP_RING_TIMES_4    4
#define BEEP_RING_TIMES_5    5
#define BEEP_RING_FOREVER    0xFF

void beep_run_tick(uint8_t base);
void beep_handle();
void beep_bind_gpio(gpio_num_en num);
void beep_ring_set(uint16_t on_time, uint16_t off_time, uint8_t times);


#endif