#include "lin.h"
#include "usbd_cdc_if.h"


// Global variables
//----------------------------------------------------------------------------
LIN_MASTER_t LIN_MASTER;
LIN_FRAME_t LIN_FRAME;
LIN_FRAME_t lin_rx_frame;

uint8_t testBuff[12] = {0x1, 0x2};
uint8_t linRxBff[1] = {0};
uint8_t wert = 0;

LIN_FRAME_t frameBufer[50] = {0};
uint8_t farmeBuffPtr = 0;

// Function declarate
//----------------------------------------------------------------------------
int SetDataLen(uint8_t bitMask)
{
  int dataLen = 0;
  switch (bitMask)
  {
    case 0x00:
      dataLen = 2;
    break;
    
    case 0x01:
      dataLen = 2;
    break;
    
    case 0x02:
      dataLen = 4;
    break;
    
    case 0x08:
      dataLen = 8;
    break;
  }
  
  return dataLen;
}


uint8_t p_LIN_makeChecksum(LIN_FRAME_t *frame);
void UB_LIN_Master_Init(void);
// Init all befors start
//----------------------------------------------------------------------------
void UB_LIN_Master_Init(void)
{
  // init all struct
  LIN_MASTER.mode=WAIT_SYNC;
  LIN_MASTER.data_ptr=0;  
  LIN_MASTER.crc=0;

  LIN_FRAME.frame_id=0;
  LIN_FRAME.data_len=0; 
  LIN_FRAME.data[0]=0;

  // Wait a short time until Transceiver is ready
  HAL_Delay(10);
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
  //HAL_LIN_SendBreak(&huart1);
  //wait until break field has been transmitted
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // Small pause
  HAL_Delay(LIN_BREAKFIELD_DELAY);

  //------------------------
  // Sync-Field
  //------------------------
  huart1.Instance->DR = LIN_SYNC_DATA;

  //wait until the sync field is sent
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // kleine Pause
  HAL_Delay(LIN_DATA_BYTE_DELAY); 

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

    // Small Pause
    HAL_Delay(LIN_DATA_BYTE_DELAY);
  }
  
  HAL_Delay(LIN_DATA_BYTE_DELAY);
  
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

// LIN recive frame
////----------------------------------------------------------------------------
LIN_ERR_t LIN_ReceiveData(LIN_FRAME_t *frame)
{
  uint32_t rx_timeout;
  uint8_t checksum,n;

  // check the length
  if((frame->data_len<1) || (frame->data_len>LIN_MAX_DATA)) {
    return(LIN_WRONG_LEN);
  }

  // warte bis letztes Byte gesendet wurde
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TXE) == RESET);

  //-------------------------------
  // Break-Field
  //-------------------------------
  HAL_LIN_SendBreak(&huart1); 
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // kleine Pause
  HAL_Delay(LIN_BREAKFIELD_DELAY);

  //-------------------------------
  // Sync-Field
  //-------------------------------
  huart1.Instance->DR = LIN_SYNC_DATA;
 // while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // kleine Pause
  HAL_Delay(LIN_DATA_BYTE_DELAY); 

  //-------------------------------
  // ID-Field
  //-------------------------------
  huart1.Instance->DR = frame->frame_id;
  //while (USART_GetFlagStatus(LIN_UART, USART_FLAG_TC) == RESET);

  // kleine Pause
  HAL_Delay(LIN_DATA_BYTE_DELAY);

  //-------------------------------
  // Master vorbereiten
  //-------------------------------
  LIN_MASTER.mode=WAIT_SYNC;
  LIN_MASTER.data_ptr=0;
  LIN_MASTER.crc=0;

  LIN_FRAME.data_len=frame->data_len;
  LIN_FRAME.data[0]=0; 

  //-------------------------------
  // wait until Frame is received
  // or Timeout
  //-------------------------------
  rx_timeout=0;
  n=0;
  do {   
    // timeout counter
    rx_timeout++;    
    if(rx_timeout>LIN_RX_TIMEOUT_CNT) {
      // leave loop
      break;
    }
    // timeout reset, in the case of data reception
    if(LIN_MASTER.data_ptr!=n) {
      n=LIN_MASTER.data_ptr;
      rx_timeout=0;
    }
  }while(LIN_MASTER.mode!=WAIT_DATA);

  //-------------------------------
  // check whether frame is received
  //-------------------------------
  if(LIN_MASTER.mode!=WAIT_DATA) {
    // no Frame received
    LIN_MASTER.mode=WAIT_DATA;
  // little break
  // so that next Frame is not sent too fast
    HAL_Delay(LIN_INTER_FRAME_DELAY);
    return(LIN_RX_EMPTY);
  }  

  //-------------------------------
  // received copy data
  //-------------------------------
  for(n=0;n<frame->data_len;n++) {
    frame->data[n]=LIN_FRAME.data[n]; 
  }
  // check sum calculate
  checksum=p_LIN_makeChecksum(frame);

  //-------------------------------
  // check if crc is ok
  //-------------------------------
  if(LIN_MASTER.crc!=checksum) {
  // check sum is wrong
  // little break
  // so that next Frame is not sent too fast
    HAL_Delay(LIN_INTER_FRAME_DELAY);
    return(LIN_WRONG_CRC);
  }
      
  //-------------------------------
  // data are ok
  //-------------------------------
  // little break
  // so that next Frame is not sent too fast
  HAL_Delay(LIN_INTER_FRAME_DELAY);

  return(LIN_OK);
}


