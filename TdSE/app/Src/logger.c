/********************** inclusions *******************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "logger.h"

/********************** external data definition *****************************/
static char logger_msg_buffer_[LOGGER_CONFIG_MAXLEN];
char* const logger_msg = logger_msg_buffer_;
int logger_msg_len;

/********************** external functions definition ************************/

#if LOGGER_CONFIG_USE_SEMIHOSTING == 1
void logger_log_print_(char* const msg){
	printf(msg);
	fflush(stdout);
}
#else
void logger_log_print_(char* const msg){
    return;
}
#endif

/********************** end of file ******************************************/
