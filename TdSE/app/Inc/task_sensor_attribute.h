#ifndef TASK_INC_TASK_SENSOR_ATTRIBUTE_H_
#define TASK_INC_TASK_SENSOR_ATTRIBUTE_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "main.h"

/********************** typedef **********************************************/
typedef enum task_sensor_ev {EV_SENSOR_XX_UP,
							 EV_SENSOR_XX_DOWN} task_sensor_ev_t;

typedef enum task_sensor_st {ST_SENSOR_XX_UP,
							 ST_SENSOR_XX_FALLING,
							 ST_SENSOR_XX_DOWN,
						     ST_SENSOR_XX_RISING} task_sensor_st_t;

typedef enum task_sensor_id {ID_BTN_PERSON_IN,
							 ID_BTN_PERSON_OUT,
							 ID_BTN_SYSTEM_ACTIVATE,
							 ID_BTN_MENU_ITERATE,
							 ID_BTN_MENU_ACCEPT,
							 ID_SWC_IR_BARRIER,
							 ID_SWC_SYSTEM_DEACTIVATE} task_sensor_id_t;

typedef struct {
	task_sensor_id_t	identifier;
	GPIO_TypeDef *		gpio_port;
	uint16_t			pin;
	GPIO_PinState		pressed;
	uint32_t			tick_max;
	task_sensor_ev_t	signal_up;
	task_sensor_ev_t	signal_down;
} task_sensor_cfg_t;

typedef struct {
	uint32_t			tick;
	task_sensor_st_t	state;
	task_sensor_ev_t	event;
} task_sensor_dta_t;

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
