#ifndef		__GPIO_H__
#define		__GPIO_H__

typedef enum {
	GPIO_00,
	GPIO_01,
	GPIO_02,

	GPIO_NUM_MAX	
} gpio_num_en;

typedef enum {
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT,
	
	GPIO_MODE_MAX	
} gpio_mode_en;

typedef enum {
	GPIO_LEVEL_LOW,
	GPIO_LEVEL_HIGH
} gpio_level_en;


gpio_level_en gpio_level_get(gpio_num_en gpio_num);
void gpio_level_set(gpio_num_en gpio_num, gpio_level_en level);
void gpio_pullup_set(gpio_num_en gpio_num);
void gpio_pulldown_set(gpio_num_en gpio_num);
void gpio_mode_set(gpio_num_en gpio_num, gpio_mode_en mode);


#endif