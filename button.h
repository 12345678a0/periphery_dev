#ifndef		__BUTTON_H__
#define		__BUTTON_H__

#include "configButton.h"

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

void button_handle(void);
void button_run_tick(uint8_t base); 	
void button_bind_gpio(button_id_en id, gpio_num_en num);
void button_event_register_cb(void (*button_event_cb)(button_id_en, button_event_en, uint16_t));
void button_event_unregister_cb(void);

#endif

