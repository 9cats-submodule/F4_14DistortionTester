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

//ȫ��ͨ�ñ���
extern u8 MODE;   //��ǰģʽ
extern u8 CH_NUM; //��ѡͨ������

//FreeRTOS--������Ϣ�����
extern osMessageQueueId_t USART1_RXHandle;
extern osSemaphoreId_t    TFT_TX_LEDHandle;
extern osSemaphoreId_t    TFT_RX_LEDHandle;
extern osSemaphoreId_t    SAMPLE_FINISHEDHandle;
//���� ADS8688 ����
#define SAMPLE_POINT_MAX 2048
#define SAMPLE_CH_MAX 2
extern u32 SAMPLE_POINT;
extern u16 ADS8688_BUF[][SAMPLE_POINT_MAX];

//------���»������ö�-------
extern u8 RxBuffer; //USART1����BUF
#endif
