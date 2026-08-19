#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_

/*************************** inclusions **************************************/
#include <stdint.h>

/********************** external functions declaration ***********************/
extern void temperatureSensorInit(void);
extern void temperatureSensorUpdate(void);

extern float temperatureSensorReadCelsiusLM35(void);
extern float temperatureSensorReadCelsiusInternal(void);

extern float temperatureSensorReadFahrenheitLM35(void);
extern float celsiusToFahrenheit(float tempInCelsiusDegrees);

#endif // _TEMPERATURE_SENSOR_H_