// LIN Transceiver (MAX13020)
// from the "Sleep Mode" in "Normal-Slope Mode" switch
//----------------------------------------------------------------------------
void p_LIN_aktivateTransceiver(void)
{
  // NSLP-Pin auf Lo-Pegel
  HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_SET); // in "Sleep-Mode"
  // kurz warten (min. 10us)
  HAL_Delay(LIN_AKTIV_DELAY);
  // NSLP-Pin auf Hi-Pegel  
  HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_RESET); // in "Normal-Slope-Mode"
  // kurz warten (min. 10us)
  HAL_Delay(LIN_AKTIV_DELAY);
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

void AddFrameToBuff(LIN_FRAME_t lin_frame)
{
  farmeBuffPtr++;
  frameBufer[farmeBuffPtr - 1] = lin_frame;
}

LIN_FRAME_t GetFrameFromBuff(void)
{
  LIN_FRAME_t f = frameBufer[farmeBuffPtr - 1];
  farmeBuffPtr++;

  return f;
}

void ClearArray(uint8_t* array)
{
  memset(array, 0, sizeof(array));
}

void HAL_UART_RxCplt(UART_HandleTypeDef *huart)
{ 
  if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
  {
    wert=huart->Instance->DR;
    
    switch ((uint8_t)(LIN_MASTER.mode))
    {
	case WAIT_SYNC:
            if(wert == LIN_SYNC_DATA)
              LIN_MASTER.mode = WAIT_ID;
          break;
          
	case WAIT_ID:
            lin_rx_frame.frame_Header = wert;
            lin_rx_frame.frame_id = wert & ID_MASK;
            lin_rx_frame.data_len =  SetDataLen((wert & (0x03 <<4))>>4);
            LIN_MASTER.mode = WAIT_DATA;
          break;
          
	case WAIT_DATA:
            if(LIN_MASTER.data_ptr < lin_rx_frame.data_len)
            {
              lin_rx_frame.data[LIN_MASTER.data_ptr] = wert;
              LIN_MASTER.data_ptr++;
            }else
            {
              LIN_MASTER.data_ptr = 0;
              LIN_MASTER.mode = WAIT_CHECKSUM;
              lin_rx_frame.crc = wert;
              LIN_MASTER.mode = WAIT_SYNC;
              CDC_Transmit_FS(lin_rx_frame.data, 8);
              HAL_GPIO_TogglePin(LED_PIN_GPIO_Port, LED_PIN_Pin);
            }
        break;
        case WAIT_CHECKSUM:
            LIN_MASTER.crc = wert;
            LIN_MASTER.mode = WAIT_SYNC;
          break;
    }
    
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
  }
}
