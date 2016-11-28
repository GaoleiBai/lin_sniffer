#include "lin.h"


// Global variables
//----------------------------------------------------------------------------
LIN_MASTER_t LIN_MASTER;
LIN_FRAME_t LIN_FRAME;

// Function declarate
//----------------------------------------------------------------------------
uint8_t p_LIN_makeChecksum(LIN_FRAME_t *frame);
void UB_LIN_Master_Init(void);

// Init all befors start
//----------------------------------------------------------------------------
void UB_LIN_Master_Init(void)
{
  // init all struct
  LIN_MASTER.mode=SEND_DATA;
  LIN_MASTER.data_ptr=0;  
  LIN_MASTER.crc=0;

  LIN_FRAME.frame_id=0;
  LIN_FRAME.data_len=0; 
  LIN_FRAME.data[0]=0;

  // Wait a short time until Transceiver is ready
  HAL_Delay(LIN_POWERON_DELAY);
}

// Send data
//----------------------------------------------------------------------------
LIN_ERR_t LIN_SendData(LIN_FRAME_t *frame)
{
  uint8_t checksum,n;

  //check the length
  if((frame->data_len<1) || (frame->data_len>LIN_MAX_DATA)) {
    return(LIN_WRONG_LEN);
  }   

  // checksumme ausrechnen
  checksum=p_LIN_makeChecksum(frame);

  // wait until last Byte has been sent
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TXE) == RESET);

  //------------------------
  // Break-Field
  //------------------------
  HAL_LIN_SendBreak(&huart1);
  //wait until break field has been transmitted
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // Small pause
  /AL_Delay(LIN_BREAKFIELD_DELAY);

  //------------------------
  // Sync-Field
  //------------------------
  //huart1.Instance->DR = (uint8_t)LIN_SYNC_DATA;
  HAL_UART_Transmit(huart1, LIN_SYNC_DATA, 1, HAL_LIN_UART_TIMEOUT);
  //wait until the sync field is sent
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // kleine Pause
  //HAL_Delay(LIN_DATA_BYTE_DELAY); 

  //------------------------
  // ID-Field
  //------------------------
  huart1.Instance->DR = frame->frame_id;
  
  // wait until the ID field has been sent
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // kleine Pause
  HAL_Delay(LIN_FRAME_RESPONSE_DELAY);

  //------------------------
  // Data-Field [1...n]
  //------------------------
  for(n=0;n<frame->data_len;n++) {
    huart1.Instance->DR = frame->data[n];
    // wait until the DataField has been sent
    //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

    // kleine Pause
    HAL_Delay(LIN_DATA_BYTE_DELAY);
  }

  //------------------------
  // CRC-Field
  //------------------------
  huart1.Instance->DR = checksum;
  // wait until CRC field was sent
  // while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // small pause
  // so that next Frame is not sent too fast
  HAL_Delay(LIN_INTER_FRAME_DELAY);

  return(LIN_OK);    
}




// Calcuate Checksumm for LIN_Frame
//----------------------------------------------------------------------------
uint8_t p_LIN_makeChecksum(LIN_FRAME_t *frame)
{
  uint8_t ret_wert=0,n;
  uint16_t dummy;

  // checksumme ausrechnen  
  dummy=0;
  for(n=0;n<frame->data_len;n++) {
    dummy+=frame->data[n];
    if(dummy>0xFF) {
      dummy-=0xFF;
    } 
  }
  ret_wert=(uint8_t)(dummy);
  ret_wert^=0xFF;
  
  return(ret_wert);
}