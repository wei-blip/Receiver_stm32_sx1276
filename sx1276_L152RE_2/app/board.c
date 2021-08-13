/*!
 * \file      board.c
 *
 * \brief     Target board general functions implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include "stm32l1xx.h"
#include "utilities.h"
#include "gpio.h"
//#include "adc.h"
#include "spi.h"
//#include "i2c.h"
//#include "uart.h"
#include "timer.h"
#include "board-config.h"
#include "lpm-board.h"
#include "rtc-board.h"

#if defined( SX1261MBXBAS ) || defined( SX1262MBXCAS ) || defined( SX1262MBXDAS )
    #include "sx126x-board.h"
#elif defined( LR1110MB1XXS )
    #include "lr1110-board.h"
#elif defined( SX1272MB2DAS)
    #include "sx1272-board.h"
#elif defined( SX1276MB1LAS ) || defined( SX1276MB1MAS )
    #include "sx1276-board.h"
#endif
#include "board.h"

/*!
 * Unique Devices IDs register set ( STM32L152x )
 */
#define         ID1                                 ( 0x1FF800D0 )
#define         ID2                                 ( 0x1FF800D4 )
#define         ID3                                 ( 0x1FF800E4 )

/*!
 * LED GPIO pins objects
 */
Gpio_t Led1;
Gpio_t Led2;

/*
 * MCU objects
 */
//Adc_t  Adc;
//Uart_t Uart2;

#if defined( LR1110MB1XXS )
    extern lr1110_t LR1110;
#endif

/*!
 * Initializes the unused GPIO to a know status
 */
static void BoardUnusedIoInit( void );

/*!
 * System Clock Configuration
 */
static void SystemClockConfig( void );

/*!
 * System Clock Re-Configuration when waking up from STOP mode
 */
static void SystemClockReConfig( void );

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;

/*!
 * Flag used to indicate if board is powered from the USB
 */
static bool UsbIsConnected = false;

/*!
 * UART2 FIFO buffers size
 */
#define UART2_FIFO_TX_SIZE                                1024
#define UART2_FIFO_RX_SIZE                                1024

uint8_t Uart2TxBuffer[UART2_FIFO_TX_SIZE];
uint8_t Uart2RxBuffer[UART2_FIFO_RX_SIZE];

void BoardCriticalSectionBegin( uint32_t *mask )
{
    *mask = __get_PRIMASK( );
    __disable_irq( );
}

void BoardCriticalSectionEnd( uint32_t *mask )
{
    __set_PRIMASK( *mask );
}

void BoardInitPeriph( void )
{

}

void BoardInitMcu( void )
{
    if( McuInitialized == false )
    {
        RtcInit( );
        BoardUnusedIoInit( );
//        LpmSetOffMode( LPM_APPLI_ID, LPM_DISABLE );

    }
}

void BoardResetMcu( void )
{
    CRITICAL_SECTION_BEGIN( );

    //Restart system
    NVIC_SystemReset( );
}

void BoardDeInitMcu( void )
{

}

uint32_t BoardGetRandomSeed( void )
{
    return ( ( *( uint32_t* )ID1 ) ^ ( *( uint32_t* )ID2 ) ^ ( *( uint32_t* )ID3 ) );
}

void BoardGetUniqueId( uint8_t *id )
{
    id[7] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 24;
    id[6] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 16;
    id[5] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 8;
    id[4] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) );
    id[3] = ( ( *( uint32_t* )ID2 ) ) >> 24;
    id[2] = ( ( *( uint32_t* )ID2 ) ) >> 16;
    id[1] = ( ( *( uint32_t* )ID2 ) ) >> 8;
    id[0] = ( ( *( uint32_t* )ID2 ) );
}

/*!
 * Factory power supply
 */
#define VDDA_VREFINT_CAL ( ( uint32_t ) 3000 )  // mV

/*!
 * VREF calibration value
 */
#define VREFINT_CAL ( *( uint16_t* ) ( ( uint32_t ) 0x1FF800F8 ) )

/*
 * Internal temperature sensor, parameter TS_CAL1: TS ADC raw data acquired at
 * a temperature of 110 DegC (+-5 DegC), VDDA = 3.3 V (+-10 mV).
 */
#define TEMP30_CAL_ADDR ( *( uint16_t* ) ( ( uint32_t ) 0x1FF8007A ) )

/* Internal temperature sensor, parameter TS_CAL2: TS ADC raw data acquired at
 *a temperature of  30 DegC (+-5 DegC), VDDA = 3.3 V (+-10 mV). */
#define TEMP110_CAL_ADDR ( *( uint16_t* ) ( ( uint32_t ) 0x1FF8007E ) )

/* Vdda value with which temperature sensor has been calibrated in production
   (+-10 mV). */
#define VDDA_TEMP_CAL ( ( uint32_t ) 3000 )

/*!
 * Battery thresholds
 */
#define BATTERY_MAX_LEVEL 3000       // mV
#define BATTERY_MIN_LEVEL 2400       // mV
#define BATTERY_SHUTDOWN_LEVEL 2300  // mV

#define BATTERY_LORAWAN_UNKNOWN_LEVEL 255
#define BATTERY_LORAWAN_MAX_LEVEL 254
#define BATTERY_LORAWAN_MIN_LEVEL 1
#define BATTERY_LORAWAN_EXT_PWR 0

