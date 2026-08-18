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
 * @file   : task_set_up.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_set_up_attribute.h"
#include "task_set_up_interface.h"
#include "task_normal_attribute.h"
#include "task_normal_interface.h"
#include "task_normal.h"
#include "display.h"

/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI			0ul
#define G_TASK_MEN_TICK_CNT_INI		0ul

#define DEL_MEN_XX_MIN				0ul
#define DEL_MEN_XX_MED				50ul
#define DEL_MEN_XX_MAX				500ul

/********************** internal data declaration ****************************/
task_set_up_dta_t *p_task_set_up_dta;
task_set_up_dta_t task_set_up_dta = {DEL_MEN_XX_MIN, ST_MEN_ASLEEP, EV_SYS_MENU_ITERATE_IDLE, false, 0};

static task_set_up_st_t prev_menu_state; //Variables creadas para saber cuándo actualizar el display.
static uint32_t prev_option_index;

const uint32_t selectable_ticks[] = {2000, 3000, 4000, 5000, 6000};
const uint32_t selectable_passenger_amount[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const task_set_up_st_t selectable_submenu[] = {ST_MEN_MODIFY_PARAM_TICK, ST_MEN_MODIFY_PARAM_PASSENGER_AMOUNT, ST_MEN_ASLEEP};

#define INITIAL_OPTION_VALUE 0xFFFFFFFF;
#define SET_UP_DTA_QTY	(sizeof(task_set_up_dta)/sizeof(task_set_up_dta_t))
#define SELECTABLE_SUBMENU_QTY (sizeof(selectable_submenu) / sizeof(selectable_submenu[0]))
#define SELECTABLE_TICKS_QTY (sizeof(selectable_ticks) / sizeof(selectable_ticks[0]))
#define SELECTABLE_PASSENGER_AMOUNT_QTY (sizeof(selectable_passenger_amount) / sizeof(selectable_passenger_amount[0]))


/********************** external data declaration ****************************/
extern bool g_is_menu_active;

/********************** internal functions declaration ***********************/
void task_set_up_statechart(void);

void task_set_up_update_display(task_set_up_st_t current_state, uint32_t current_option_index) {
    char linea1[17];
    char linea2[17];

    // Dependiendo del estado del menú se muestra una línea distinta
    switch(current_state) {
        case ST_MEN_ROOT:
            sprintf(linea1, "Menu Principal  ");
            // Se usa el índice para saber en donde está el cursor
            if (current_option_index == 0) {
                sprintf(linea2, ">1.Ajustar Ticks");
            } else if (current_option_index == 1) {
                sprintf(linea2, ">2.Ajustar Pers.");
            } else if (current_option_index == 2) {
                sprintf(linea2, ">3.Salir        ");
            }
            break;

        case ST_MEN_MODIFY_PARAM_TICK:
            sprintf(linea1, "Set: Tiempo (ms)");
            // Accedemos al array constante usando el índice actual
            sprintf(linea2, "Valor: %04lu     ", selectable_ticks[current_option_index]);
            break;

        case ST_MEN_MODIFY_PARAM_PASSENGER_AMOUNT:
            sprintf(linea1, "Set: Personas   ");
            // Accedemos al array constante usando el índice actual[cite: 35]
            sprintf(linea2, "Valor: %02lu       ", selectable_passenger_amount[current_option_index]);
            break;

        default:
            // Para ST_MEN_XX_IDLE u otros estados de transición[cite: 35]
            sprintf(linea1, "                ");
            sprintf(linea2, "                ");
            break;
    }

    // Escribimos en la Línea 1 (Posición X=0, Y=0)[cite: 35]
    displayCharPositionWrite(0, 0);
    LOGGER_LOG("%s\n", linea1);
    displayStringWrite(linea1);

    // Escribimos en la Línea 2 (Posición X=0, Y=1)[cite: 35]
    displayCharPositionWrite(0, 1);
    LOGGER_LOG("%s\n", linea2);
    displayStringWrite(linea2);
}


/********************** internal data definition *****************************/
const char *p_task_set_up 		= "Task Menu (Interactive Menu)";
const char *p_task_set_up_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_set_up_cnt;
volatile uint32_t g_task_set_up_tick_cnt;

/********************** external functions definition ************************/
void task_set_up_init(void *parameters){
	p_task_set_up_dta = &task_set_up_dta;
	task_set_up_st_t	state;
	task_set_up_ev_t	event;
	bool b_event;
	prev_menu_state = ST_MEN_ASLEEP;
	prev_option_index = INITIAL_OPTION_VALUE;

	/* Print out: Task Initialized */
	LOGGER_LOG(" ");
	LOGGER_LOG("  %s is running - %s", GET_NAME(task_set_up_init), p_task_set_up);
	LOGGER_LOG("  %s is a %s\n", GET_NAME(task_set_up), p_task_set_up_);

	/* Init & Print out: Task execution counter */
	g_task_set_up_cnt = G_TASK_MEN_CNT_INI;
	LOGGER_LOG("   %s = %lu\n", GET_NAME(g_task_set_up_cnt), g_task_set_up_cnt);

	init_queue_event_task_set_up();


	/* Init & Print out: Task execution FSM */
	state = ST_MEN_ASLEEP;
	p_task_set_up_dta->state = state;

	event = EV_SYS_MENU_ITERATE_IDLE;
	p_task_set_up_dta->event = event;

	b_event = false;
	p_task_set_up_dta->flag = b_event;

	LOGGER_LOG(" ");
	LOGGER_LOG("   %s = %lu   %s = %lu   %s = %s", GET_NAME(state), (uint32_t)state, GET_NAME(event), (uint32_t)event, GET_NAME(b_event), (b_event ? "true" : "false"));
}

void task_set_up_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_MEN_TICK_CNT_INI < g_task_set_up_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_set_up_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_set_up_cnt++;

		/* Run Task Menu Statechart */
    	task_set_up_statechart();
    	g_is_menu_active = (p_task_set_up_dta->state == ST_MEN_ASLEEP) ?  false : true;
    	if ((p_task_set_up_dta->state != prev_menu_state) || (p_task_set_up_dta->option_index != prev_option_index)) {
    	if (g_is_menu_active == true) { // Si el menú está activo, se actualiza el LCD
    		// Se limpia la pantalla completa antes de reescribir para evitar basura visual
    		task_set_up_update_display(p_task_set_up_dta->state, p_task_set_up_dta->option_index);
    	}
    	prev_menu_state = p_task_set_up_dta->state;
    	prev_option_index = p_task_set_up_dta->option_index;
    	}

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_MEN_TICK_CNT_INI < g_task_set_up_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_set_up_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
	}
}

