/*
 * rf_test.c
 *
 *  Created on: May 12, 2021
 *      Author: user
 */

#include "rf-test.h"

// #define RTC_TEST
// #define TX_TEST
#define UART_TEST

#ifdef UART_TEST
static uint8_t data_UART[] = {0x00, 0x00, 0x00, 0x00};
static uint8_t data_Tx[] = {0x00, 0x00, 0x00, 0x00};
static uint32_t prevSend = 0;
static uint32_t Error = 0;
uint32_t prevTick = 0;
uint32_t AverageTime = 0;
static uint32_t full_data_UART = 0;
static uint32_t count = 0;
static char  str_to_send [128] = {0};
extern QueueHandle_t xQueueUartData;
#endif

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

States_t State = LOWPOWER;

static int8_t RssiValue = 0;
int8_t SnrValue = 0;

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * LED GPIO pins objects
 */
extern SX1276_t SX1276;
extern Gpio_t Led1;
extern Gpio_t Led2;
extern UART_HandleTypeDef huart2;

void init_rf (void)
{
  // Target board initialization
  BoardInitMcu( );
  BoardInitPeriph( );

  // Radio initialization
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init( &RadioEvents );

  Radio.SetChannel( RF_FREQUENCY );

#if defined( USE_MODEM_LORA )

  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                 LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                 LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                 LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

  Radio.SetMaxPayloadLength( MODEM_LORA, BUFFER_SIZE );

#elif defined( USE_MODEM_FSK )

  Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                FSK_DATARATE, 0,
                                FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                true, 0, 0, 0, 3000 );

  Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                0, 0,false, true );

  Radio.SetMaxPayloadLength( MODEM_FSK, BUFFER_SIZE );
#endif

}

#ifdef RTC_TEST
TimerEvent_t rtc_tim_1 = {0};

volatile int rtc_tim_1_cnt = 0;

void rtc_tim_1_callback (void)
{
  rtc_tim_1_cnt++;
  TimerStop(&rtc_tim_1);
  TimerSetValue(&rtc_tim_1, 1000);
  TimerStart(&rtc_tim_1);
}
#endif


void ping_pong_rf (void)
{
//  RtcInit();
  bool isMaster = false;
  uint8_t i;
  init_rf( );
#ifdef RTC_TEST
  TimerInit( &rtc_tim_1, rtc_tim_1_callback );
  TimerStop(&rtc_tim_1);
  TimerSetValue(&rtc_tim_1, 1000);
  TimerStart(&rtc_tim_1);
  while(1)
    {

    }
#endif

#ifdef TX_TEST
  uint8_t data_AA [20];
  memset(data_AA,0xAA,sizeof(data_AA));
  while(1)
    {
//      Radio.Send( data_AA, sizeof(data_AA) );
      HAL_Delay(5);
      Radio.Rx( RX_TIMEOUT_VALUE );
    }
#endif

#ifdef UART_TEST
  Radio_Rx( );
#endif
  Radio.Rx( RX_TIMEOUT_VALUE );
  HAL_GPIO_WritePin(LED_EXT_GPIO_Port, LED_EXT_Pin, RESET);
  while( 1 )
     {
         switch( State )
         {
         case RX:
             if( isMaster == true )
             {
                 if( BufferSize > 0 )
                 {
                     if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
                     {
                         // Indicates on a LED that the received frame is a PONG
                         GpioToggle( &Led1 );

                         // Send the next PING frame
                         Buffer[0] = 'P';
                         Buffer[1] = 'I';
                         Buffer[2] = 'N';
                         Buffer[3] = 'G';
                         // We fill the buffer with numbers for the payload
                         for( i = 4; i < BufferSize; i++ )
                         {
                             Buffer[i] = i - 4;
                         }
                         DelayMs( 1 );
                         Radio.Send( Buffer, BufferSize );
                     }
                     else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                     { // A master already exists then become a slave
                         isMaster = false;
                         GpioToggle( &Led2 ); // Set LED off
                         Radio.Rx( RX_TIMEOUT_VALUE );
                     }
                     else // valid reception but neither a PING or a PONG message
                     {    // Set device as master ans start again
                         isMaster = true;
                         Radio.Rx( RX_TIMEOUT_VALUE );
                     }
                 }
             }
             else
             {
                 if( BufferSize > 0 )
                 {
                     if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
                     {
                         // Indicates on a LED that the received frame is a PING
                         GpioToggle( &Led1 );

                         // Send the reply to the PONG string
                         Buffer[0] = 'P';
                         Buffer[1] = 'O';
                         Buffer[2] = 'N';
                         Buffer[3] = 'G';
                         // We fill the buffer with numbers for the payload
                         for( i = 4; i < BufferSize; i++ )
                         {
                             Buffer[i] = i - 4;
                         }
                         DelayMs( 1 );
                         Radio.Send( Buffer, BufferSize );
                     }
                     else // valid reception but not a PING as expected
                     {    // Set device as master and start again
                         isMaster = true;
                         Radio.Rx( RX_TIMEOUT_VALUE );
                     }
                 }
             }
             State = LOWPOWER;
             break;
         case TX:
             // Indicates on a LED that we have sent a PING [Master]
             // Indicates on a LED that we have sent a PONG [Slave]
             GpioToggle( &Led2 );
             Radio.Rx( RX_TIMEOUT_VALUE );
             State = LOWPOWER;
             break;
         case RX_TIMEOUT:
//	     State = RX;
//	     break;
         case RX_ERROR:
             if( isMaster == true )
             {
                 // Send the next PING frame
                 Buffer[0] = 'P';
                 Buffer[1] = 'I';
                 Buffer[2] = 'N';
                 Buffer[3] = 'G';
                 for( i = 4; i < BufferSize; i++ )
                 {
                     Buffer[i] = i - 4;
                 }
                 DelayMs( 1 );
                 Radio.Send( Buffer, BufferSize );
             }
             else
             {
                 Radio.Rx( RX_TIMEOUT_VALUE );
             }
             State = LOWPOWER;
             break;
         case TX_TIMEOUT:
             Radio.Rx( RX_TIMEOUT_VALUE );
             State = LOWPOWER;
             break;
         case LOWPOWER:
         default:
             // Set low power
             break;
         }

//         BoardLowPowerHandler( );
         // Process Radio IRQ
         if( Radio.IrqProcess != NULL )
         {
             Radio.IrqProcess( );
         }

     }
}

