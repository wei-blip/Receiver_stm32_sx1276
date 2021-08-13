/*!
 * \file      sx1276mb1las-board.c
 *
 * \brief     Target board SX1276MB1LAS shield driver implementation
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
#include <stdlib.h>
#include "utilities.h"
#include "board-config.h"
#include "delay.h"
#include "radio.h"
#include "sx1276-board.h"

/*!
 * \brief Gets the board PA selection configuration
 *
 * \param [IN] channel Channel frequency in Hz
 * \retval PaSelect RegPaConfig PaSelect value
 */
static uint8_t SX1276GetPaSelect( uint32_t channel );

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

SX1276_DIO_callbacks_t DioIrqCallbacks;
extern DioIrqHandler *DioIrq[];

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby,
    SX1276SetRx,
    SX1276StartCad,
    SX1276SetTxContinuousWave,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength,
    SX1276SetPublicNetwork,
    SX1276GetWakeupTime,
    NULL, // void ( *IrqProcess )( void )
    NULL, // void ( *RxBoosted )( uint32_t timeout ) - SX126x Only
    NULL, // void ( *SetRxDutyCycle )( uint32_t rxTime, uint32_t sleepTime ) - SX126x Only
};

void SX1276IoInit( void )
{
  // DIO_0
  SX1276.DIO0.port = DIO_0_GPIO_Port;
  SX1276.DIO0.pin = DIO_0_Pin;
  // DIO_1
  SX1276.DIO1.port = DIO_1_GPIO_Port;
  SX1276.DIO1.pin = DIO_1_Pin;
  // DIO_2
  SX1276.DIO2.port = DIO_2_GPIO_Port;
  SX1276.DIO2.pin = DIO_2_Pin;
  // DIO_3
  SX1276.DIO3.port = DIO_3_GPIO_Port;
  SX1276.DIO3.pin = DIO_3_Pin;
  // DIO_4
  SX1276.DIO4.port = DIO_4_GPIO_Port;
  SX1276.DIO4.pin = DIO_4_Pin;
  // DIO_5
  SX1276.DIO5.port = DIO_5_GPIO_Port;
  SX1276.DIO5.pin = DIO_5_Pin;
}

void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
  DioIrqCallbacks.dio0 = irqHandlers[0];
  DioIrqCallbacks.dio1 = irqHandlers[1];
  DioIrqCallbacks.dio2 = irqHandlers[2];
  DioIrqCallbacks.dio3 = irqHandlers[3];
  DioIrqCallbacks.dio4 = irqHandlers[4];
  DioIrqCallbacks.dio5 = irqHandlers[5];
}

void SX1276IoDeInit( void )
{
  DioIrqCallbacks.dio0 = NULL;
  DioIrqCallbacks.dio1 = NULL;
  DioIrqCallbacks.dio2 = NULL;
  DioIrqCallbacks.dio3 = NULL;
  DioIrqCallbacks.dio4 = NULL;
  DioIrqCallbacks.dio5 = NULL;
}

void SX1276IoDbgInit( void )
{
#if defined( USE_RADIO_DEBUG )
    GpioInit( &DbgPinTx, RADIO_DBG_PIN_TX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    GpioInit( &DbgPinRx, RADIO_DBG_PIN_RX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
#endif
}

void SX1276IoTcxoInit( void )
{
    // No TCXO component available on this board design.
}

void SX1276SetBoardTcxo( uint8_t state )
{
    // No TCXO component available on this board design.
}

uint32_t SX1276GetBoardTcxoWakeupTime( void )
{
    return BOARD_TCXO_WAKEUP_TIME;
}

void SX1276Reset( void )
{
    // Enables the TCXO if available on the board design
    SX1276SetBoardTcxo( true );

    // Set RESET pin to 0
    HAL_GPIO_WritePin(SX1276.Reset.port, SX1276.Reset.pin, RESET);

    // Wait 1 ms
    DelayMs( 1 );

    // Configure RESET as input
    HAL_GPIO_WritePin(SX1276.Reset.port, SX1276.Reset.pin, SET);

    // Wait 6 ms
    DelayMs( 6 );
}

void SX1276SetRfTxPower( int8_t power )
{
    uint8_t paConfig = 0;
    uint8_t paDac = 0;

    paConfig = SX1276Read( REG_PACONFIG );
    paDac = SX1276Read( REG_PADAC );

    paConfig = ( paConfig & RF_PACONFIG_PASELECT_MASK ) | SX1276GetPaSelect( SX1276.Settings.Channel );

    if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
    {
        if( power > 17 )
        {
            paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_ON;
        }
        else
        {
            paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_OFF;
        }
        if( ( paDac & RF_PADAC_20DBM_ON ) == RF_PADAC_20DBM_ON )
        {
            if( power < 5 )
            {
                power = 5;
            }
            if( power > 20 )
            {
                power = 20;
            }
            paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
        }
        else
        {
            if( power < 2 )
            {
                power = 2;
            }
            if( power > 17 )
            {
                power = 17;
            }
            paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
        }
    }
    else
    {
        if( power > 0 )
        {
            if( power > 15 )
            {
                power = 15;
            }
            paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( 7 << 4 ) | ( power );
        }
        else
        {
            if( power < -4 )
            {
                power = -4;
            }
            paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( 0 << 4 ) | ( power + 4 );
        }
    }
    SX1276Write( REG_PACONFIG, paConfig );
    SX1276Write( REG_PADAC, paDac );
}

static uint8_t SX1276GetPaSelect( uint32_t channel )
{
    if( channel > RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

void SX1276SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
            SX1276AntSwInit( );
        }
        else
        {
            SX1276AntSwDeInit( );
        }
    }
}

void SX1276AntSwInit( void )
{
//    GpioInit( &AntSwitch, RADIO_ANT_SWITCH, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
}

void SX1276AntSwDeInit( void )
{
//    GpioInit( &AntSwitch, RADIO_ANT_SWITCH, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
}

void SX1276SetAntSw( uint8_t opMode )
{
}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}

uint32_t SX1276GetDio1PinState( void )
{
    return GpioRead( &SX1276.DIO1 );
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  void (*fptr)(void *context) = NULL;
  switch(GPIO_Pin)
  {
    case DIO_0_Pin:
      fptr = DioIrq[0];
      break;
    case DIO_1_Pin:
      fptr = DioIrq[1];
      break;
    case DIO_2_Pin:
      fptr = DioIrq[2];
      break;
    case DIO_3_Pin:
      fptr = DioIrq[3];
      break;
    case DIO_4_Pin:
      fptr = DioIrq[4];
      break;
    case DIO_5_Pin:
      fptr = DioIrq[5];
      break;
    default:
      break;
}
  if(fptr != 0)
    fptr(NULL);
}
