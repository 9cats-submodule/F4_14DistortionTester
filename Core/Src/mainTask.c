#include "main.h"
#include "cmsis_os.h"
#include "cmd_process.h"
#include "ADS8688.h"
#include "AD9959.h"

/*!
 *  \brief  TIM1 配置 PRC, ARR
 *  \param  PRC 预分配系数
 *  \param  ARR 自动重装载值
 *  \
 */
void TIM1_CONFIG(u32 PRC,u32 ARR)
{
	__HAL_TIM_SET_PRESCALER(  &htim1, PRC);
	__HAL_TIM_SET_AUTORELOAD( &htim1, ARR);
	__HAL_TIM_SET_COUNTER(    &htim1,   0);
}

/*!
 *  \brief  ADS8688 多通道 动态配置
 *  \param  CH 所选通道 (例 0b11011101)
 *  \param  range 通道范围
 *  \warn!  初始化后120ms左右内采样值不精确
 */
void ADS8688_MUL_CONFIG(u8 CH,u8 range)
{
	CH_NUM = 0;
	ADS8688_CONFIG(CH,0x01);
	while(CH)
	{
		CH_NUM += CH & 0x01;
		CH >>= 1;
	}
}

/*!
 *  \brief  AD9959 动态配置
 *  \param  freq 频率
 *  \param  amp  幅度
 *  \warn!  初始化后需要一定时间
 */
void AD9959_CONFIG(float freq,float mv)
{
	//只有通道1 和 通道3可用
	Out_freq(2, freq);
	Out_mV(2, mv);
}

/*!
 *  \brief  开启 TIM1 定时采样一定点数
 *  \param  point 每个通道的采样点数
 */
void ADS8688_SAMPLE(u16 point)
{
	SAMPLE_POINT = CH_NUM * point;
	HAL_TIM_Base_Start_IT(&htim1);
	while(osSemaphoreAcquire(SAMPLE_FINISHEDHandle, 0) != osOK)
		osDelay(1);
}

/*!
 *  \brief    FFT变换
 */
void FFT(void)
{
	arm_rfft_fast_instance_f32 S;
	u16 i;

	for(i=0;i<2048;i++)
	{
		FFT_INPUT[i] = (float)BUF[i]*10.24f/0x10000;
	}
	arm_rfft_fast_init_f32(&S,2048);                    //FFT初始化
	arm_rfft_fast_f32(&S, FFT_INPUT, FFT_OUTPUT,0);     //FFT变化
	arm_cmplx_mag_f32(FFT_OUTPUT,FFT_OUTPUT_REAL,2048); //求模
}

/*!
 *  \brief  主任务
 */
void MainTask_Start(void const *argument) {
	TIM1_CONFIG(25-1,105-1);						//采样率 64K
	ADS8688_MUL_CONFIG(0b00000100,2);		//ADS 3通道开启，±5.12 V
//	AD9959_CONFIG(1000,300);						//DDS 3通道开启，1KHZ，300mV

	osDelay(400);

	for(;;){
		ADS8688_SAMPLE(2048);

		{
			u16 i;
			for(i=0;i<2048;i++)
			{
				OutData[0] = ADS8688_BUF[0][i];
				OutPut_Data();
				osDelay(1);
			}
		}
		SetTextValue(1,49,(u8*)"测量电路参数中..  ");
		osDelay(300);
	}
}
