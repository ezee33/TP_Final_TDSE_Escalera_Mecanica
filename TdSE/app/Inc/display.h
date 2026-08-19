#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/****************************** inclusions ***********************************/
#include <stdint.h>

/************************ external data declaration **************************/
typedef enum {
     DISPLAY_CONNECTION_GPIO_4BITS,
     DISPLAY_CONNECTION_GPIO_8BITS,
} display_connection_t;

typedef struct {
   display_connection_t connection;
} display_t;

/********************** external functions declaration ***********************/
void displayInit( display_connection_t connection );
void displayCharPositionWrite( uint8_t charPositionX, uint8_t charPositionY );
void displayStringWrite( const char * str );

#endif // _DISPLAY_H_
