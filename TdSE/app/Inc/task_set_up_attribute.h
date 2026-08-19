#ifndef TASK_INC_TASK_SET_UP_ATTRIBUTE_H_
#define TASK_INC_TASK_SET_UP_ATTRIBUTE_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>

/********************** typedef **********************************************/
typedef enum task_set_up_ev {EV_SYS_MENU_ITERATE_IDLE,
							 EV_SYS_MENU_ITERATE_ACTIVE,
							 EV_SYS_MENU_ACCEPT_IDLE,
							 EV_SYS_MENU_ACCEPT_ACTIVE,
							 EV_SYS_MENU_WAKEUP_IDLE,
							 EV_SYS_MENU_WAKEUP_ACTIVE,
							 EV_SYS_MENU_SLEEP_IDLE,
							 EV_SYS_MENU_SLEEP_ACTIVE} task_set_up_ev_t;

typedef enum task_set_up_st {ST_MEN_ASLEEP = 80,
							 ST_MEN_ROOT,
							 ST_MEN_MODIFY_PARAM_TICK,
							 ST_MEN_MODIFY_PARAM_PASSENGER_AMOUNT} task_set_up_st_t;

typedef struct {
	uint32_t			tick;
	task_set_up_st_t	state;
	task_set_up_ev_t	event;
	bool				flag;
	uint8_t 			option_index;
} task_set_up_dta_t;

/********************** external data declaration ****************************/
extern task_set_up_dta_t task_set_up_dta;

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SET_UP_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