#define COMPUTE_TEMPERATURE( TS_ADC_DATA, VDDA_APPLI )                                                          \
    ( ( ( ( ( ( ( int32_t )( ( TS_ADC_DATA * VDDA_APPLI ) / VDDA_TEMP_CAL ) - ( int32_t ) TEMP30_CAL_ADDR ) ) * \
            ( int32_t )( 110 - 30 ) )                                                                           \
          << 8 ) /                                                                                              \
        ( int32_t )( TEMP110_CAL_ADDR - TEMP30_CAL_ADDR ) ) +                                                   \
      ( 30 << 8 ) )

static uint16_t BatteryVoltage = BATTERY_MAX_LEVEL;

//uint16_t BoardBatteryMeasureVoltage( void )
//{
//    uint16_t vref = 0;
//
//    // Read the current Voltage
//    vref = AdcReadChannel( &Adc, ADC_CHANNEL_VREFINT );
//
//    // Compute and return the Voltage in millivolt
//    return ( ( ( uint32_t ) VDDA_VREFINT_CAL * VREFINT_CAL ) / vref );
//}

uint32_t BoardGetBatteryVoltage( void )
{
    return BatteryVoltage;
}

uint8_t BoardGetBatteryLevel( void )
{
    uint8_t batteryLevel = 0;

    BatteryVoltage = BoardBatteryMeasureVoltage( );

    if( GetBoardPowerSource( ) == USB_POWER )
    {
        batteryLevel = BATTERY_LORAWAN_EXT_PWR;
    }
    else
    {
        if( BatteryVoltage >= BATTERY_MAX_LEVEL )
        {
            batteryLevel = BATTERY_LORAWAN_MAX_LEVEL;
        }
        else if( ( BatteryVoltage > BATTERY_MIN_LEVEL ) && ( BatteryVoltage < BATTERY_MAX_LEVEL ) )
        {
            batteryLevel =
                ( ( 253 * ( BatteryVoltage - BATTERY_MIN_LEVEL ) ) / ( BATTERY_MAX_LEVEL - BATTERY_MIN_LEVEL ) ) + 1;
        }
        else if( ( BatteryVoltage > BATTERY_SHUTDOWN_LEVEL ) && ( BatteryVoltage <= BATTERY_MIN_LEVEL ) )
        {
            batteryLevel = 1;
        }
        else  // if( BatteryVoltage <= BATTERY_SHUTDOWN_LEVEL )
        {
            batteryLevel = BATTERY_LORAWAN_UNKNOWN_LEVEL;
        }
    }
    return batteryLevel;
}

//int16_t BoardGetTemperature( void )
//{
//    uint16_t tempRaw = 0;
//
//    BatteryVoltage = BoardBatteryMeasureVoltage( );
//
//    tempRaw = AdcReadChannel( &Adc, ADC_CHANNEL_TEMPSENSOR );
//
//    // Compute and return the temperature in degree celcius * 256
//    return ( int16_t ) COMPUTE_TEMPERATURE( tempRaw, BatteryVoltage );
//}

static void BoardUnusedIoInit( void )
{
    HAL_DBGMCU_EnableDBGSleepMode( );
    HAL_DBGMCU_EnableDBGStopMode( );
    HAL_DBGMCU_EnableDBGStandbyMode( );
}
/**
  * \brief Enters Low Power Stop Mode
  *
  * \note ARM exists the function when waking up
  */
void LpmEnterStopMode( void)
{
    CRITICAL_SECTION_BEGIN( );

    BoardDeInitMcu( );

    // Disable the Power Voltage Detector
    HAL_PWR_DisablePVD( );

    // Clear wake up flag
    SET_BIT( PWR->CR, PWR_CR_CWUF );

    // Enable Ultra low power mode
    HAL_PWREx_EnableUltraLowPower( );

    // Enable the fast wake up from Ultra low power mode
    HAL_PWREx_EnableFastWakeUp( );

    CRITICAL_SECTION_END( );

    // Enter Stop Mode
    HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
}

/*!
 * \brief Exists Low Power Stop Mode
 */
void LpmExitStopMode( void )
{
    // Disable IRQ while the MCU is not running on HSI
    CRITICAL_SECTION_BEGIN( );

    // Initilizes the peripherals
    BoardInitMcu( );

    CRITICAL_SECTION_END( );
}

/*!
 * \brief Enters Low Power Sleep Mode
 *
 * \note ARM exits the function when waking up
 */
void LpmEnterSleepMode( void)
{
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void BoardLowPowerHandler( void )
{
    __disable_irq( );
    /*!
     * If an interrupt has occurred after __disable_irq( ), it is kept pending 
     * and cortex will not enter low power anyway
     */

    LpmEnterLowPower( );

    __enable_irq( );
}

/*
 * Function to be used by stdout for printf etc
 */
//int _write( int fd, const void *buf, size_t count )
//{
//    while( UartPutBuffer( &Uart2, ( uint8_t* )buf, ( uint16_t )count ) != 0 ){ };
//    return count;
//}

/*
 * Function to be used by stdin for scanf etc
 */
//int _read( int fd, const void *buf, size_t count )
//{
//    size_t bytesRead = 0;
//    while( UartGetBuffer( &Uart2, ( uint8_t* )buf, count, ( uint16_t* )&bytesRead ) != 0 ){ };
//    // Echo back the character
//    while( UartPutBuffer( &Uart2, ( uint8_t* )buf, ( uint16_t )bytesRead ) != 0 ){ };
//    return bytesRead;
//}





