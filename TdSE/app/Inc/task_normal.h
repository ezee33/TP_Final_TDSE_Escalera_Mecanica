#ifndef TASK_INC_TASK_SYSTEM_H_
#define TASK_INC_TASK_SYSTEM_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** external data declaration ****************************/
extern uint32_t g_task_system_cnt;
extern volatile uint32_t g_task_system_tick_cnt;
extern uint32_t TRESHOLD_MAX_AMOUNT_PASSENGERS;
extern uint32_t TRESHOLD_MIN_AMOUNT_PASSENGERS;
extern uint32_t tick_until_stop;
extern uint32_t passenger_amount;

/********************** external functions declaration ***********************/
extern void task_system_init(void *parameters);
extern void task_system_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_NORMAL_H_ */

/********************** end of file ******************************************/
