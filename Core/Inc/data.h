#ifndef __DATA_H
#define __DATA_H
#include "base.h"
#include "cmd_process.h"

typedef struct _SVAR
{
  float FFT_COMPENSATE; //
  float RMS_COMPENSATE; //
  float DC_COMPENSATE;  //
  float ADS_OFFSET;     //
  float C3_NORMAL;      //
}SVAR;
extern SVAR Svar;

//全局通用变量
extern u8 mode;
//FreeRTOS--任务信息量句柄
extern osMessageQueueId_t USART1_RXHandle;
extern osSemaphoreId_t    TFT_TX_LEDHandle;
extern osSemaphoreId_t    TFT_RX_LEDHandle;
extern osSemaphoreId_t    SAMPLE_FINISHEDHandle;
//用于 ADS8688 采样
extern u16 SAMPLE_POINT;
extern u16 ADS8688_BUF[];

//------以下基本不用动-------
extern u8 RxBuffer; //USART1接收BUF
#endif
