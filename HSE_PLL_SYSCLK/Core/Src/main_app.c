/*
 * main.c
 *
 *  Created on: 06-Aug-2024
 *      Author: alexa
 */

/*---------My modified template-----------*/


#include<string.h>
#include<stdio.h>
#include "stm32f4xx_hal.h"
#include "main_app.h"

#define TRUE 1
#define FALSE 0
#define SYS_CLOCK_FREQ_50_MHZ 50
#define SYS_CLOCK_FREQ_80_MHZ 80
#define SYS_CLOCK_FREQ_120_MHZ 120


void UART2_Init(void);
void Error_handler(void);
void menu(void);
void SystemClock_Config(uint8_t clock_freq);
void SystemClock_Config_HSE(uint8_t clock_freq);


UART_HandleTypeDef huart2;
char msg[100];
uint8_t recvd_data;
uint32_t count=0;
uint8_t reception_complete = FALSE;
uint8_t FLatency = 0;



int main(void)
{


	HAL_Init();

	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);

	UART2_Init();
		memset(msg,0,sizeof(msg));
		msg[sizeof(msg) - 1] = '\0';
		sprintf(msg,"SYSCLK : %ldHz\r\n",HAL_RCC_GetSysClockFreq());
		HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

		memset(msg,0,sizeof(msg));
		sprintf(msg,"HCLK   : %ldHz \r\n",HAL_RCC_GetHCLKFreq());
		HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

		memset(msg,0,sizeof(msg));
		sprintf(msg,"PCLK1  : %ldHz \r\n",HAL_RCC_GetPCLK1Freq());
		HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

		memset(msg,0,sizeof(msg));
		sprintf(msg,"PCLK2  : %ldHz \r\n\r\n",HAL_RCC_GetPCLK2Freq());
		HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);



	while(1);


	return 0;
}

void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
		RCC_ClkInitTypeDef clk_init;

		osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		osc_init.HSEState = RCC_HSE_BYPASS;
		osc_init.PLL.PLLState = RCC_PLL_ON;
		osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

		switch(clock_freq)
		{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;
			osc_init.PLL.PLLP = 2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

		    FLatency = FLASH_ACR_LATENCY_1WS;;
			break;
		}

		case SYS_CLOCK_FREQ_80_MHZ:
		{

			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 160;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;
			osc_init.PLL.PLLP = 2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

		    FLatency = FLASH_ACR_LATENCY_2WS;
			break;
		}

		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 1;
			osc_init.PLL.PLLP = 4;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		    FLatency = FLASH_ACR_LATENCY_3WS;
			break;
		}

		default:
			return;
		}

		if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
			{
			Error_handler();
			}

		if(HAL_RCC_ClockConfig( &clk_init, FLatency) != HAL_OK)
			{
				Error_handler();
			}

		//Systick frequency
		HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void SystemClock_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 15;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
	case SYS_CLOCK_FREQ_50_MHZ:
	{
		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 100;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;
		osc_init.PLL.PLLP = 2;

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	    FLatency = FLASH_ACR_LATENCY_1WS;;
		break;
	}

	case SYS_CLOCK_FREQ_80_MHZ:
	{

		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 160;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;
		osc_init.PLL.PLLP = 2;

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	    FLatency = FLASH_ACR_LATENCY_2WS;
		break;
	}

	case SYS_CLOCK_FREQ_120_MHZ:
	{
		osc_init.PLL.PLLM = 16;
		osc_init.PLL.PLLN = 240;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;
		osc_init.PLL.PLLP = 4;

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	    FLatency = FLASH_ACR_LATENCY_3WS;
		break;
	}

	default:
		return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
		{
		Error_handler();
		}

	if(HAL_RCC_ClockConfig( &clk_init, FLatency) != HAL_OK)
		{
			Error_handler();
		}

	//Systick frequency
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

//	 if(recvd_data == '\r')
//	 {
//		 reception_complete = TRUE;
//		 data_buffer[count++]='\r';
//		 HAL_UART_Transmit(huart,data_buffer,count,HAL_MAX_DELAY);
//	 }
//	 else
//	 {
//		 data_buffer[count++] = recvd_data;
//		 if(data_buffer[count] == '1')
//		 {
//			 char pressed1[20] = "You pressed 1";
//			 HAL_UART_Transmit(huart,pressed1,count,HAL_MAX_DELAY);
//
//		 }
//	 }


}

//void menu(void)
//{
//	char buffer[100];
//
//	memset(buffer, 0, sizeof(buffer) - 1);
//	buffer[sizeof(buffer) - 1] = '\0';
//	sprintf(buffer, "Welcome to select SYSCLK frequency\r\n\r\n"
//			"1. Press 1 for 4 MHz\r\n"
//			"2. Press 2 for 8 MHz\r\n");
//
//	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
//
//}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_handler();
	}


}


void Error_handler(void)
{
	while(1);
}