void OnTxDone( void )
{
    Radio.Sleep( );
    State = TX;
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	uint32_t Tick = HAL_GetTick();
	uint32_t dTick = 0;
	HAL_GPIO_TogglePin(LED_EXT_GPIO_Port, LED_EXT_Pin);
	Radio.Sleep( );
    BufferSize = size;
    memcpy( Buffer, payload, BufferSize );
    RssiValue = rssi;
    SnrValue = snr;
    State = RX;
#ifdef UART_TEST
    memcpy( data_UART, payload, sizeof(data_UART)/sizeof(uint8_t) );
    if ( xQueueSendToBackFromISR( xQueueUartData,  &data_UART, pdFALSE ) != pdPASS) {
    	printf("Failed to post the message");
    }
    Error += prevSend - *( uint32_t* )data_UART - 1;
    if (!prevSend) {
    	prevTick = Tick;
    }
    else {
    	dTick = Tick - prevTick; // находим разницу времени между двумя принятыми посылками
    	prevTick = Tick;
    }
    prevSend = *( uint32_t* )data_UART;

    count++;
    if( count == 999999 ) count = 0;
#endif
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    State = TX_TIMEOUT;
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
}

void OnRxError( void )
{
	char *str_to_tx =  "RX failed\n\r";
    Radio.Sleep( );
    State = RX_ERROR;
    HAL_UART_Transmit(&huart2, (uint8_t*)str_to_tx, strlen(str_to_tx), 10);
}

void UART_Tx( void ) {
	xQueueReceiveFromISR( xQueueUartData, &data_Tx, pdFALSE );
	full_data_UART = *(uint32_t*)data_Tx;
	sprintf(str_to_send, "%d\n\r", (int)full_data_UART);
	HAL_UART_Transmit(&huart2, (uint8_t*)str_to_send, strlen(str_to_send), 10);
	xQueueReset( xQueueUartData );
}


// Перед вызовом этой функции надо сначала вызвать Radio.Rx( RX_TIMEOUT_VALUE ) ВНЕ БЕСКОНЕЧНОГО ЦИКЛА!!!
// Сама функция Radio_Rx должна крутиться в бесконечном цикле
void Radio_Rx( void ) {
		  switch (State) {
		  case RX:
		  case RX_ERROR:
		  case RX_TIMEOUT:
		  case TX_TIMEOUT:
		  case TX:
			  Radio.Rx( RX_TIMEOUT_VALUE );
			  State = LOWPOWER;
			  break;
		  case LOWPOWER:
			  break;
	  }
}




