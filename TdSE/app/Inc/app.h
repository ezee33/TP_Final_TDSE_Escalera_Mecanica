#ifndef APP_INC_APP_H_
#define APP_INC_APP_H_

/********************** Start of CPP guard ***********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include "main.h"

/********************** macros ***********************************************/
#define TEST_0 (0)
#define TEST_1 (1)
#define TEST_2 (2)

#define TEST_X (TEST_0)

/********************** external data declaration ****************************/
extern uint32_t g_app_cnt;
extern uint32_t g_app_time_us;
extern volatile uint32_t g_app_tick_cnt;

/********************** external functions declaration ***********************/
void app_init(void);
void app_update(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* APP_INC_APP_H_ */

/********************** end of file ******************************************/
