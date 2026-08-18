/********************** inclusions *******************************************/
#include "app.h"
#include "logger.h"
#include "board.h"
#include "task_normal_attribute.h"

/********************** macros and definitions *******************************/
#define EVENT_UNDEFINED	(255)
#define MAX_EVENTS		(16)

/********************** internal data definition *****************************/
struct
{
	uint32_t			head;
	uint32_t			tail;
	uint32_t			count;
	task_system_ev_t	queue[MAX_EVENTS];
} queue_task_a;

/********************** external functions definition ************************/
void init_queue_event_task_system(void){
	uint32_t i;

	queue_task_a.head = 0;
	queue_task_a.tail = 0;
	queue_task_a.count = 0;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_task_a.queue[i] = EVENT_UNDEFINED;
}

void put_event_task_system(task_system_ev_t event){
	queue_task_a.count++;
	queue_task_a.queue[queue_task_a.head++] = event;

	if (MAX_EVENTS == queue_task_a.head)
		queue_task_a.head = 0;
}

task_system_ev_t get_event_task_system(void){
	task_system_ev_t event;

	queue_task_a.count--;
	event = queue_task_a.queue[queue_task_a.tail];
	queue_task_a.queue[queue_task_a.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_task_a.tail)
		queue_task_a.tail = 0;

	return event;
}

bool any_event_task_system(void){
  return (queue_task_a.head != queue_task_a.tail);
}

/********************** end of file ******************************************/
