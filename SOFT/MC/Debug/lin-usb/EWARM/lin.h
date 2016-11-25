#ifndef LIN_H
#define LIN_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

#define  LIN_SYNC_DATA               0x55  // SyncField (nicht aendern)
#define  LIN_MAX_DATA                   8  // max 8 Datenytes

#define  LIN_POWERON_DELAY           ((uint32_t)10)  // Pause after PowerOn     (ca. 10ms)
#define  LIN_AKTIV_DELAY              ((uint32_t)1  // Pause for Transceiver (ca. 50us)

#define  LIN_INTER_FRAME_DELAY      ((uint32_t)10)  // Pause (Frame->Frame)   (ca. 10ms)
#define  LIN_FRAME_RESPONSE_DELAY    ((uint32_t)2) // Pause (Header->Data)   (ca.  2ms)
#define  LIN_BREAKFIELD_DELAY        ((uint32_t)4)  // Pause (Breakfield)     (ca.  4ms)
#define  LIN_DATA_BYTE_DELAY         ((uint32_t)1)  // Pause (Data->Data)     (ca.  1ms)
#define  LIN_RX_TIMEOUT_CNT          ((uint32_t)5)  // timeout when recive (ca.  5ms)

#define HAL_LIN_UART_TIMEOUT       100  // Timeout for HAL_Uart_Transmit function 

extern UART_HandleTypeDef huart1;

//
// LIN farme data structure
//
typedef struct {
  uint8_t frame_id;              // ID-Nummer for Frame
  uint8_t data_len;              // Data lenght
  uint8_t data[LIN_MAX_DATA];    // data array  
}LIN_FRAME_t;

//
// LIN bus status
//
typedef enum { 
  RECEIVE_DATA =0,  // Receive data
  RECEIVE_CRC,      // CRC recive
  SEND_DATA         // Send data
}LIN_MODE_t;


typedef struct {
  LIN_MODE_t mode;  // currebt Mode
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
LIN_ERR_t LIN_SendData(LIN_FRAME_t *frame);
LIN_ERR_t LIN_ReceiveData(LIN_FRAME_t *frame);
#endif