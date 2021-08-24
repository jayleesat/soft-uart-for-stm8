/*
 * ���ƣ�Main.c
 * ���ܣ�������
 * ���ߣ�JayLee
 * ���ʣ�jayleesat@163.com
 * ʱ�䣺20191127
 * �汾��Ver1.0
 * ���ļ�¼��
 *
 *
 *
 *
 */

/*
 * �ļ�����
 */
#include "stm8s_conf.h"
#include "Driver.h"
#include "soft_uart.h"

/*
 * ȫ�ֱ���
 */
 
/*������ڶ�ʱ����ʼ��*/
/*64bps����ʱʱ��(0.2)*(1/64) = 3125 uS����ʱֵ = ��ʱʱ��*16:49999*/
/*128bps����ʱʱ��(0.2)*(1/128) = 1562.5 uS����ʱֵ = ��ʱʱ��*16:24999*/
/*256bps����ʱʱ��(0.2)*(1/256) = 781.25 uS����ʱֵ = ��ʱʱ��*16:12499*/
/*1200bps����ʱʱ��(0.2)*(1/1200) = 166.67 uS,��ʱֵ = ��ʱʱ��*16:2666*/
/*2400bps����ʱʱ��(0.2)*(1/2400) = 83.33 uS,��ʱֵ = ��ʱʱ��*16:1332*/
void SoftUart_Timer_Init(void)
{
	TIM2_TimeBaseInit(TIM2_PRESCALER_1,24999);
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE); /*ʹ���ж�*/
	TIM2_SetCounter(0);
	TIM2_Cmd(ENABLE);
}

/*������*/
int main(void)
{
	/*Ӳ����ʼ��*/
	/* Clock divider to HSI/1 */
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

	/* Configure the system clock to use HSI clock source and to run at 16Mhz */
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	
	/*���� ����*/
    GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);
    
    /*���� ����*/
    GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);
    
    GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);
	
	SoftUart_Timer_Init();
    
    /*������ڳ�ʼ��*/
    SoftUart_Init(0,GPIOD,GPIO_PIN_3,NULL,GPIO_PIN_0,SOFT_UART_CONFIG_SEND_EN | SOFT_UART_CONFIG_PARITY_EN | SOFT_UART_CONFIG_PARITY_EVEN | SOFT_UART_CONFIG_STOP_BIT_2); /*MCU����ָ�����*/
    
    SoftUart_Init(1,NULL,GPIO_PIN_0,GPIOC,GPIO_PIN_5,SOFT_UART_CONFIG_RECEIVE_EN | SOFT_UART_CONFIG_PARITY_EN | SOFT_UART_CONFIG_PARITY_EVEN | SOFT_UART_CONFIG_STOP_BIT_2); /*MCU�������ݣ����գ�slot5*/
    SoftUart_Init(2,NULL,GPIO_PIN_0,GPIOC,GPIO_PIN_4,SOFT_UART_CONFIG_RECEIVE_EN | SOFT_UART_CONFIG_PARITY_EN | SOFT_UART_CONFIG_PARITY_EVEN | SOFT_UART_CONFIG_STOP_BIT_2); /*MCU�������ݣ����գ�slot4*/
    SoftUart_Init(3,NULL,GPIO_PIN_0,GPIOC,GPIO_PIN_3,SOFT_UART_CONFIG_RECEIVE_EN | SOFT_UART_CONFIG_PARITY_EN | SOFT_UART_CONFIG_PARITY_EVEN | SOFT_UART_CONFIG_STOP_BIT_2); /*MCU�������ݣ����գ�slot3*/
    SoftUart_Init(4,NULL,GPIO_PIN_0,GPIOB,GPIO_PIN_4,SOFT_UART_CONFIG_RECEIVE_EN | SOFT_UART_CONFIG_PARITY_EN | SOFT_UART_CONFIG_PARITY_EVEN | SOFT_UART_CONFIG_STOP_BIT_2); /*MCU�������ݣ����գ�slot2*/
    SoftUart_Init(5,NULL,GPIO_PIN_0,GPIOB,GPIO_PIN_5,SOFT_UART_CONFIG_RECEIVE_EN | SOFT_UART_CONFIG_PARITY_EN | SOFT_UART_CONFIG_PARITY_EVEN | SOFT_UART_CONFIG_STOP_BIT_2); /*MCU�������ݣ����գ�slot1*/

	enableInterrupts();

	while(1)
	{

		/*������ڲ��Կ�ʼ*/
        {
            uint8_t buff[16];
            uint8_t rcv_data,rcv_err;
            
            if(SoftUart_ReceiveData(1,&rcv_data,&rcv_err)){
                SoftUart_SendData(0,rcv_data);
                while(SoftUart_CheckSendBusy(0));
                SoftUart_SendData(0,rcv_err);
                while(SoftUart_CheckSendBusy(0));
            }
            
            if(SoftUart_ReceiveData(2,&rcv_data,&rcv_err)){
                SoftUart_SendData(0,rcv_data);
                while(SoftUart_CheckSendBusy(0));
                SoftUart_SendData(0,rcv_err);
                while(SoftUart_CheckSendBusy(0));
            }
            
            if(SoftUart_ReceiveData(3,&rcv_data,&rcv_err)){
                SoftUart_SendData(0,rcv_data);
                while(SoftUart_CheckSendBusy(0));
                SoftUart_SendData(0,rcv_err);
                while(SoftUart_CheckSendBusy(0));
            }
            
            if(SoftUart_ReceiveData(4,&rcv_data,&rcv_err)){
                SoftUart_SendData(0,rcv_data);
                while(SoftUart_CheckSendBusy(0));
                SoftUart_SendData(0,rcv_err);
                while(SoftUart_CheckSendBusy(0));
            }
            
            if(SoftUart_ReceiveData(5,&rcv_data,&rcv_err)){
                SoftUart_SendData(0,rcv_data);
                while(SoftUart_CheckSendBusy(0));
                SoftUart_SendData(0,rcv_err);
                while(SoftUart_CheckSendBusy(0));
            }
            
            //strcpy((char *)buff,"Test.\r\n");
            //SoftUart_Puts(0,(char *)buff);
        }
        /*������ڲ��Խ���*/
	}
}
