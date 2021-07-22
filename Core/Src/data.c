#include "data.h"

//可储存变量
SVAR Svar = {
  /*float FFT_COMPENSATE; //补偿FFT的误差*/909.09090909f,
  /*float RMS_COMPENSATE; //补偿RMS     */1134.0f,
  /*float DC_COMPENSATE;  //FFT的直流补偿*/0.25473997f,
  /*float ADS_OFFSET;     //ADS偏置补偿  */32.2000008f,
  /*float C3_NORMAL;      //C3*/98.0f
};

//全局通用变量
u8 MODE = 0;   //当前模式
u8 CH_NUM = 0; //所选通道总数

//用于 ADS8688 采样
u32 SAMPLE_POINT      =  0;
u16 ADS8688_BUF[SAMPLE_CH_MAX][SAMPLE_POINT_MAX] = {0};

//FFT变换BUF


//------以下基本不用动-------
u8 RxBuffer; //USART1接收BUF
