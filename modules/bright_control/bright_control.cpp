/*
   Pines de placa NUCLEO asociados al RGB LED: PB_4(PWM3/1), PA_0(PWM4/1), y PD_12(PWM2/1).
    
    Este archivo de codigo modifica la intensidad de los 3 leds del LED RGB (Red, Green and Blue) para
    obtener así distintas combinaciones logrando diferentes colores. Esto se logra mediante el uso de
    señales PWM generadas a partir de interrupciones por un ticker al cual se le configura una determinada
    frecuencia. Modificando el Duty Cycle de las señales PWM se logran diferentes intensidades (cuanto mayor
    es el tiempo en ON del PWM, entonces mayor será el brillo en el led).
    
    Arbol de funciones:
    brightControlInit()
        |- setPeriod()
        |- setDutyCycle()
   tickerCallbackBrightControl()
   */

//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "bright_control.h"

#include "light_level_control.h"

//=====[Declaration of private defines]========================================

#define LEDS_QUANTITY 3

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut RGBLed[] = {(PB_4), (PA_0), (PD_12)};

Ticker tickerBrightControl;

//=====[Declaration and initialization of private global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static int onTime[LEDS_QUANTITY];
static int offTime[LEDS_QUANTITY];

static int tickRateMSBrightControl = 1;
static int tickCounter[LEDS_QUANTITY];

static float periodSFloat[LEDS_QUANTITY];

//=====[Declarations (prototypes) of private functions]========================

static void setPeriod( lightSystem_t light, float period );
static void tickerCallbackBrightControl( );

//=====[Implementations of public functions]===================================

void brightControlInit()
{
    tickerBrightControl.attach( tickerCallbackBrightControl, 
                              ( (float) tickRateMSBrightControl) / 1000.0 );

    setPeriod( RGB_LED_RED, 0.01f );
    setPeriod( RGB_LED_GREEN, 0.01f );
    setPeriod( RGB_LED_BLUE, 0.01f );

    setDutyCycle( RGB_LED_RED, 0.5f );
    setDutyCycle( RGB_LED_GREEN, 0.5f );
    setDutyCycle( RGB_LED_BLUE, 0.5f );
}

void setDutyCycle( lightSystem_t light, float dutyCycle )
{
    onTime[light] = int ( ( periodSFloat[light] * dutyCycle ) * 1000 );
    offTime[light] = int ( periodSFloat[light] * 1000) - onTime[light];
}

//=====[Implementations of private functions]==================================

static void setPeriod( lightSystem_t light, float period )
{
    periodSFloat[light] = period;
}

static void tickerCallbackBrightControl( )
{
    int i;

    for (i = 0 ; i < LEDS_QUANTITY ; i++) {
        tickCounter[i]++;
        if ( RGBLed[i].read() == ON ) {
            if( tickCounter[i] > onTime[i] ) {
                tickCounter[i] = 1;
                if ( offTime[i] ) RGBLed[i] = OFF;
                
            }
        } else {
            if( tickCounter[i] > offTime[i] ) { 
                tickCounter[i] = 1;
                if ( onTime[i] ) RGBLed[i] = ON;
            }
        }
    }
}