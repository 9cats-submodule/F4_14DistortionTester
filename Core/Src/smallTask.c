#include "main.h"
#include "cmsis_os.h"
#include "cmd_process.h"
#include "semphr.h"


/*!
 *  \brief    变量自动存储
 *  \details  每隔0.1s储存一次数据，优先级最低
 */
void FLASH_Data_AutoUpdate_Start(void *arguement)	{
	for(;;)
	{
		DATA_UPDATE();
		vTaskDelay_ms(100);
	}
}

/*!
 *  \brief    TFT串口指令处理
 *  \details  当USART1串口接收到TFT的指令时接触阻塞
 */
void TFT_CMD_Process_Start(void *argument)	{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	CTRL_MSG   TFT_CMD_MSG = {0};

  for(;;)
  {
  	if(xQueueReceiveFromISR(USART1_RXHandle, &TFT_CMD_MSG, &xHigherPriorityTaskWoken) == pdPASS)
  	{
  		ProcessMessage(&TFT_CMD_MSG,0);
			osSemaphoreRelease(TFT_RX_LEDHandle);
  	}
  	vTaskDelay_ms(30);
  }
}

/*!
 *  \brief    TFT-发送 LED1 闪烁
 *  \details  每次发送一次指令,LED1闪烁一次，持续0.05 s
 */
void StartLED0Toggle(void const *argument) {
//	osSemaphoreId semaphore = (osSemaphoreId) argument;

	for (;;) {
		if(osSemaphoreAcquire(TFT_TX_LEDHandle , 0) == osOK)
		{
			LED0_ON;
  		osDelay(20);
			LED0_OFF;
		}
		osDelay(20);
	}
}

/*!
 *  \brief    TFT-LED2闪烁
 *  \details  每处理一次指令,LED2闪烁一次,持续 0.1 秒
 */
void StartLED1Toggle(void const *argument) {
//	osSemaphoreId semaphore = (osSemaphoreId) argument;

  for (;;) {
		if(osSemaphoreAcquire(TFT_RX_LEDHandle , 0) == osOK)
  	{
    	LED1_ON;
    	osDelay(20);
    	LED1_OFF;
  	}
  	osDelay(20);
  }
}

/*!
 *  \brief    TFT-LED2闪烁
 *  \details  每处理一次指令,LED2闪烁一次,持续 0.1 秒
 */
void StartLED2Toggle(void *argument)	{
  for(;;)
  {
  	LED2_ON;
  	osDelay(300);
  	LED2_OFF;
  	osDelay(300);
  }
}

