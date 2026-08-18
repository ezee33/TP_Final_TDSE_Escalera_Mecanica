#ifndef TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_
#define TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif
/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>

/********************** typedef **********************************************/
typedef enum task_system_ev {EV_SYS_PERSON_IN_IDLE = 40,
						 	 EV_SYS_PERSON_IN_ACTIVE,
							 EV_SYS_PERSON_OUT_IDLE,
							 EV_SYS_PERSON_OUT_ACTIVE,
							 EV_SYS_SYSTEM_ACTIVATE_IDLE,
							 EV_SYS_SYSTEM_ACTIVATE_ACTIVE,
							 EV_SYS_SWC_IR_EMPTY_BARRIER_IDLE,
							 EV_SYS_SWC_IR_EMPTY_BARRIER_ACTIVE,
							 EV_SYS_SWC_SYSTEM_DEACTIVATE_IDLE,
							 EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE} task_system_ev_t;

typedef enum task_system_st {ST_SYS_INACTIVE,
							 ST_OP_STOP,
							 ST_OP_MIN_VELOCITY,
							 ST_OP_MAX_VELOCITY,
							 ST_OP_STANDBY} task_system_st_t;

typedef struct
{
	uint32_t			tick;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool				flag;
} task_system_dta_t;

/********************** external data declaration ****************************/
extern task_system_dta_t task_system_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_NORMAL_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
