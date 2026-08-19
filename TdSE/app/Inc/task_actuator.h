#ifndef TASK_INC_TASK_ACTUATOR_H_
#define TASK_INC_TASK_ACTUATOR_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** external data declaration ****************************/
extern uint32_t g_task_actuator_cnt;
extern volatile uint32_t g_task_actuator_tick_cnt;

/********************** external functions declaration ***********************/
extern void task_actuator_init(void *parameters);
extern void task_actuator_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_ACTUATOR_H_ */

/********************** end of file ******************************************/
