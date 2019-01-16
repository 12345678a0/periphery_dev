#include "gpio.h"
#include "button.h"

#ifndef BUTTON_LONG_TIME
	#define BUTTON_LONG_TIME    3000 
#endif /* BUTTON_LONG_TIME */

#ifndef BUTTON_DBCLIK_TIME_INTERVAL
	#define BUTTON_DBCLIK_TIME_INTERVAL    600
#endif /* BUTTON_DBCLIK_TIME_INTERVAL */

#ifndef BUTTON_FILT_TIME
	#define BUTTON_FILT_TIME    10
#endif /* BUTTON_FILT_TIME */

#ifndef BUTTON_PRESS_LEVEL
	#define BUTTON_PRESS_LEVEL		GPIO_LEVEL_LOW
#endif /* BUTTON_PRESS_LEVEL */

#ifndef configUSE_BUTTON_HOOK
	#error configUSE_BUTTON_HOOK should be defined in configButton.h as either 1 or 0.
#else
	extern void button_event_hook(button_id_en id, button_event_en event);	
#endif /* configUSE_BUTTON_HOOK */
	



/* 按键状态机 FSM(Finite State Machine) 有限状态机 */
typedef enum _tag_button_FSM_en {
	BUTTON_LOOSEN_STATE,         // 松开 
	BUTTON_FILT_STATE,           // 滤波 
	BUTTON_TIMER_START_STATE,    // 计时开始 
	BUTTON_TIMER_ING_STATE,      // 计时中 
	BUTTON_TIMER_OVER_STATE,     // 计时结束 
	BUTTON_WAIT_LOOSEN_STATE,    // 等待松开 
} button_FSM_en;

/* 按键对象 */
typedef struct _tag_button_obj_st {
	/* 绑定GPIO */
	struct {
		uint8_t enable;	
		gpio_num_en gpio_num;	
	} bind[BUTTON_ID_MAX];
	/* 按键次数 */
	uint8_t times[BUTTON_ID_MAX];
	/* 按下时间 单位：ms */
	uint16_t presstime[BUTTON_ID_MAX];
	/* 双击时间 */
	uint16_t dbclik_time[BUTTON_ID_MAX];
	/* 按键事件回调 */
	void (*button_event_cb)(button_id_en, button_event_en);	
} button_obj_st;

/* 按键对象定义 */
static  button_obj_st g_button_obj;

void button_hold_press_handle(button_id_en id)
{
	/* 按住回调 */
	if(g_button_obj.button_event_cb)
	{
		g_button_obj.button_event_cb(id, BUTTON_EVENT_HOLD_PRESS);
	}
	
	#if (configUSE_BUTTON_HOOK == 1)
	{
		button_event_hook(id, BUTTON_EVENT_HOLD_PRESS);
	}
	#endif /* configUSE_BUTTON_HOOK */
}

void button_short_press_handle(button_id_en id)
{
	/* 短按回调 */ 
	if (g_button_obj.button_event_cb)
	{
		g_button_obj.button_event_cb(id, BUTTON_EVENT_SHORT_PRESS);
	}
	
	#if (configUSE_BUTTON_HOOK == 1)
	{
		button_event_hook(id, BUTTON_EVENT_SHORT_PRESS);
	}
	#endif /* configUSE_BUTTON_HOOK */
}

void button_long_press_handle(button_id_en id)
{
	/* 长按回调 */ 
	if (g_button_obj.button_event_cb)
	{
		g_button_obj.button_event_cb(id, BUTTON_EVENT_LONG_PRESS);
	}
	
	#if (configUSE_BUTTON_HOOK == 1)
	{
		button_event_hook(id, BUTTON_EVENT_LONG_PRESS);
	}
	#endif /* configUSE_BUTTON_HOOK */
}

void button_dbclik_press_handle(button_id_en id)
{
	/* 双击回调 */ 
	if (g_button_obj.button_event_cb)
	{
		g_button_obj.button_event_cb(id, BUTTON_EVENT_DBCLIK);
	}
	
	#if (configUSE_BUTTON_HOOK == 1)
	{
		button_event_hook(id, BUTTON_EVENT_DBCLIK);
	}
	#endif /* configUSE_BUTTON_HOOK */
}

