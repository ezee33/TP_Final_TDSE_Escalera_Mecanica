#ifndef TASK_INC_TASK_ACTUATOR_ATTRIBUTESH_
#define TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "main.h"
#include <stdbool.h>

/********************** typedef **********************************************/
typedef enum task_actuator_ev {EV_ACTUATOR_XX_OFF = 20,
							   EV_ACTUATOR_XX_ON,
							   EV_ACTUATOR_XX_NOT_BLINK,
							   EV_ACTUATOR_XX_BLINK,
							   EV_ACTUATOR_XX_PULSE} task_actuator_ev_t;

typedef enum task_actuator_st {ST_ACTUATOR_XX_OFF,
							   ST_ACTUATOR_XX_ON,
							   ST_ACTUATOR_XX_BLINK_ON,
							   ST_ACTUATOR_XX_BLINK_OFF} task_actuator_st_t;

typedef enum task_actuator_id {ID_LED_MAX_VELOCITY,
							   ID_LED_MIN_VELOCITY,
							   ID_LED_SYSTEM_ACTIVATED,
							   ID_LED_SYSTEM_DEACTIVATED,
							   ID_BUZZER_ESCALATOR_STOPPED} task_actuator_id_t;

typedef struct{
	task_actuator_id_t	identifier;
	GPIO_TypeDef *		gpio_port;
	uint16_t			pin;
	GPIO_PinState		actuator_on;
	GPIO_PinState		actuator_off;
	uint32_t			tick_blink;
	uint32_t			tick_pulse;
} task_actuator_cfg_t;

typedef struct{
	uint32_t			tick;
	task_actuator_st_t	state;
	task_actuator_ev_t	event;
	bool				flag;
} task_actuator_dta_t;

/********************** external data declaration ****************************/
extern task_actuator_dta_t task_actuator_dta_list[5];

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
