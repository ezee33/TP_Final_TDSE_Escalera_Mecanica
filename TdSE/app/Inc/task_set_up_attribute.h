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
 * @file   : task_set_up_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_SET_UP_ATTRIBUTE_H_
#define TASK_INC_TASK_SET_UP_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/* Events to excite Task Menu */
typedef enum task_set_up_ev {
							 EV_SYS_MENU_ITERATE_IDLE,
							 EV_SYS_MENU_ITERATE_ACTIVE,
							 EV_SYS_MENU_ACCEPT_IDLE,
							 EV_SYS_MENU_ACCEPT_ACTIVE,
							 EV_SYS_MENU_WAKEUP_IDLE,
							 EV_SYS_MENU_WAKEUP_ACTIVE,
							 EV_SYS_MENU_SLEEP_IDLE,
							 EV_SYS_MENU_SLEEP_ACTIVE} task_set_up_ev_t;

/* State of Task Menu */
typedef enum task_set_up_st {ST_MEN_ASLEEP = 80,
							 ST_MEN_ROOT,
							 ST_MEN_MODIFY_PARAM_TICK,
							 ST_MEN_MODIFY_PARAM_PASSENGER_AMOUNT} task_set_up_st_t;

typedef struct
{
	uint32_t		tick;
	task_set_up_st_t	state;
	task_set_up_ev_t	event;
	bool			flag;
	uint8_t option_index;
} task_set_up_dta_t;

/********************** external data declaration ****************************/
extern task_set_up_dta_t task_set_up_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SET_UP_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
