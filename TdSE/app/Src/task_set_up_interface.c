/********************** inclusions *******************************************/
#include "main.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "app.h"
#include "task_set_up_attribute.h"

/********************** macros and definitions *******************************/
#define EVENT_UNDEFINED	(255)
#define MAX_EVENTS		(16)

/********************** internal data definition *****************************/
struct{
	uint32_t			head;
	uint32_t			tail;
	uint32_t			count;
	task_set_up_ev_t	queue[MAX_EVENTS];
} queue_task_a_normal;

/********************** external functions definition ************************/
void init_queue_event_task_set_up(void){
	uint32_t i;

	queue_task_a_normal.head = 0;
	queue_task_a_normal.tail = 0;
	queue_task_a_normal.count = 0;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_task_a_normal.queue[i] = EVENT_UNDEFINED;
}

void put_event_task_set_up(task_set_up_ev_t event){
	queue_task_a_normal.count++;
	queue_task_a_normal.queue[queue_task_a_normal.head++] = event;

	if (MAX_EVENTS == queue_task_a_normal.head)
		queue_task_a_normal.head = 0;
}

task_set_up_ev_t get_event_task_set_up(void){
	task_set_up_ev_t event;

	queue_task_a_normal.count--;
	event = queue_task_a_normal.queue[queue_task_a_normal.tail];
	queue_task_a_normal.queue[queue_task_a_normal.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_task_a_normal.tail)
		queue_task_a_normal.tail = 0;

	return event;
}

bool any_event_task_set_up(void){
  return (queue_task_a_normal.head != queue_task_a_normal.tail);
}

/********************** end of file ******************************************/
