/*
 * rf_test.c
 *
 *  Created on: May 12, 2021
 *      Author: user
 */

#include "rf-test.h"

// #define RTC_TEST
// #define TX_TEST

#ifdef UART_TEST

static uint8_t data_UART[] = {0x00, 0x00, 0x00, 0x00};
static uint8_t data_Tx[] = {0x00, 0x00, 0x00, 0x00}; // это для теста uart
static uint32_t full_data_UART = 0; // это для теста uart
static char  str_to_send [128] = {0};

extern QueueHandle_t xQueueUartData;

#endif

static char  str_to_send [128] = {0};

static uint32_t prevSend = 0;
static uint32_t prevTick = 0;
static uint32_t Error = 0;
static uint16_t count = 0;
uint8_t data_UART[] = {0x00, 0x00, 0x00, 0x00};

// последовательности Баркера которые будут использоваться при поиске ошибок

BarkerSeq_t BarkerSeq[] =
  {
  		{ BARKER_2  , 0x0002 },
  		{ BARKER_3  , 0x0006 },
  		{ BARKER_4  , 0x000B },
  		{ BARKER_5  , 0x001D },
  		{ BARKER_7  , 0x0072 },
  		{ BARKER_11 , 0x0712 },
  		{ BARKER_13 , 0x1F35 }
  };

extern QueueHandle_t xQueueUartData;
struct InputParametrsRX_s iparamCopy;
struct BER_RX_s berParamCopy;
struct PER_RX_s perParamCopy;

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

static void InitRf (bool crcOn)
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
								crcOn, 0, 0, 0, 3000 );

  Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, crcOn,
                                0, 0, false, true );

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
  bool isMaster = false;
  uint8_t i;
  InitRf( true );
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
    memcpy( data_UART, payload, sizeof(data_UART) );
    if ( xQueueSendToBackFromISR( xQueueUartData,  &data_UART, pdFALSE ) != pdPASS) {
    	printf( "Failed to post the message" );
    }
#endif
    memcpy( data_UART, payload, sizeof(data_UART) );
if( iparamCopy.mode == PER ) {
    if ( !prevTick ) {
    	prevTick = Tick;  /* фиксируем время начала приёма */
    }
    else {
    	dTick = Tick - prevTick; // находим разницу времени между двумя принятыми посылками
    	prevTick = Tick;
    }
    if ( dTick > iparamCopy.pPER->AverageTime ) {
    	if ( *( uint32_t* )data_UART < prevSend ) {
    		if ( !( ( *( uint32_t* )data_UART == 0 ) && ( prevSend == ( iparamCopy.pPER->NumberOfPacketSent - 1 ) ) ) ) {
        		Error += ( iparamCopy.pPER->NumberOfPacketSent - prevSend ) - 1;	/* если произошла потеря и начался следующий период передачи
        		то находим сколько пакетов было потеряно между концом предыдущего периода и началом следующего
        		Отнимаем один потому что считаем от 0
        	 */
    		}
    	}
    	/* Отправляем итоговую ошибку в очередь для вывода в терминал*/
        if ( xQueueSendToBackFromISR( xQueueUartData,  &Error, pdFALSE ) != pdPASS) {
        	printf( "Failed to post the message" );
        }

        if ( *( uint32_t* )data_UART < prevSend ) {
        	Error = *( uint32_t* )data_UART; /* если с начала приёма следующего периода были потеряны
        	пакеты то находим их количество ( тут происходит обнуление )*/
        }
    }
    else {
    	Error += *( uint32_t* )data_UART - prevSend - 1; /* если время текущего периода ещё не вышло то
    	находим число потерянных пакетов, если такие есть*/
    }
    prevSend = *( uint32_t* )data_UART;
}
else if( iparamCopy.mode == BER ) {
    count++;
    uint16_t rxSeq = *( uint16_t* )data_UART;
    BarkerSeq_t* StandartSeq;
    int i = 0;

    while( ( ( StandartSeq = &BarkerSeq[i++] )->SequenceSize ) != iparamCopy.pBER->len ); /* ищем нужную эталонную последовательность */
    i = 0;
    while( i < iparamCopy.pBER->len ){
    	Error += ( ( rxSeq & ( 0x0001 << i ) ) ^ ( ( StandartSeq->Sequence ) & ( 0x0001 << i ) ) ) >> i; /* считаем количество несоответствующих битов*/
    	i++;
    }
// передаём в очередь для вывод на экран
        if ( xQueueSendToBackFromISR( xQueueUartData,  &Error, pdFALSE ) != pdPASS) {
        	printf( "Failed to post the message" );
        }
}
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
	if ( !uxQueueMessagesWaitingFromISR(xQueueUartData) ) {
			return;
	}
	uint32_t data_Tx;
	xQueueReceiveFromISR( xQueueUartData, &data_Tx, pdFALSE );
	if( iparamCopy.mode == BER ) {
		sprintf(str_to_send, "%s %lu %s %d \n\r",  "Bits lost: ", data_Tx, "of", iparamCopy.pBER->len );
		Error = 0;
	}
	else if( iparamCopy.mode == PER) {
		sprintf(str_to_send, "%s %lu %s %u \n\r",  "Packets lost: ", data_Tx, "of", iparamCopy.pPER->NumberOfPacketSent);
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)str_to_send, strlen(str_to_send), 10);
	xQueueReset( xQueueUartData );

#ifdef UART_TEST
	/* Это если понадобится выводить принятые данные в терминал, но сначала надо подправить колбэк
	 * успешного приёма*/
	xQueueReceiveFromISR( xQueueUartData, &data_Tx, pdFALSE );
	full_data_UART = *(uint32_t*)data_Tx;
	sprintf(str_to_send, "%d\n\r", (int)full_data_UART);
	HAL_UART_Transmit(&huart2, (uint8_t*)str_to_send, strlen(str_to_send), 10);
    xQueueReset( xQueueUartData );
#endif
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

bool Measurements ( struct InputParametrsRX_s* param ) {
	iparamCopy.mode = param->mode;
	berParamCopy.len = param->pBER->len;
	perParamCopy.AverageTime = param->pPER->AverageTime;
	perParamCopy.NumberOfPacketSent = param->pPER->NumberOfPacketSent;
	iparamCopy.pBER = &berParamCopy;
	iparamCopy.pPER = &perParamCopy;
	switch ( iparamCopy.mode ) {
	case BER:
		if ( iparamCopy.pBER != NULL ) InitRf( false );
		else return false;
	break;
	case PER:
		if ( iparamCopy.pPER != NULL ) {
			count = 0;
			InitRf( true );
		}
		else return false;
	break;
	default:
	break;
	}
	return true;
}