void task_set_up_statechart(void)
{
	if (true == any_event_task_set_up()){
		p_task_set_up_dta->flag = true;
		p_task_set_up_dta->event = get_event_task_set_up();
	}

	if (((uint32_t) p_task_set_up_dta->flag == true) && ((uint32_t) p_task_set_up_dta->event == EV_SYS_MENU_SLEEP_ACTIVE)){
	    p_task_set_up_dta->flag = false;           // Consumimos el evento
	    p_task_set_up_dta->state = ST_MEN_ASLEEP; // Forzamos al menú a irse a dormir
	    // Aquí se puede limpiar display si es necesario
	}

	switch (p_task_set_up_dta->state){
		case ST_MEN_ASLEEP:
			if (((uint32_t) p_task_set_up_dta->flag) == true && ((uint32_t) p_task_set_up_dta->event == EV_SYS_MENU_WAKEUP_ACTIVE)){
				p_task_set_up_dta->flag = false;
				p_task_set_up_dta->state = ST_MEN_ROOT; // Despierta y va al menú principal
                p_task_set_up_dta->option_index = 0;    // Inicializa el cursor en la primera opción
			}
			break;

		case ST_MEN_ROOT:
			if ((p_task_set_up_dta->flag == true) && (p_task_set_up_dta->event == EV_SYS_MENU_ITERATE_ACTIVE)) { // Si presiona el botón NEXT/ITERATE
				p_task_set_up_dta->flag = false;
				p_task_set_up_dta->option_index = ((p_task_set_up_dta->option_index+=1) % SELECTABLE_SUBMENU_QTY);
			}
            else if ((p_task_set_up_dta->flag == true) && (p_task_set_up_dta->event == EV_SYS_MENU_ACCEPT_ACTIVE)) { // Si presiona el botón ACCEPT
            	p_task_set_up_dta->flag = false;
            	if (p_task_set_up_dta->option_index < SELECTABLE_SUBMENU_QTY-1){
            		p_task_set_up_dta->state = selectable_submenu[p_task_set_up_dta->option_index];
            	}
            	else{
            		put_event_task_set_up(EV_SYS_MENU_SLEEP_ACTIVE);
            		put_event_task_system(EV_SYS_SYSTEM_ACTIVATE_ACTIVE);
            	}
            	p_task_set_up_dta->option_index = 0; // Se reinicia el cursor para el submenú
			}
			break;

         case ST_MEN_MODIFY_PARAM_TICK:
        	 if ((p_task_set_up_dta->flag == true) && ((p_task_set_up_dta->event == EV_SYS_MENU_ITERATE_ACTIVE))){
        		 p_task_set_up_dta->flag = false;
        		 p_task_set_up_dta->option_index = ((p_task_set_up_dta->option_index+=1) % SELECTABLE_TICKS_QTY);
        	 }
        	 else if ((p_task_set_up_dta->flag == true) && (p_task_set_up_dta->event == EV_SYS_MENU_ACCEPT_ACTIVE)){
        		 p_task_set_up_dta->flag = false;
        		 tick_until_stop = selectable_ticks[p_task_set_up_dta->option_index];
        		 p_task_set_up_dta->state = ST_MEN_ROOT;
        		 p_task_set_up_dta->option_index = 0;
        	 }
             break;

         case ST_MEN_MODIFY_PARAM_PASSENGER_AMOUNT:
        	 if ((p_task_set_up_dta->flag == true) && ((p_task_set_up_dta->event == EV_SYS_MENU_ITERATE_ACTIVE))){
        		 p_task_set_up_dta->flag = false;
        		 p_task_set_up_dta->option_index = ((p_task_set_up_dta->option_index+=1) % SELECTABLE_PASSENGER_AMOUNT_QTY);
        	 }
        	 else if ((p_task_set_up_dta->flag == true) && (p_task_set_up_dta->event == EV_SYS_MENU_ACCEPT_ACTIVE)){
        		 p_task_set_up_dta->flag = false;
        		 passenger_amount = selectable_passenger_amount[p_task_set_up_dta->option_index];
        		 p_task_set_up_dta->state = ST_MEN_ROOT;
        		 p_task_set_up_dta->option_index = 0;
        	 }
        	 break;

		    default:
			    p_task_set_up_dta->tick  = DEL_MEN_XX_MIN;
			    p_task_set_up_dta->state = ST_MEN_ASLEEP;
			    p_task_set_up_dta->event = EV_SYS_MENU_ITERATE_IDLE;
			    p_task_set_up_dta->flag  = false;
			    break;
	}
}

/********************** end of file ******************************************/
