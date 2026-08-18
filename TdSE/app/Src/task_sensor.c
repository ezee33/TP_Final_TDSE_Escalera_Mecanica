/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_sensor.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include <stdbool.h>
#include "app.h"

/* Demo includes. */
#include "task_set_up.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "logger.h"
#include "task_sensor_attribute.h"
#include "task_normal_attribute.h"
#include "task_normal_interface.h"
#include "task_set_up_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_SENSOR_XX_MIN				0ul
#define DEL_SENSOR_XX_MED				25ul
#define DEL_SENSOR_XX_MAX				25ul

/********************** internal data declaration ****************************/
const task_sensor_cfg_t task_sensor_cfg_list[] = {
	{ID_BTN_PERSON_IN,  BTN_PERSON_IN_PORT,  BTN_PERSON_IN_PIN,  BTN_PERSON_IN_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_PERSON_IN_IDLE,  EV_SYS_PERSON_IN_ACTIVE}, //Botón que indica la entrada de una persona
	{ID_BTN_PERSON_OUT,  BTN_PERSON_OUT_PORT,  BTN_PERSON_OUT_PIN,  BTN_PERSON_OUT_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_PERSON_OUT_IDLE,  EV_SYS_PERSON_OUT_ACTIVE}, //Botón que indica la salida de una persona
	{ID_BTN_SYSTEM_ACTIVATE,  BTN_SYSTEM_ACTIVATE_PORT,  BTN_SYSTEM_ACTIVATE_PIN,  BTN_SYSTEM_ACTIVATE_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_SYSTEM_ACTIVATE_IDLE,  EV_SYS_SYSTEM_ACTIVATE_ACTIVE}, //Botón que activa el sistema de contol
	{ID_BTN_MENU_ITERATE,  BTN_MENU_ITERATE_PORT,  BTN_MENU_ITERATE_PIN,  BTN_MENU_ITERATE_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_MENU_ITERATE_IDLE,  EV_SYS_MENU_ITERATE_ACTIVE}, //Botón para iterar en el menú.
	{ID_BTN_MENU_ACCEPT,  BTN_MENU_ACCEPT_PORT,  BTN_MENU_ACCEPT_PIN,  BTN_MENU_ACCEPT_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_MENU_ACCEPT_IDLE,  EV_SYS_MENU_ACCEPT_ACTIVE},// Botón para aceptar en el menú..
	{ID_SWC_IR_BARRIER,  SWC_IR_BARRIER_PORT,  SWC_IR_BARRIER_PIN,  SWC_IR_BARRIER_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_SWC_IR_EMPTY_BARRIER_IDLE,  EV_SYS_SWC_IR_EMPTY_BARRIER_ACTIVE},	// Sensor que detecta si la escalera se vació.
	{ID_SWC_SYSTEM_DEACTIVATE,  SWC_SYSTEM_DEACTIVATE_PORT,  SWC_SYSTEM_DEACTIVATE_PIN,  SWC_SYSTEM_DEACTIVATE_PRESSED, DEL_SENSOR_XX_MAX, EV_SYS_SWC_SYSTEM_DEACTIVATE_IDLE,  EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE},	// Se utiliza para deshabilitar el sistema de control.
	}; // Se definen cada uno de los sensores a utilizar.

#define SENSOR_CFG_QTY	(sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] = {
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},	// BTN_PERSON_IN
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},  // BTN_PERSON_OUT
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},  // BTN_SYSTEM ACTIVE
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},  // BTN_MENU_ITERATE
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},  // BTN_MENU_ACCEPT
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},	// SWC_IR_BARRIER
	{DEL_SENSOR_XX_MIN, ST_SENSOR_XX_UP, EV_SENSOR_XX_UP},	// SWC_SYSTEM_DEACTIVATE
}; // Se definen los parámetros iniciales de cada uno de los sensores a utilizar.

#define SENSOR_DTA_QTY	(sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_sensor 		= "Task Sensor (Sensor Statechart)";
const char *p_task_sensor_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_sensor_cnt;
volatile uint32_t g_task_sensor_tick_cnt;

/********************** external functions definition ************************/
void task_sensor_init(void *parameters)
{
	uint32_t index;
	task_sensor_dta_t *p_task_sensor_dta;
	task_sensor_st_t state;
	task_sensor_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_sensor_init), p_task_sensor);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_sensor), p_task_sensor_);

	g_task_sensor_cnt = G_TASK_SEN_CNT_INIT;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_sensor_cnt), g_task_sensor_cnt);

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_sensor_dta = &task_sensor_dta_list[index];

		/* Print out: Index & Task execution FSM */
		LOGGER_LOG("   %s = %lu", GET_NAME(index), index);

		state = p_task_sensor_dta->state;
		LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

		event = p_task_sensor_dta->event;
		LOGGER_LOG("   %s = %lu\r\n", GET_NAME(event), (uint32_t)event);
	}
	g_task_sensor_tick_cnt = G_TASK_SEN_TICK_CNT_INI;
}

