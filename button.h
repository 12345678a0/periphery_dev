#ifndef		__BUTTON_H__
#define		__BUTTON_H__

#include "configButton.h"

typedef enum _tag_button_id_en {
	BUTTON_ID_1,
	BUTTON_ID_2,
	BUTTON_ID_3,
	
	BUTTON_ID_MAX 
} button_id_en;

typedef enum _tag_button_event_en {
	BUTTON_EVENT_HOLD_PRESS,   
	BUTTON_EVENT_SHORT_PRESS,  
	BUTTON_EVENT_LONG_PRESS,   
	BUTTON_EVENT_DBCLIK        
} button_event_en;

void button_handle();
void button_run_tick(uint8_t base); 	
void button_bind_gpio(button_id_en id, gpio_num_en num);
void button_register_cb(void (*button_event_cb)(button_id_en, button_event_en));
void button_unregister_cb();

#endif

