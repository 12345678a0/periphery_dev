#ifndef		__CONFIG_BUTTON_H__
#define		__CONFIG_BUTTON_H__

// 定义长按的时间
//#define BUTTON_LONG_TIME               5000 

// 定义双击间隔的时间
//#define BUTTON_DBCLIK_TIME_INTERVAL    800

//定义按键滤波时间
//#define BUTTON_FILT_TIME    50

// 定义按键按下时的有效电平
//#define BUTTON_PRESS_LEVEL     GPIO_LEVEL_LOW 


/*定义configUSE_BUTTON_HOOK为1, 启用钩子函数void button_event_hook(button_id_en id, button_event_en event),
 *定义为0表示不启用。当启用时,需在外部定义函数, 参考示例如下：
void button_event_hook(button_id_en id, button_event_en event)
{
	if(id == BUTTON_ID_1)
	{
		switch (event)
		{
			case BUTTON_EVENT_HOLD_PRESS:
				break;
			case BUTTON_EVENT_SHORT_PRESS:
				break;
			case BUTTON_EVENT_LONG_PRESS:
				break;		
			case BUTTON_EVENT_DBCLIK:
				break;
		} 		
	}
}
 */
#define configUSE_BUTTON_HOOK    1


#endif /* __CONFIG_BUTTON_H__ */