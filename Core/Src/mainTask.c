#include "main.h"
#include "cmsis_os.h"
#include "cmd_process.h"

void MainTask_Start(void const *argument) {
	for(;;){
		switch(mode)
		{
		case 1:
//			xSemaphoreTake(SAMPLE_FINISHED,portMAX_DELAY);
			break;
		case 2:break;
		case 3:break;
		}
		vTaskDelay_ms(10);
	}
}

void StartSample(void const *argument)	{
	for(;;){
		osDelay(10);
	}
}
