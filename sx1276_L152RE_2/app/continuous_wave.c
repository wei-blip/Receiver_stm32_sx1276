/*
 * continuous_wave.c
 *
 *  Created on: 1 июн. 2021 г.
 *      Author: user
 */
#include "continuous_wave.h"

static TimerEvent_t Led1Timer;
volatile bool Led1TimerEvent = false;

static TimerEvent_t Led2Timer;
volatile bool Led2TimerEvent = false;

/*!
 * \brief Function executed on Led 1 Timeout event
 */
void OnLed1TimerEvent( void* context )
{
    Led1TimerEvent = true;
}

/*!
 * \brief Function executed on Led 2 Timeout event
 */
void OnLed2TimerEvent( void* context )
{
    Led2TimerEvent = true;
}

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnRadioTxTimeout( void )
{
    // Restarts continuous wave transmission when timeout expires
    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, TX_TIMEOUT );
}

/**
 * Main application entry point.
 */
void continuos_wave(void)
{
    // Target board initialization
    BoardInitMcu( );
    BoardInitPeriph( );

    TimerInit( &Led2Timer, OnLed2TimerEvent );
    TimerSetValue( &Led2Timer, 90 );

//    Led2.port = LD2_GPIO_Port;
//    Led2.pin = LD2_Pin;

    // Switch LED 1 ON
    GpioWrite( &Led2, 1 );
    TimerStart( &Led2Timer );

    // Radio initialization
    RadioEvents.TxTimeout = OnRadioTxTimeout;
    Radio.Init( &RadioEvents );

    Radio.SetTxContinuousWave( RF_FREQUENCY, TX_OUTPUT_POWER, TX_TIMEOUT );

    // Blink LEDs just to show some activity
    while( 1 )
    {
        // Process Radio IRQ
        if( Radio.IrqProcess != NULL )
        {
            Radio.IrqProcess( );
        }
//        if( Led1TimerEvent == true )
//        {
//            Led1TimerEvent = false;
//
//            // Switch LED 1 OFF
//            GpioWrite( &Led1, 1 );
//            // Switch LED 2 ON
//            GpioWrite( &Led2, 0 );
//            TimerStart( &Led2Timer );
//        }

        if( Led2TimerEvent == true )
        {
            Led2TimerEvent = false;

            // Switch LED 2 OFF
            GpioToggle(&Led2);
            TimerStart( &Led2Timer );
        }
    }
}

