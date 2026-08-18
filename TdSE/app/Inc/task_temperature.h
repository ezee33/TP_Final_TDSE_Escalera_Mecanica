//=====[#include guards - begin]===============================================
#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_

//=====[Inclusions]============================================================
#include <stdint.h>

//=====[Declarations (prototypes) of public functions]=========================
void temperatureSensorInit(void);
void temperatureSensorUpdate(void);

float temperatureSensorReadCelsiusLM35(void);
float temperatureSensorReadCelsiusInternal(void);

float temperatureSensorReadFahrenheitLM35(void);
float celsiusToFahrenheit(float tempInCelsiusDegrees);

//=====[#include guards - end]=================================================
#endif // _TEMPERATURE_SENSOR_H_
