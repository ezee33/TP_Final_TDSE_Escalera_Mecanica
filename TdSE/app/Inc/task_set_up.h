#ifndef TASK_INC_TASK_SET_UP_H_
#define TASK_INC_TASK_SET_UP_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** external data declaration ****************************/
extern uint32_t g_task_set_up_cnt;
extern volatile uint32_t g_task_set_up_tick_cnt;

/********************** external functions declaration ***********************/
extern void task_set_up_init(void *parameters);
extern void task_set_up_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SET_UP_H_ */

/********************** end of file ******************************************/
