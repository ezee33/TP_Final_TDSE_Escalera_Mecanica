#ifndef TASK_INC_TASK_SYSTEM_INTERFACE_H_
#define TASK_INC_TASK_SYSTEM_INTERFACE_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdbool.h>

/********************** external functions declaration ***********************/
extern void init_queue_event_task_system(void);
extern void put_event_task_system(task_system_ev_t event);
extern task_system_ev_t get_event_task_system(void);
extern bool any_event_task_system(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_NORMAL_INTERFACE_H_ */

/********************** end of file ******************************************/
