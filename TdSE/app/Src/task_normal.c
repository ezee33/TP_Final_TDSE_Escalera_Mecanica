#include <stdlib.h>
#include "app.h"
#include "logger.h"
#include "task_normal_attribute.h"
#include "task_normal_interface.h"
#include "display.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"
#include "task_temperature.h"

/********************** macros and definitions *******************************/
#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_XX_MIN				0ul
#define DEL_SYS_XX_MED				50ul
#define DEL_SYS_XX_MAX				5000ul

#define SYSTEM_DTA_QTY	(sizeof(task_system_dta)/sizeof(task_system_dta_t))

#define THRESHOLD_MAX_AMOUNT_PASSENGERS 5
#define THRESHOLD_MIN_AMOUNT_PASSENGERS 1
#define THRESHOLD_MIN_CHANGE_BEFORE_REFRESH 3

/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta = {DEL_SYS_XX_MIN, ST_SYS_INACTIVE, EV_SYS_PERSON_IN_IDLE, false};

static task_system_st_t prev_sys_state;
static uint32_t prev_passenger_amount;
static int prev_temp_int;
static int prev_temp_ext;
static bool temp_int_changed;
static bool temp_ext_changed;
static bool state_changed;
static bool passenger_amount_changed;

/********************** external data declaration ****************************/
extern bool g_is_menu_active;

/********************** internal functions declaration ***********************/

static void task_normal_update_display(task_system_st_t current_state, uint32_t current_passenger_amount,  uint32_t tick_until_stop, int temp_lm35, int temp_uc) {
    char linea1[17]; // 16 caracteres + '\0'
    char linea2[17];
    char state_str[4];
    char tick_str[4];

    // Se determina el string de estado basado en la Máquina de estados
    switch (current_state) {
        case ST_SYS_INACTIVE:      sprintf(state_str, "OFF"); break;
        case ST_OP_STOP:           sprintf(state_str, "STP"); break;
        case ST_OP_MIN_VELOCITY:   sprintf(state_str, "MIN"); break;
        case ST_OP_MAX_VELOCITY:   sprintf(state_str, "MAX"); break;
        case ST_OP_STANDBY:        sprintf(state_str, "STB"); break;
        default:                   sprintf(state_str, "ERR"); break;
    }
    // Se determina el string de tick basado en la Máquina de estados
    switch (tick_until_stop) {
    	case 2000:      		   sprintf(tick_str, "2s"); break;
    	case 3000:				   sprintf(tick_str, "3s"); break;
    	case 4000:                 sprintf(tick_str, "4s"); break;
    	case 5000:                 sprintf(tick_str, "5s"); break;
    	case 6000:                 sprintf(tick_str, "6s"); break;
    	default:                   sprintf(tick_str, "--"); break;
}

    sprintf(linea1, "ST:%s P:%02lu T:%s", state_str, current_passenger_amount, tick_str);
    sprintf(linea2, "TI:%02d°C TE:%02d°C", temp_uc, temp_lm35);

    displayCharPositionWrite(0, 0);
    LOGGER_LOG("%s\n", linea1);
    displayStringWrite(linea1);

    displayCharPositionWrite(0, 1);
    LOGGER_LOG("%s\n", linea2);
    displayStringWrite(linea2);
}


/*************************** operation parameteres ***************************/
uint32_t passenger_amount;
uint32_t tick_until_stop;

/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Statechart)";
const char *p_task_system_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_system_cnt;
volatile uint32_t g_task_system_tick_cnt;

/********************** external functions definition ************************/
void task_system_init(void *parameters){
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;
	passenger_amount = 0;
	tick_until_stop = DEL_SYS_XX_MAX;
	prev_sys_state = ST_SYS_INACTIVE;

	//Valores iniciales exagerados para forzar la sobreescritura
	prev_passenger_amount = 0xFFFFFFFF;
	prev_temp_int = -100;
	prev_temp_ext = -100;

	//LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_system_init), p_task_system);
	//LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_system), p_task_system_);

	g_task_system_cnt = G_TASK_SYS_CNT_INI;

	//LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_system_cnt), g_task_system_cnt);

	init_queue_event_task_system();

	p_task_system_dta = &task_system_dta;
	state = p_task_system_dta->state;

	//LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_system_dta->event;
	//LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_system_dta->flag;
	//LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

	g_task_system_tick_cnt = G_TASK_SYS_TICK_CNT_INI;
}

