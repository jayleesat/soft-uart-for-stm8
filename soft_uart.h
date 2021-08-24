/*
 * 名称：soft_uart
 * 功能：软件串口模块（数据位宽 8）
 * 作者：JayLee
 * 电邮：jayleesat@163.com
 * 时间：20210824
 * 版本：Ver1.0
 * 更改记录：
 *
 *
 *
 *
 */

#ifndef SOFT_UART_H_
#define SOFT_UART_H_

/*
 * 文件引用
 */
#include "Common.h"

/*
 * 定义
 */
#define SOFT_UART_NUM               	6 /*支持的串口总数目*/

typedef struct _tagSoft_Uart
{
	/*GPIO*/
	GPIO_TypeDef						*TX_GPIO;
	GPIO_TypeDef						*RX_GPIO;
	GPIO_Pin_TypeDef					TX_PIN;
	GPIO_Pin_TypeDef					RX_PIN;
	
	/*设置参数*/
	uint8_t								config; /*b0-校验使能位（1：使能）;b1-校验类型（0：奇校验，1：偶校验）;b2-停止位（0：1个停止位，1：2个停止位）;b3-接收使能（1：使能）;b4-发送使能（1：使能）*/
	
	/*状态*/
	uint8_t								status; /*b0-发送状态（1：忙）;b1-接收状态（1：忙）;b2-接收数据有效状态（1：有效）*/
	
	/*内部控制寄存器*/
	uint8_t								rx_bit_cnt; /*接收bit计数*/
	uint8_t								tx_bit_cnt; /*发送bit计数*/
	uint16_t							rx_data_reg; /*接收数据缓存*/
	uint16_t							tx_data_reg; /*发送数据缓存*/
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
 * 函数声明
 */
void SoftUart_Init(uint8_t serial_id,GPIO_TypeDef *TX_GPIO,GPIO_Pin_TypeDef TX_PIN,GPIO_TypeDef *RX_GPIO,GPIO_Pin_TypeDef RX_PIN,uint8_t config);
void SoftUart_Handler(void);
uint8_t SoftUart_SendData(uint8_t serial_id,uint8_t tx_data);
uint8_t SoftUart_CheckSendBusy(uint8_t serial_id);
uint8_t SoftUart_ReceiveData(uint8_t serial_id,uint8_t *data,uint8_t *error);
void SoftUart_Puts(uint8_t serial_id,char * str);

#endif
