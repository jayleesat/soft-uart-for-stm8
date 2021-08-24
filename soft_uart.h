/*
 * ���ƣ�soft_uart
 * ���ܣ��������ģ�飨����λ�� 8��
 * ���ߣ�JayLee
 * ���ʣ�jayleesat@163.com
 * ʱ�䣺20210824
 * �汾��Ver1.0
 * ���ļ�¼��
 *
 *
 *
 *
 */

#ifndef SOFT_UART_H_
#define SOFT_UART_H_

/*
 * �ļ�����
 */
#include "Common.h"

/*
 * ����
 */
#define SOFT_UART_NUM               	6 /*֧�ֵĴ�������Ŀ*/

typedef struct _tagSoft_Uart
{
	/*GPIO*/
	GPIO_TypeDef						*TX_GPIO;
	GPIO_TypeDef						*RX_GPIO;
	GPIO_Pin_TypeDef					TX_PIN;
	GPIO_Pin_TypeDef					RX_PIN;
	
	/*���ò���*/
	uint8_t								config; /*b0-У��ʹ��λ��1��ʹ�ܣ�;b1-У�����ͣ�0����У�飬1��żУ�飩;b2-ֹͣλ��0��1��ֹͣλ��1��2��ֹͣλ��;b3-����ʹ�ܣ�1��ʹ�ܣ�;b4-����ʹ�ܣ�1��ʹ�ܣ�*/
	
	/*״̬*/
	uint8_t								status; /*b0-����״̬��1��æ��;b1-����״̬��1��æ��;b2-����������Ч״̬��1����Ч��*/
	
	/*�ڲ����ƼĴ���*/
	uint8_t								rx_bit_cnt; /*����bit����*/
	uint8_t								tx_bit_cnt; /*����bit����*/
	uint16_t							rx_data_reg; /*�������ݻ���*/
	uint16_t							tx_data_reg; /*�������ݻ���*/
}SOFT_UART;

#define SOFT_UART_CONFIG_PARITY_EN		0x01
#define SOFT_UART_CONFIG_PARITY_EVEN	0x02
#define SOFT_UART_CONFIG_STOP_BIT_2 	0x04
#define SOFT_UART_CONFIG_RECEIVE_EN		0x08
#define SOFT_UART_CONFIG_SEND_EN		0x10

#define SOFT_UART_STATUS_TX_BUSY		0x01
#define SOFT_UART_STATUS_RX_BUSY		0x02
#define SOFT_UART_STATUS_RX_DATA_VALID	0x04

#define SOFT_UART_ERROR_PARITY			0x01
#define SOFT_UART_ERROR_FRAMING			0x02

/*
 * ��������
 */
void SoftUart_Init(uint8_t serial_id,GPIO_TypeDef *TX_GPIO,GPIO_Pin_TypeDef TX_PIN,GPIO_TypeDef *RX_GPIO,GPIO_Pin_TypeDef RX_PIN,uint8_t config);
void SoftUart_Handler(void);
uint8_t SoftUart_SendData(uint8_t serial_id,uint8_t tx_data);
uint8_t SoftUart_CheckSendBusy(uint8_t serial_id);
uint8_t SoftUart_ReceiveData(uint8_t serial_id,uint8_t *data,uint8_t *error);
void SoftUart_Puts(uint8_t serial_id,char * str);

#endif
