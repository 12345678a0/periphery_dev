#ifndef		__BUTTON_H__
#define		__BUTTON_H__

// 定义双击间隔的时间
//#define BUTTON_DBCLIK_TIME_INTERVAL    800

//定义按键滤波时间
//#define BUTTON_FILT_TIME    50

// 定义按键按下时的有效电平
//#define BUTTON_PRESS_LEVEL     GPIO_LEVEL_LOW 

//#define configUSE_BUTTON_HOOK 
/*定义configUSE_BUTTON_HOOK为1, 启用钩子函数void button_event_hook(button_id_en id, button_event_en event, uint16_t presstime)
 *定义为0表示不启用。当启用时,需在外部定义函数, 参考示例如下：
void button_event_hook(button_id_en id, button_event_en event, uint16_t presstime)
{
	if(id == BUTTON_ID_1)
	{
		switch (event)
		{
			case BUTTON_EVENT_DBCLIK:
				break;
		} 		
	}
}
 */

typedef enum _tag_button_id_en {
	BUTTON_ID_0,
	BUTTON_ID_1,
	BUTTON_ID_2,
	
	BUTTON_ID_MAX 
} button_id_en;

typedef enum _tag_button_event_en {
	BUTTON_EVENT_DOWN_PRESS,   
	BUTTON_EVENT_UP_PRESS,
	BUTTON_EVENT_DBCLIK,
	BUTTON_EVENT_SECOND_COUNT	
} button_event_en;

void button_init(void);
void button_loop(void);
void button_run_tick(uint8_t base); 	
void button_bind_gpio(button_id_en id, gpio_num_en num);
void button_event_register_cb(void (*button_event_cb)(button_id_en, button_event_en, uint16_t));
void button_event_unregister_cb(void);

#endif