void task_system_update(void *parameters){
	task_system_dta_t *p_task_system_dta;
	bool b_time_update_required = false;

	g_task_system_cnt++;

	__asm("CPSID i");
    if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt){
    	g_task_system_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");

	while (b_time_update_required){
		__asm("CPSID i");
		if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt){
			g_task_system_tick_cnt--;
			b_time_update_required = true;
		}
		else{
			b_time_update_required = false;
		}
		__asm("CPSIE i");

		p_task_system_dta = &task_system_dta;

		if (any_event_task_system() == true){
			p_task_system_dta->flag = true;
			p_task_system_dta->event = get_event_task_system();
		}

		switch (p_task_system_dta->state){
			case ST_SYS_INACTIVE:
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SYSTEM_ACTIVATE_ACTIVE)){
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_LED_SYSTEM_ACTIVATED);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_SYSTEM_DEACTIVATED);

					if ((passenger_amount != 0) && (passenger_amount <= THRESHOLD_MAX_AMOUNT_PASSENGERS)){
						put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_MIN_VELOCITY);
						p_task_system_dta->state = ST_OP_MIN_VELOCITY;
					}
					else if (passenger_amount > THRESHOLD_MAX_AMOUNT_PASSENGERS){
						put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_MAX_VELOCITY);
						p_task_system_dta->state = ST_OP_MAX_VELOCITY;
					}
					else{
						p_task_system_dta->state = ST_OP_STOP;
					}
				}
				break;

			case ST_OP_STOP:
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE)){
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_BUZZER_ESCALATOR_STOPPED);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_SYSTEM_ACTIVATED);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_SYSTEM_DEACTIVATED);
					p_task_system_dta->state = ST_SYS_INACTIVE;
				}
				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_PERSON_IN_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount++;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_BUZZER_ESCALATOR_STOPPED);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_MIN_VELOCITY);
					p_task_system_dta->state = ST_OP_MIN_VELOCITY;
				}
				break;

			case ST_OP_MIN_VELOCITY:
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE)){
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_SYSTEM_ACTIVATED);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MIN_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_SYSTEM_DEACTIVATED);
					p_task_system_dta->state = ST_SYS_INACTIVE;
				}
				else if ((p_task_system_dta->flag == true) && (passenger_amount < THRESHOLD_MAX_AMOUNT_PASSENGERS) && (p_task_system_dta->event == EV_SYS_PERSON_IN_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount++;
					p_task_system_dta->state = ST_OP_MIN_VELOCITY;
				}
				else if ((p_task_system_dta->flag == true) && (passenger_amount >= THRESHOLD_MAX_AMOUNT_PASSENGERS) && (p_task_system_dta->event == EV_SYS_PERSON_IN_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount++;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MIN_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_MAX_VELOCITY);
					p_task_system_dta->state = ST_OP_MAX_VELOCITY;
				}
				else if ((p_task_system_dta->flag == true) && (passenger_amount > THRESHOLD_MIN_AMOUNT_PASSENGERS) && (p_task_system_dta->event == EV_SYS_PERSON_OUT_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount--;
					p_task_system_dta->state = ST_OP_MIN_VELOCITY;
				}
				else if ((p_task_system_dta->flag == true) && (passenger_amount == THRESHOLD_MIN_AMOUNT_PASSENGERS) && (p_task_system_dta->event == EV_SYS_PERSON_OUT_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount--;
					p_task_system_dta->tick = tick_until_stop;
					p_task_system_dta->state = ST_OP_STANDBY;
				}
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_IR_EMPTY_BARRIER_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount = 0;
					p_task_system_dta->tick = tick_until_stop;
					p_task_system_dta->state = ST_OP_STANDBY;
				}
				break;

			case ST_OP_MAX_VELOCITY:
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE)){
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_SYSTEM_ACTIVATED);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MAX_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_SYSTEM_DEACTIVATED);
					p_task_system_dta->state = ST_SYS_INACTIVE;
				}
				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_PERSON_IN_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount++;
					p_task_system_dta->state = ST_OP_MAX_VELOCITY;
				}
				else if ((p_task_system_dta->flag == true) && (passenger_amount >= THRESHOLD_MAX_AMOUNT_PASSENGERS + 2) && (p_task_system_dta->event == EV_SYS_PERSON_OUT_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount--;
					p_task_system_dta->state = ST_OP_MAX_VELOCITY;
				}
				else if ((p_task_system_dta->flag == true) && (passenger_amount == THRESHOLD_MAX_AMOUNT_PASSENGERS + 1) && (p_task_system_dta->event == EV_SYS_PERSON_OUT_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount--;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MAX_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_MIN_VELOCITY);
					p_task_system_dta->state = ST_OP_MIN_VELOCITY;
				}
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_IR_EMPTY_BARRIER_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount = 0;
					p_task_system_dta->tick = tick_until_stop;
					p_task_system_dta->state = ST_OP_STANDBY;
				}
				break;

			case ST_OP_STANDBY:
				if (p_task_system_dta->tick > 0){
					p_task_system_dta->tick--;
				}
				else if (p_task_system_dta->tick == 0){
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_BUZZER_ESCALATOR_STOPPED);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MIN_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MAX_VELOCITY);
					p_task_system_dta->state = ST_OP_STOP;
				}
				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE)){
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_SYSTEM_ACTIVATED);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MAX_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MIN_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_SYSTEM_DEACTIVATED);
					p_task_system_dta->state = ST_SYS_INACTIVE;
				}
				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_PERSON_IN_ACTIVE)){
					p_task_system_dta->flag = false;
					passenger_amount++;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_MAX_VELOCITY);
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_MIN_VELOCITY);
					p_task_system_dta->state = ST_OP_MIN_VELOCITY;
				}

			default:
				break;
		}

		// Se obtiene la temperatura del LM35 y del sensor interno de temperatura
		int temp_lm35 = (int)temperatureSensorReadCelsiusLM35();
		int temp_uc = (int)temperatureSensorReadCelsiusInternal();

		temp_int_changed = abs(temp_uc - prev_temp_int) > THRESHOLD_MIN_CHANGE_BEFORE_REFRESH;
		temp_ext_changed = abs(temp_lm35 - prev_temp_ext) > THRESHOLD_MIN_CHANGE_BEFORE_REFRESH;
		state_changed = p_task_system_dta->state != prev_sys_state;
		passenger_amount_changed = passenger_amount != prev_passenger_amount;

		// Actualización de la pantalla basada en cambios en los estados previos
		if (((state_changed) || (passenger_amount_changed) || (temp_int_changed) || (temp_ext_changed)) && (g_is_menu_active == false)) {

			task_normal_update_display(p_task_system_dta->state, passenger_amount, tick_until_stop, temp_lm35, temp_uc);

			prev_sys_state = p_task_system_dta->state;
			prev_passenger_amount = passenger_amount;
			if (temp_int_changed) prev_temp_int = temp_uc;
			if (temp_ext_changed) prev_temp_ext = temp_lm35;
		}
	}
}

/********************** end of file ******************************************/
