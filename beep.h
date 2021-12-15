#ifndef		__BEEP_H__
#define		__BEEP_H__

// 定义蜂鸣器响的电平
#define BEEP_RING_LEVEL    GPIO_LEVEL_HIGH 

#define BEEP_RING_TIMES_1    1
#define BEEP_RING_TIMES_2    2
#define BEEP_RING_TIMES_3    3
#define BEEP_RING_TIMES_4    4
#define BEEP_RING_TIMES_5    5
#define BEEP_RING_FOREVER    0xFF

void beep_init(void);
void beep_loop(void);
void beep_run_tick(uint8_t base);
void beep_bind_gpio(gpio_num_en num);
void beep_ring_set(uint16_t on_time, uint16_t off_time, uint8_t times);


#endif