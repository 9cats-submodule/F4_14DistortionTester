#include "data.h"

//�ɴ������
SVAR Svar = {
  /*float FFT_COMPENSATE; //����FFT�����*/909.09090909f,
  /*float RMS_COMPENSATE; //����RMS     */1134.0f,
  /*float DC_COMPENSATE;  //FFT��ֱ������*/0.25473997f,
  /*float ADS_OFFSET;     //ADSƫ�ò���  */32.2000008f,
  /*float C3_NORMAL;      //C3*/98.0f
};

//ȫ��ͨ�ñ���
u8 mode = 0;

xSemaphoreHandle SAMPLE_FINISHED = {0}; //��������

//���� ADS8688 ����
#define SAMPLE_POINT_MAX 2048
u16 SAMPLE_POINT      =  0;
u16 ADS8688_BUF[2048] = {0};

//FFT�任BUF


//------���»������ö�-------
u8 RxBuffer; //USART1����BUF
