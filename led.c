#include "gpio.h"
#include "led.h"

#ifndef LED_ON_LEVEL
#define LED_ON_LEVEL     GPIO_LEVEL_HIGH
#endif

#ifndef LED_OFF_LEVEL
#define LED_OFF_LEVEL    GPIO_LEVEL_LOW
#endif

#define LED_ON_STATE   1
#define LED_OFF_STATE  0


typedef struct _tag_led_obj_st {
	/* 绑定GPIO */
	struct {
		uint8_t enable;		
		gpio_num_en gpio_num;			
	} bind[LED_ID_MAX];
	/* 闪烁 */
	struct {
		uint8_t flag;       // 闪烁标志
		uint16_t times;     // 闪烁次数
		uint8_t state;      // 灯状态 
		uint16_t tick;      // 闪烁时钟 
		uint16_t interval;  // 闪烁间隔 
	} twinkle[LED_ID_MAX];
} led_obj_st;

/* led对象定义 */
static led_obj_st g_led_obj;


void led_on(led_id_en id)
{
	if (g_led_obj.bind[id].enable)
	{
		g_led_obj.twinkle[id].flag = false;
		g_led_obj.twinkle[id].state = LED_ON_STATE;
		gpio_level_set(g_led_obj.bind[id].gpio_num, LED_ON_LEVEL);	
	}
}

void led_off(led_id_en id)
{
	if (g_led_obj.bind[id].enable)
	{
		g_led_obj.twinkle[id].flag = false;
		g_led_obj.twinkle[id].state = LED_OFF_STATE;
		gpio_level_set(g_led_obj.bind[id].gpio_num, LED_OFF_LEVEL);	
	}
}

void led_bind_gpio(led_id_en id, gpio_num_en num)
{
	g_led_obj.bind[id].enable = true;
	g_led_obj.bind[id].gpio_num = num;  	
}
  
void led_twinkle_set(led_id_en id, uint16_t interval, uint8_t times)
{
	g_led_obj.twinkle[id].flag = true;
	g_led_obj.twinkle[id].interval = interval;
	g_led_obj.twinkle[id].times = 2 * times;
	g_led_obj.twinkle[id].tick = 0;
	//g_led_obj.twinkle[id].state = LED_OFF_STATE;
}

void led_run_tick(uint8_t base)
{
	uint8_t id;
	
	for (id = 0; id < LED_ID_MAX; id ++)
	{
		g_led_obj.twinkle[id].tick += base;	
	}
}


void led_handle_sub(led_id_en id)
{
	if (g_led_obj.twinkle[id].flag == true && g_led_obj.twinkle[id].tick >= g_led_obj.twinkle[id].interval)	
	{
		g_led_obj.twinkle[id].tick = 0;
		
		if (g_led_obj.twinkle[id].times < 2 * 0xFF && g_led_obj.twinkle[id].times != 0x00)
		{
			if (g_led_obj.twinkle[id].state == LED_ON_STATE)
			{
				gpio_level_set(g_led_obj.bind[id].gpio_num, LED_OFF_LEVEL);
				g_led_obj.twinkle[id].state = LED_OFF_STATE;
				g_led_obj.twinkle[id].times	--;			
			}
			else if (g_led_obj.twinkle[id].state == LED_OFF_STATE)
			{
				gpio_level_set(g_led_obj.bind[id].gpio_num, LED_ON_LEVEL);
				g_led_obj.twinkle[id].state = LED_ON_STATE;	
				g_led_obj.twinkle[id].times	--;	
			}		
		}
		else if(g_led_obj.twinkle[id].times >= 2 * 0xFF)
		{
			if (g_led_obj.twinkle[id].state == LED_ON_STATE)
			{
				gpio_level_set(g_led_obj.bind[id].gpio_num, LED_OFF_LEVEL);
				g_led_obj.twinkle[id].state = LED_OFF_STATE;				
			}
			else if (g_led_obj.twinkle[id].state == LED_OFF_STATE)
			{
				gpio_level_set(g_led_obj.bind[id].gpio_num, LED_ON_LEVEL);
				g_led_obj.twinkle[id].state = LED_ON_STATE;	
			}			
		}	
	}
}

void led_handle()
{
	uint8_t id;
	
	for (id = 0; id < LED_ID_MAX; id ++)
	{
		if (g_led_obj.bind[id].enable)
		{
			led_handle_sub(id);
		}
	}
}




