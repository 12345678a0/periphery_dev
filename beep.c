#include "gpio.h"
#include "beep.h"

#ifndef BEEP_ON_LEVEL
#define BEEP_ON_LEVEL     GPIO_LEVEL_HIGH
#endif

#ifndef BEEP_OFF_LEVEL
#define BEEP_OFF_LEVEL    GPIO_LEVEL_LOW
#endif

#define BEEP_ON_STATE   1
#define BEEP_OFF_STATE  0


typedef struct _tag_beep_obj_st {
	/* 绑定GPIO */
	struct {
		uint8_t enable;		
		gpio_num_en gpio_num;			
	} bind;
	/* 蜂鸣器 响 */
	struct {
		uint16_t times;     // 闪烁次数
		uint8_t state;      // 灯状态 
		uint16_t tick;      // 闪烁时钟 
		uint16_t on_time;   // 打开（响）时间 
		uint16_t off_time;  // 关闭（不响）时间
	} ring;
} beep_obj_st;

/* beep对象定义 */
static beep_obj_st g_beep_obj;

void beep_on()
{
	if (g_beep_obj.bind.enable)
	{
		g_beep_obj.ring.state = BEEP_ON_STATE;
		gpio_level_set(g_beep_obj.bind.gpio_num, BEEP_ON_LEVEL);	
	}
}

void beep_off()
{
	if (g_beep_obj.bind.enable)
	{
		g_beep_obj.ring.state = BEEP_OFF_STATE;
		gpio_level_set(g_beep_obj.bind.gpio_num, BEEP_OFF_LEVEL);	
	}
}

void beep_bind_gpio(gpio_num_en num)
{
	g_beep_obj.bind.enable = true;
	g_beep_obj.bind.gpio_num = num; 
}
  
void beep_ring_set(uint16_t on_time, uint16_t off_time, uint8_t times)
{
	g_beep_obj.ring.on_time = on_time;
	g_beep_obj.ring.off_time = off_time;
	g_beep_obj.ring.times = 2 * times;
	g_beep_obj.ring.tick = 0;
	//g_beep_obj.ring.state = BEEP_OFF_STATE;
}

void beep_run_tick(uint8_t base)
{
	g_beep_obj.ring.tick += base;
}


void beep_handle_sub()
{
	if (g_beep_obj.ring.state == BEEP_ON_STATE && g_beep_obj.ring.tick >= g_beep_obj.ring.on_time)
	{
		g_beep_obj.ring.tick = 0;
		
		if (g_beep_obj.ring.times  < 2 * 0xFF && g_beep_obj.ring.times != 0x00)
		{
			beep_off();
			g_beep_obj.ring.times --;
		}
		else if (g_beep_obj.ring.times == 0xFF)
		{
			beep_off();
		}
	}
	else if (g_beep_obj.ring.state == BEEP_OFF_STATE && g_beep_obj.ring.tick >= g_beep_obj.ring.off_time)
	{
		g_beep_obj.ring.tick = 0;
		
		if (g_beep_obj.ring.times >= 2 * 0xFF && g_beep_obj.ring.times != 0x00)
		{
			beep_on();
			g_beep_obj.ring.times --;
		}
		else if (g_beep_obj.ring.times == 0xFF)
		{
			beep_on();
		}
	}		
}

void beep_handle()
{
	if (g_beep_obj.bind.enable)
	{
		beep_handle_sub();
	}
}