void task_sensor_update(void *parameters)
{
	uint32_t index;									// Valor que indica la posición de la cola circular que se está ejecutando.
	const task_sensor_cfg_t *p_task_sensor_cfg;
	task_sensor_dta_t *p_task_sensor_dta;			// Puntero a la lista con los datos de todos los sensores.
	bool b_time_update_required = false;			// Booleano que indica si hay inserciones en la cola sin actualizar.

	/* Update Task Sensor Counter */
	g_task_sensor_cnt++;

	/* Protect shared resource (g_task_sensor_tick_cnt) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
    {
    	g_task_sensor_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_sensor_tick_cnt) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
		{
			g_task_sensor_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	for (index = 0; SENSOR_DTA_QTY > index; index++)
		{
    		/* Update Task Sensor Configuration & Data Pointer */
			p_task_sensor_cfg = &task_sensor_cfg_list[index];
			p_task_sensor_dta = &task_sensor_dta_list[index];

			if (p_task_sensor_cfg->pressed == HAL_GPIO_ReadPin(p_task_sensor_cfg->gpio_port, p_task_sensor_cfg->pin))
			{
				p_task_sensor_dta->event =	EV_SENSOR_XX_DOWN;
			}
			else
			{
				p_task_sensor_dta->event =	EV_SENSOR_XX_UP;
			}

			switch (p_task_sensor_dta->state)
			{
				case ST_SENSOR_XX_UP:

					if (p_task_sensor_dta->event == EV_SENSOR_XX_DOWN)
					{
						p_task_sensor_dta->tick = DEL_SENSOR_XX_MAX;
						p_task_sensor_dta->state = ST_SENSOR_XX_FALLING;
					}

					break;

				case ST_SENSOR_XX_FALLING:
					if (p_task_sensor_dta->tick > 0){
						p_task_sensor_dta->tick--;
					}

					else if ((p_task_sensor_dta->event == EV_SENSOR_XX_UP) && (p_task_sensor_dta->tick == 0)){
						p_task_sensor_dta->state = ST_SENSOR_XX_UP;
					}

					else if ((p_task_sensor_dta->event == EV_SENSOR_XX_DOWN) && (p_task_sensor_dta->tick == 0)){
						if (((uint32_t)p_task_sensor_cfg->signal_down == (uint32_t)EV_SYS_MENU_ITERATE_ACTIVE) || ((uint32_t)p_task_sensor_cfg->signal_down == (uint32_t)EV_SYS_MENU_ACCEPT_ACTIVE)) { // Se identifica si el evento le pertenece a la tarea del menú
							put_event_task_set_up(p_task_sensor_cfg->signal_down);
						}
						else if ((uint32_t)p_task_sensor_cfg->signal_down == (uint32_t)EV_SYS_SWC_SYSTEM_DEACTIVATE_ACTIVE) {
							put_event_task_system(p_task_sensor_cfg->signal_down);
						    put_event_task_set_up(EV_SYS_MENU_WAKEUP_ACTIVE); //Se activa el modo set up y se encola en el menú (se desactivó el sistema de control)
						}

						else if (((uint32_t)p_task_sensor_cfg->signal_down == (uint32_t) EV_SYS_SYSTEM_ACTIVATE_ACTIVE)){
							put_event_task_system(p_task_sensor_cfg->signal_down);
						    put_event_task_set_up(EV_SYS_MENU_SLEEP_ACTIVE);
						}

						else {
						    put_event_task_system(p_task_sensor_cfg->signal_down);
						}

						p_task_sensor_dta->state = ST_SENSOR_XX_DOWN;
					}
					break;

				case ST_SENSOR_XX_DOWN:

					if (p_task_sensor_dta->event == EV_SENSOR_XX_UP)
					{
						p_task_sensor_dta->tick = DEL_SENSOR_XX_MAX;
						p_task_sensor_dta->state = ST_SENSOR_XX_RISING;
					}
					break;

				case ST_SENSOR_XX_RISING:
					if (p_task_sensor_dta->tick > 0){
						p_task_sensor_dta->tick--;
					}
					else if ((p_task_sensor_dta->event == EV_SENSOR_XX_UP) && (p_task_sensor_dta->tick == 0)){

						// Se filtra si el evento al soltar pertenece al menú
						if (((uint32_t)p_task_sensor_cfg->signal_up == (uint32_t)EV_SYS_MENU_ITERATE_IDLE) || ((uint32_t)p_task_sensor_cfg->signal_up == (uint32_t)EV_SYS_MENU_ACCEPT_IDLE)) {
							put_event_task_set_up(p_task_sensor_cfg->signal_up);
						}
						// Si no es del menú, va al sistema
						else {
							put_event_task_system(p_task_sensor_cfg->signal_up);
						}
						p_task_sensor_dta->state = ST_SENSOR_XX_UP;
					}
					else if ((p_task_sensor_dta->event == EV_SENSOR_XX_DOWN) && (p_task_sensor_dta->tick == 0)){
						p_task_sensor_dta->state = ST_SENSOR_XX_DOWN;
					}
					break;

				default:

					p_task_sensor_dta->tick  = DEL_SENSOR_XX_MIN;
					p_task_sensor_dta->state = ST_SENSOR_XX_UP;
					p_task_sensor_dta->event = EV_SENSOR_XX_UP;

					break;
			}
		}
    }
}

/********************** end of file ******************************************/
