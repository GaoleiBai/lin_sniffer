#ifndef LIN_H
#define LIN_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

#define  LIN_SYNC_DATA               0x55  // SyncField (do not edit)
#define  LIN_MAX_DATA                   8  // max 8 Datenytes
#define  LIN_RX_TX_ARRAY_LENGHT        12  // Array for tx/rx lin frane to/from PC 

#define  LIN_POWERON_DELAY             10  // Pause after PowerOn     (ca. 10ms)
#define  LIN_AKTIV_DELAY                1  // Pause for Transceiver (ca. 50us)

#define  LIN_INTER_FRAME_DELAY         10  // Pause (Frame->Frame)   (ca. 10ms)
#define  LIN_FRAME_RESPONSE_DELAY       2  // Pause (Header->Data)   (ca.  2ms)
#define  LIN_BREAKFIELD_DELAY           4  // Pause (Breakfield)     (ca.  4ms)
#define  LIN_DATA_BYTE_DELAY            1  // Pause (Data->Data)     (ca.  1ms)
#define  LIN_RX_TIMEOUT_CNT             5  // timeout when recive (ca.  5ms)

#define HAL_LIN_UART_TIMEOUT          100  // Timeout for HAL_Uart_Transmit function 

#define ID_MASK                      0x0f // get real ID from header of lIN message
//#defien DATALENGHT_MASK              (0x03 <<4))>>4 // get data lenght from header of lIN message

extern UART_HandleTypeDef huart1;

extern uint8_t linRxBff[1];
extern uint8_t farmeBuffPtr;

//
// LIN farme data structure
//
typedef struct {
  uint8_t frame_Header;
  uint8_t frame_id;              // ID-Nummer for Frame
  uint8_t data_len;              // Data lenght
  uint8_t data[LIN_MAX_DATA];    // data array
  uint8_t crc;
}LIN_FRAME_t;


//
// LIN bus status
//
typedef enum { 
  RECEIVE_DATA =0,  // Receive data
  RECEIVE_CRC,      // CRC recive
  SEND_DATA         // Send data
}LIN_MODE_t;

//
//LIN recive mode
//
typedef enum {
	WAIT_SYNC = 0,
        WAIT_ID,
	WAIT_DATA,
	WAIT_CHECKSUM
}LIN_RXMODE_t;


typedef struct {
  LIN_RXMODE_t mode;  // currebt Mode
  uint8_t data_ptr; // Datenpointer  
  uint8_t crc;      // Checksumme
}LIN_MASTER_t;


typedef enum {
  LIN_OK  = 0,   // no error
  LIN_WRONG_LEN, // wrong error
  LIN_RX_EMPTY,  //no Frame recived
  LIN_WRONG_CRC  // Wrong CRC
}LIN_ERR_t;


void UB_LIN_Master_Init(void);
void AddFrameToBuff(LIN_FRAME_t lin_frame);
LIN_FRAME_t GetFrameFromBuff(void);
void HAL_UART_RxCplt(UART_HandleTypeDef *huart);
LIN_ERR_t LIN_SendData(LIN_FRAME_t *frame);
LIN_ERR_t LIN_ReceiveData(LIN_FRAME_t *frame);
#endif
