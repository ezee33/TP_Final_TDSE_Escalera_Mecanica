#ifndef TASK_INC_TASK_SENSOR_H_
#define TASK_INC_TASK_SENSOR_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** external data declaration ****************************/
extern uint32_t g_task_sensor_cnt;
extern volatile uint32_t g_task_sensor_tick_cnt;

/********************** external functions declaration ***********************/
void task_sensor_init(void *parameters);
void task_sensor_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_H_ */

/********************** end of file ******************************************/
