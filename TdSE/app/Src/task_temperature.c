//=====[Libraries]=============================================================
#include "task_temperature.h"
#include "main.h"

//=====[Declaration of private defines]========================================
#define LM35_NUMBER_OF_AVG_SAMPLES    10

//=====[Declaration of external public global variables]=======================
extern ADC_HandleTypeDef hadc1;

//=====[Declaration and initialization of private global variables]============
static float lm35TemperatureC = 0.0f;
static float internalTemperatureC = 0.0f;
static uint16_t lm35ReadingsArray[LM35_NUMBER_OF_AVG_SAMPLES];
static int lm35SampleIndex = 0;

//=====[Declarations (prototypes) of private functions]========================
static float analogReadingScaledWithTheLM35Formula(float analogReading);
static float calculateInternalTemperature(uint16_t analogReading);

//=====[Implementations of public functions]===================================
void temperatureSensorInit(void)
{
    for(int i = 0; i < LM35_NUMBER_OF_AVG_SAMPLES ; i++) {
        lm35ReadingsArray[i] = 0;
    }
    lm35SampleIndex = 0;
}
/*
void temperatureSensorUpdate(void)
{
    // Se inicia el ADC
    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint16_t analog_value = HAL_ADC_GetValue(&hadc1);
        uint16_t adc_lm35_raw = analog_value;
        uint16_t adc_internal_raw = analog_value;

        // LM35
        lm35ReadingsArray[lm35SampleIndex] = adc_lm35_raw;
        lm35SampleIndex = (lm35SampleIndex++) % LM35_NUMBER_OF_AVG_SAMPLES;

        float lm35ReadingsSum = 0.0f;
        for (int i = 0; i < LM35_NUMBER_OF_AVG_SAMPLES; i++) {
            lm35ReadingsSum += (float)lm35ReadingsArray[i];
        }
        float lm35ReadingsAverage = lm35ReadingsSum / LM35_NUMBER_OF_AVG_SAMPLES;

        lm35TemperatureC = analogReadingScaledWithTheLM35Formula(lm35ReadingsAverage);
    }

    // Lectura del sensor interno
    internalTemperatureC = calculateInternalTemperature(adc_internal_raw);

    HAL_ADC_Stop(&hadc1);
}*/

void temperatureSensorUpdate(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // --------------------------------------------------
    // 1. LECTURA DEL LM35
    // --------------------------------------------------
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint16_t adc_lm35_raw = HAL_ADC_GetValue(&hadc1);

        // Promedio móvil LM35
        lm35ReadingsArray[lm35SampleIndex] = adc_lm35_raw;
        lm35SampleIndex++;
        if (lm35SampleIndex >= LM35_NUMBER_OF_AVG_SAMPLES) {
            lm35SampleIndex = 0;
        }

        float lm35ReadingsSum = 0.0f;
        for (int i = 0; i < LM35_NUMBER_OF_AVG_SAMPLES; i++) {
            lm35ReadingsSum += (float)lm35ReadingsArray[i];
        }
        float lm35ReadingsAverage = lm35ReadingsSum / LM35_NUMBER_OF_AVG_SAMPLES;
        lm35TemperatureC = analogReadingScaledWithTheLM35Formula(lm35ReadingsAverage);
    }
    HAL_ADC_Stop(&hadc1);

    // --------------------------------------------------
    // 2. LECTURA DEL SENSOR INTERNO
    // --------------------------------------------------
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // El sensor interno requiere más tiempo de muestreo
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint16_t adc_internal_raw = HAL_ADC_GetValue(&hadc1);
        internalTemperatureC = calculateInternalTemperature(adc_internal_raw);
    }
    HAL_ADC_Stop(&hadc1);
}

float temperatureSensorReadCelsiusLM35(void)
{
    return lm35TemperatureC;
}

float temperatureSensorReadCelsiusInternal(void)
{
    return internalTemperatureC;
}

float temperatureSensorReadFahrenheitLM35(void)
{
    return celsiusToFahrenheit(lm35TemperatureC);
}

float celsiusToFahrenheit(float tempInCelsiusDegrees)
{
    return (tempInCelsiusDegrees * 9.0f / 5.0f) + 32.0f;
}

//=====[Implementations of private functions]==================================
static float analogReadingScaledWithTheLM35Formula(float analogReading)
{
    // El ADC de STM32 es de 12 bits (0 a 4095) y la tensión es de 3.3V
    float voltage = (analogReading * 3.3f) / 4095.0f;
    // El LM35 entrega 10mV por grado Celsius (0.01V/°C)
    return (voltage / 0.01f);
}

static float calculateInternalTemperature(uint16_t analogReading)
{
    // Fórmula del Datasheet del STM32F103 para el sensor interno
    // Temp = ((V25 - V_sense) / Avg_Slope) + 25
    // V25 = 1.43V (típico), Avg_Slope = 4.3 mV/°C = 0.0043 V/°C
    float v_sense = (analogReading * 3.3f) / 4095.0f;
    return ((1.43f - v_sense) / 0.0043f) + 25.0f;
}
