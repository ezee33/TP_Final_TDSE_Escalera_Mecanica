Description:
 Bare Metal - Event-Triggered & Time-Triggered Cooperative Multitasking System
 Escalator Control & Monitoring System (Sistema de Control de Escalera Mecánica)
 Project for STM32 Microcontroller (STM32CubeIDE)

 SystemCoreClock     => 64MHz (15.625nS)
 SysTick Rate Hertz  => 1000 ticks per second (1mS)

Modules Summary:

 app.c (app.h)
   Endless loop architecture executing non-blocking cooperative tasks sequentially.

 task_sensor.c (task_sensor.h, task_sensor_attribute.h) 
   Non-Blocking & Update By Time Code -> Digital Inputs & Debouncing Modeling (7 inputs).

 task_temperature.c (task_temperature.h, task_temperature_attribute.h)
   Periodic Signal Conditioning -> Dual Channel ADC Driver (LM35 & Internal Temp Sensor).

 task_system.c (task_system.h, task_system_attribute.h) 
   Event-Driven FSM -> Main Escalator Normal Operation & State Machine.

 task_set_up.c (task_set_up.h, task_set_up_attribute.h)
   Event-Driven FSM -> Interactive Configuration Menu (Passenger count & Standby delay).

 task_actuator.c (task_actuator.h, task_actuator_attribute.h) 
   Non-Blocking & Update By Time Code -> Actuators & Blink Driver (LEDs & Buzzer).

 display.c (display.h)
   Low-Level Hardware Driver -> Alphanumeric LCD HD44780 (4-bit data bus).

Requerimientos de conexión:
 - Display LCD HD44780: Bus de datos de 4 bits (D4-D7) y líneas de control RS y EN.
 - Sensor Térmico LM35: Conectado al canal analógico ADC1_IN4 (Pin PA4).
 - Pulsadores / Switches (Entradas):
     * BTN_PERSON_IN (PB5), BTN_PERSON_OUT (PB4)
     * BTN_SYSTEM_ACTIVATE (PB10), BTN_MENU_ITERATE (PA8), BTN_MENU_ACCEPT (PB3)
     * SWC_IR_BARRIER (PA9), SWC_SYSTEM_DEACTIVATE (PA10)
 - Actuadores (Salidas):
     * LED_MAX_VELOCITY (PB12), LED_MIN_VELOCITY (PB13)
     * LED_SYSTEM_ACTIVATED (PB14), LED_SYSTEM_DEACTIVATED (PB15)
     * BUZZER_ESCALATOR_STOPPED (PA11)

Build procedures:
 Visit the Getting started with STM32: STM32 step-by-step at 
 "https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step"
 to get started building STM32 Projects.