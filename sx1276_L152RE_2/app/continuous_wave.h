/*
 * continuous_wave.h
 *
 *  Created on: 1 июн. 2021 г.
 *      Author: user
 */

#ifndef CONTINUOUS_WAVE_H_
#define CONTINUOUS_WAVE_H_

#include "board.h"
#include "gpio.h"
#include "timer.h"
#include "radio.h"

#define REGION_EU433

#if defined( REGION_AS923 )

#define RF_FREQUENCY                                923000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_AU915 )

#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_CN470 )

#define RF_FREQUENCY                                470000000 // Hz
#define TX_OUTPUT_POWER                             20        // 20 dBm

#elif defined( REGION_CN779 )

#define RF_FREQUENCY                                779000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_EU433 )

#define RF_FREQUENCY                                433000000 // Hz
#define TX_OUTPUT_POWER                             20        // 20 dBm

#elif defined( REGION_EU868 )

#define RF_FREQUENCY                                868000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_KR920 )

#define RF_FREQUENCY                                920000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_IN865 )

#define RF_FREQUENCY                                865000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_US915 )

#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#elif defined( REGION_RU864 )

#define RF_FREQUENCY                                864000000 // Hz
#define TX_OUTPUT_POWER                             14        // 14 dBm

#endif
#define TX_TIMEOUT                                  10        // seconds (MAX value)

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * LED GPIO pins objects
 */
extern Gpio_t Led1;
extern Gpio_t Led2;

void continuos_wave(void);
//void OnRadioTxTimeout( void );
#endif /* CONTINUOUS_WAVE_H_ */
