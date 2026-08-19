#ifndef TASK_INC_TASK_SET_UP_INTERFACE_H_
#define TASK_INC_TASK_SET_UP_INTERFACE_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_set_up_attribute.h"

/********************** external functions declaration ***********************/
extern void init_queue_event_task_set_up(void);
extern void put_event_task_set_up(task_set_up_ev_t event);
extern task_set_up_ev_t get_event_task_set_up(void);
extern bool any_event_task_set_up(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SET_UP_INTERFACE_H_ */

/********************** end of file ******************************************/