void button_times_handle(button_id_en id)
{
	if ((1 == g_button_obj.times[id]) && (g_button_obj.dbclik_time[id] >= BUTTON_DBCLIK_TIME_INTERVAL))
	{
		g_button_obj.times[id] = 0;
		//短按处理
		button_short_press_handle(id);
	}
	else if (g_button_obj.times[id] >= 2)
	{
		g_button_obj.times[id] = 0;
		//双击处理
		button_dbclik_press_handle(id);
	}
}

void button_FSM(button_id_en id)
{
	static button_FSM_en state[BUTTON_ID_MAX] = {BUTTON_LOOSEN_STATE};
	uint8_t button_level = gpio_level_get(g_button_obj.bind[id].gpio_num);

	switch(state[id])
	{
		/* 松开状态 */	
		case BUTTON_LOOSEN_STATE:
			if (button_level == BUTTON_PRESS_LEVEL)
			{
				//清除按下时间
				g_button_obj.presstime[id] = 0;
				state[id] = BUTTON_FILT_STATE; 
			}
			break;
	   /* 滤波消抖状态 */	
		case BUTTON_FILT_STATE: 
			if (g_button_obj.presstime[id] >= BUTTON_FILT_TIME)
			{
				if (button_level == BUTTON_PRESS_LEVEL)
				{
					state[id] = BUTTON_TIMER_START_STATE; 
				}
				else 
				{
					state[id] = BUTTON_LOOSEN_STATE; 
				}
			}				
			break;	
		/* 计时开始状态 */		
		case BUTTON_TIMER_START_STATE: 
			if (button_level == BUTTON_PRESS_LEVEL)
			{
				//清除按下时间
				g_button_obj.presstime[id] = 0;
				state[id] = BUTTON_TIMER_ING_STATE; 	
			}
			else
			{
				state[id] = BUTTON_LOOSEN_STATE; 	
			}
			break;
		/* 计时中状态 */
		case BUTTON_TIMER_ING_STATE:
			if (button_level == BUTTON_PRESS_LEVEL)
			{
				//按住处理		
				button_hold_press_handle(id);
				
				if (g_button_obj.presstime[id] >= BUTTON_LONG_TIME)
				{
					
					state[id] = BUTTON_TIMER_OVER_STATE; 	
				}
				else
				{
					state[id] = BUTTON_TIMER_ING_STATE; 	
				}
			}
			else
			{
				state[id] = BUTTON_TIMER_OVER_STATE; 	
			}
			break;	
		/* 计时结束状态 */	
		case BUTTON_TIMER_OVER_STATE: 	
			if (g_button_obj.presstime[id] >= BUTTON_LONG_TIME)
			{
				//长按处理
				button_long_press_handle(id);
			}
			else
			{
				//按键次数累加
				g_button_obj.times[id] ++;
				/* 清除双击计时 */
				if (1 == g_button_obj.times[id])
				{
					g_button_obj.dbclik_time[id] = 0;
				}
			}
			
			state[id] = BUTTON_WAIT_LOOSEN_STATE; 
			break;	
		/* 等待松开状态 */	
		case BUTTON_WAIT_LOOSEN_STATE:
			if (button_level == BUTTON_PRESS_LEVEL)
			{
				//按住处理		
				button_hold_press_handle(id);
			}
			else
			{
				state[id] = BUTTON_LOOSEN_STATE;
			}
			break;	
	}	
}

void button_register_cb(void (*button_event_cb)(button_id_en, button_event_en))
{
	g_button_obj.button_event_cb = button_event_cb;  		
}

void button_unregister_cb()
{
	g_button_obj.button_event_cb = NULL;  	
}

void button_bind_gpio(button_id_en id, gpio_num_en num)
{
	g_button_obj.bind[id].enable = true;
	g_button_obj.bind[id].gpio_num = num;  	
}

void button_run_tick(uint8_t base) 	
{
	uint8_t id;
		
	for (id = 0; id < BUTTON_ID_MAX; id ++)
	{
		//按下时间
		g_button_obj.presstime[id] += base;
		//双击时间
		g_button_obj.dbclik_time[id] += base; 
	}
}

void button_handle()
{
	uint8_t id;
		
	for (id = 0; id < BUTTON_ID_MAX; id ++)
	{
		if (g_button_obj.bind[id].enable)
		{
			button_FSM(id);
			button_times_handle(id);
		}
	}
}
