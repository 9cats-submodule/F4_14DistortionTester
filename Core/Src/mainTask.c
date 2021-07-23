#include "main.h"
#include "cmsis_os.h"
#include "cmd_process.h"
#include "ADS8688.h"
#include "AD9959.h"

/*!
 *  \brief  TIM1 ���� PRC, ARR
 *  \param  PRC Ԥ����ϵ��
 *  \param  ARR �Զ���װ��ֵ
 *  \
 */
void TIM1_CONFIG(u32 PRC,u32 ARR)
{
	__HAL_TIM_SET_PRESCALER(  &htim1, PRC);
	__HAL_TIM_SET_AUTORELOAD( &htim1, ARR);
	__HAL_TIM_SET_COUNTER(    &htim1,   0);
}

/*!
 *  \brief  ADS8688 ��ͨ�� ��̬����
 *  \param  CH ��ѡͨ�� (�� 0b11011101)
 *  \param  range ͨ����Χ
 *  \warn!  ��ʼ����120ms�����ڲ���ֵ����ȷ
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
 *  \brief  AD9959 ��̬����
 *  \param  freq Ƶ��
 *  \param  amp  ����
 *  \warn!  ��ʼ������Ҫһ��ʱ��
 */
void AD9959_CONFIG(float freq,float mv)
{
	//ֻ��ͨ��1 �� ͨ��3����
	Out_freq(2, freq);
	Out_mV(2, mv);
}

/*!
 *  \brief  ���� TIM1 ��ʱ����һ������
 *  \param  point ÿ��ͨ���Ĳ�������
 */
void ADS8688_SAMPLE(u16 point)
{
	SAMPLE_POINT = CH_NUM * point;
	HAL_TIM_Base_Start_IT(&htim1);
	while(osSemaphoreAcquire(SAMPLE_FINISHEDHandle, 0) != osOK)
		osDelay(1);
}

/*!
 *  \brief    FFT�任
 */
void FFT(void)
{
	arm_rfft_fast_instance_f32 S;
	u16 i;

	for(i=0;i<2048;i++)
	{
		FFT_INPUT[i] = (float)BUF[i]*10.24f/0x10000;
	}
	arm_rfft_fast_init_f32(&S,2048);                    //FFT��ʼ��
	arm_rfft_fast_f32(&S, FFT_INPUT, FFT_OUTPUT,0);     //FFT�仯
	arm_cmplx_mag_f32(FFT_OUTPUT,FFT_OUTPUT_REAL,2048); //��ģ
}

/*!
 *  \brief  ������
 */
void MainTask_Start(void const *argument) {
	TIM1_CONFIG(25-1,105-1);						//������ 64K
	ADS8688_MUL_CONFIG(0b00000100,2);		//ADS 3ͨ����������5.12 V
//	AD9959_CONFIG(1000,300);						//DDS 3ͨ��������1KHZ��300mV

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
		SetTextValue(1,49,(u8*)"������·������..  ");
		osDelay(300);
	}
}
