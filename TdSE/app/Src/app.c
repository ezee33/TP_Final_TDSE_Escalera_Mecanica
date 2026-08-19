/********************** inclusions *******************************************/
#include <app.h>
#include "task_set_up.h"
#include "task_normal.h"
#include "task_actuator.h"
#include "task_temperature.h"
#include "task_sensor.h"
#include "dwt.h"
#include "logger.h"

/********************** macros and definitions *******************************/
#define G_APP_CNT_INI		0ul
#define G_APP_TICK_CNT_INI	0ul

#define TASK_X_WCET_INI		0ul
#define TASK_X_DELAY_MIN	0ul

/****************************** typedef **************************************/
typedef struct {
	void (*task_init)(void *);		// Pointer to task (must be a
									// 'void (void *)' function)
	void (*task_update)(void *);	// Pointer to task (must be a
									// 'void (void *)' function)
	void *parameters;				// Pointer to parameters
} task_cfg_t;

typedef struct {
    uint32_t WCET;	      			// Worst-case execution time (microseconds)
} task_dta_t;

/********************** internal data definition *****************************/
const char *p_sys	= " Bare Metal - Event-Triggered Systems (ETS)\r\n";
const char *p_app	= " App - Model Integration\r\n";

/********************** internal data declaration ****************************/
uint32_t g_app_cnt;
uint32_t g_app_time_us;
bool g_is_menu_active;

volatile uint32_t g_app_tick_cnt;

const task_cfg_t task_cfg_list[]	= {
		{task_sensor_init, 		task_sensor_update, 	NULL},
		{task_system_init, 		task_system_update, 	NULL},
		{task_actuator_init,	task_actuator_update, 	NULL},
		{task_set_up_init,      task_set_up_update,     NULL},
};

#define TASK_QTY    (sizeof(task_cfg_list)/sizeof(task_cfg_t))

task_dta_t task_dta_list[TASK_QTY];

/********************** internal functions definition ************************/
void app_init(void){
	uint32_t index;
	g_is_menu_active = false;
	temperatureSensorInit();

	//LOGGER_LOG("\r\n");
	//LOGGER_LOG("%s is running - Tick [mS] = %d\r\n", GET_NAME(app_init), (int)HAL_GetTick());

	//LOGGER_LOG(p_sys);
	//LOGGER_LOG(p_app);

	g_app_cnt = G_APP_CNT_INI;

	//LOGGER_LOG(" %s = %d\r\n", GET_NAME(g_app_cnt), (int)g_app_cnt);

	for (index = 0; TASK_QTY > index; index++){
		(*task_cfg_list[index].task_init)(task_cfg_list[index].parameters);

		task_dta_list[index].WCET = TASK_X_WCET_INI;
	}
	cycle_counter_init();
}

void app_update(void){
	uint32_t index;
	uint32_t cycle_counter;
	uint32_t cycle_counter_time_us;
	temperatureSensorUpdate();

	if (G_APP_TICK_CNT_INI < g_app_tick_cnt){
    	g_app_tick_cnt--;

    	g_app_cnt++;
    	g_app_time_us = 0;

    	for (index = 0; TASK_QTY > index; index++){
			cycle_counter_reset();

			(*task_cfg_list[index].task_update)(task_cfg_list[index].parameters);

			cycle_counter = cycle_counter_get();
			cycle_counter_time_us = cycle_counter_time_us();

	    	g_app_time_us += cycle_counter_time_us;

			if (task_dta_list[index].WCET < cycle_counter_time_us){
				task_dta_list[index].WCET = cycle_counter_time_us;
			}
	    }
    	//LOGGER_LOG("Tiempo de ejecución %lu\n", g_app_time_us);
    }
}

void HAL_SYSTICK_Callback(void)
{
	g_app_tick_cnt++;
	g_task_sensor_tick_cnt++;
	g_task_system_tick_cnt++;
	g_task_actuator_tick_cnt++;
	g_task_set_up_tick_cnt++;
}

/********************** end of file ******************************************/
