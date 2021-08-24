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

/*
 * 文件引用
 */
#include "soft_uart.h"


/*
 * 全局变量
 */
SOFT_UART SoftUartData[SOFT_UART_NUM];

/**********************************************************************************************************************/
/*外部接口*/

/*写pin*/
static void SoftUart_WritePin(GPIO_TypeDef* GPIO,GPIO_Pin_TypeDef PIN,uint8_t bit_data)
{
    if(bit_data)
        GPIO->ODR |= (uint8_t)PIN;
    else
        GPIO->ODR &= (uint8_t)(~PIN);
}

/*读pin*/
static uint8_t SoftUart_ReadPin(GPIO_TypeDef* GPIO,GPIO_Pin_TypeDef PIN)
{
    return GPIO->IDR & (uint8_t)PIN;
}

/**********************************************************************************************************************/
/*串口初始化*/
void SoftUart_Init(uint8_t serial_id,GPIO_TypeDef *TX_GPIO,GPIO_Pin_TypeDef TX_PIN,GPIO_TypeDef *RX_GPIO,GPIO_Pin_TypeDef RX_PIN,uint8_t config)
{
	SoftUartData[serial_id].TX_GPIO = TX_GPIO;
	SoftUartData[serial_id].RX_GPIO = RX_GPIO;
	SoftUartData[serial_id].TX_PIN = TX_PIN;
	SoftUartData[serial_id].RX_PIN = RX_PIN;
	SoftUartData[serial_id].config = config;
	
	SoftUartData[serial_id].status = 0;
	SoftUartData[serial_id].rx_bit_cnt = 0;
	SoftUartData[serial_id].tx_bit_cnt = 0;
	SoftUartData[serial_id].rx_data_reg = 0;
	SoftUartData[serial_id].tx_data_reg = 0;
}

/*串口定时器服务程序*/
/*定时调用，调用周期：0.2 * (1 / 波特率)*/
void SoftUart_Handler(void)
{
	static uint16_t rx_shift_reg[SOFT_UART_NUM] = {0};
	static uint8_t rx_sample_point[SOFT_UART_NUM] = {0};
	static uint8_t baud_cnter = 0; /*5倍采样*/
	uint8_t i;
	uint8_t rx_pin_state;
	
	/*发送处理程序*/
	if(baud_cnter == 0){
		for(i = 0;i < SOFT_UART_NUM; i++){
			if(! (SoftUartData[i].config & SOFT_UART_CONFIG_SEND_EN)) /*发送使能位*/
				continue;
			
			if(SoftUartData[i].status & SOFT_UART_STATUS_TX_BUSY){
				if(SoftUartData[i].tx_bit_cnt > 0){
					SoftUart_WritePin(SoftUartData[i].TX_GPIO,SoftUartData[i].TX_PIN,SoftUartData[i].tx_data_reg & 0x01);
					SoftUartData[i].tx_data_reg = SoftUartData[i].tx_data_reg >> 1;
					SoftUartData[i].tx_bit_cnt --;
				}else{
					SoftUartData[i].status &= (~ SOFT_UART_STATUS_TX_BUSY);
				}
			}else{;}
		}
	}else{;}
	
	/*接收处理程序*/
	for(i = 0;i < SOFT_UART_NUM; i++){
		if(! (SoftUartData[i].config & SOFT_UART_CONFIG_RECEIVE_EN)) /*接收使能位*/
			continue;
		
		rx_pin_state = SoftUart_ReadPin(SoftUartData[i].RX_GPIO,SoftUartData[i].RX_PIN);
		
		/*检查是否有新的数据收到*/
		if(! (SoftUartData[i].status & SOFT_UART_STATUS_RX_BUSY)){
			rx_shift_reg[i] = rx_shift_reg[i] >> 1;
            if(rx_pin_state)
				rx_shift_reg[i] |= 0x8000;
			
			if((rx_shift_reg[i] & 0xFC00) == 0x1C00){
				SoftUartData[i].status |= SOFT_UART_STATUS_RX_BUSY;
				
				/*记录采样点*/
				rx_sample_point[i] = baud_cnter;
				
				/*设置接收计数（不含起始位）*/
				if(SoftUartData[i].config & SOFT_UART_CONFIG_PARITY_EN)
					SoftUartData[i].rx_bit_cnt = 8;
				else
					SoftUartData[i].rx_bit_cnt = 7;
				
				if(SoftUartData[i].config & SOFT_UART_CONFIG_STOP_BIT_2)
					SoftUartData[i].rx_bit_cnt += 2;
				else
					SoftUartData[i].rx_bit_cnt ++;
			}else{;}
		}else{
			if(baud_cnter == rx_sample_point[i]){
				rx_shift_reg[i] = rx_shift_reg[i] >> 1;
				
				if(rx_pin_state)
					rx_shift_reg[i] |= 0x8000;
				
				if(SoftUartData[i].rx_bit_cnt > 0)
					SoftUartData[i].rx_bit_cnt --;
				else{
					SoftUartData[i].rx_data_reg = rx_shift_reg[i]; /*存储接收数据*/
					SoftUartData[i].status &= (~ SOFT_UART_STATUS_RX_BUSY);
					SoftUartData[i].status |= SOFT_UART_STATUS_RX_DATA_VALID;
				}
			}else{;}
		}
	}
	
	/*波特率计数器*/
	if(baud_cnter < 4)
		baud_cnter ++;
	else
		baud_cnter = 0;
}

/*发送数据*/
/*返回0 - 发送失败；返回1 - 发送成功*/
uint8_t SoftUart_SendData(uint8_t serial_id,uint8_t tx_data)
{
	uint16_t itemp;
	uint8_t cnt_1;
	uint8_t i;
	if(SoftUartData[serial_id].status & SOFT_UART_STATUS_TX_BUSY)
		return 0;
	else{
		/*起始位*/
		SoftUartData[serial_id].tx_data_reg = 0; 
		
		/*8位数据位*/
		itemp = tx_data;
		SoftUartData[serial_id].tx_data_reg |= (itemp << 1); 
		SoftUartData[serial_id].tx_bit_cnt = 9;
		
		/*校验位*/
		/*统计1的个数*/
		if(SoftUartData[serial_id].config & SOFT_UART_CONFIG_PARITY_EN){ 
            itemp = tx_data;
            cnt_1 = 0;
            for(i = 0;i < 8;i ++){
                if(itemp & 0x01)
                    cnt_1 ++;
                itemp = itemp >> 1;
            }
            
			if(SoftUartData[serial_id].config & SOFT_UART_CONFIG_PARITY_EVEN){ /*偶校验*/
				if(cnt_1 % 2)
					SoftUartData[serial_id].tx_data_reg |= 0x0200;
			}else{ /*奇校验*/
				if(! (cnt_1 % 2))
					SoftUartData[serial_id].tx_data_reg |= 0x0200;
			}
			SoftUartData[serial_id].tx_bit_cnt ++;
		}else{;}
		
		/*停止位*/
		if(SoftUartData[serial_id].config & SOFT_UART_CONFIG_STOP_BIT_2){ /*2位停止位*/
			SoftUartData[serial_id].tx_data_reg |= (0x0002 << SoftUartData[serial_id].tx_bit_cnt);
			SoftUartData[serial_id].tx_bit_cnt += 2;
		}else{ /*1位停止位*/
			SoftUartData[serial_id].tx_data_reg |= (0x0001 << SoftUartData[serial_id].tx_bit_cnt);
			SoftUartData[serial_id].tx_bit_cnt ++;
		}
		
		/*设置发送忙状态*/
		SoftUartData[serial_id].status |= SOFT_UART_STATUS_TX_BUSY;
		
		return 1;
	}
}

/*检查发送忙状态*/
uint8_t SoftUart_CheckSendBusy(uint8_t serial_id)
{
	return SoftUartData[serial_id].status & SOFT_UART_STATUS_TX_BUSY;
}

/*接收数据处理*/
/*返回0，没有接收到数据；返回1，接收到数据*/
/*error:b0 - 校验错误状态，b1 - 帧错误状态*/
uint8_t SoftUart_ReceiveData(uint8_t serial_id,uint8_t *data,uint8_t *error)
{
	uint16_t itemp;
    uint8_t bit_cnt;
	uint8_t cnt_1;
    uint8_t i;
    
    *error = 0;
	if(SoftUartData[serial_id].status & SOFT_UART_STATUS_RX_DATA_VALID){
		/*结束位判断*/
		*error &= (~ SOFT_UART_ERROR_FRAMING);
		if(SoftUartData[serial_id].config & SOFT_UART_CONFIG_STOP_BIT_2){
			bit_cnt = 2;
			if((NULL != error) && ((SoftUartData[serial_id].rx_data_reg & 0xC000) != 0xC000))
				*error |= SOFT_UART_ERROR_FRAMING;
		}else{
			bit_cnt = 1;
			if((NULL != error) && ((SoftUartData[serial_id].rx_data_reg & 0x8000) != 0x8000))
				*error |= SOFT_UART_ERROR_FRAMING;
		}
		
		/*校验位判断，并获取数据*/
		*error &= (~ SOFT_UART_ERROR_PARITY);
        
        itemp = SoftUartData[serial_id].rx_data_reg << bit_cnt;
		if(SoftUartData[serial_id].config & SOFT_UART_CONFIG_PARITY_EN){
            itemp &= 0xFF80;
            *data = (itemp >> 7) & 0xFF; /*获取数据*/
            
			cnt_1 = 0;
			for(i = 0;i < 9;i ++){
				if(itemp & 0x8000)
					cnt_1 ++;
				itemp = itemp << 1;
			}
			
			if(SoftUartData[serial_id].config & SOFT_UART_CONFIG_PARITY_EVEN){ /*偶校验*/
				if((cnt_1 % 2))
					*error |= SOFT_UART_ERROR_PARITY;
			}else{
				if(! (cnt_1 % 2))
					*error |= SOFT_UART_ERROR_PARITY;
			}
		}else{
            itemp &= 0xFF00;
            *data = (itemp >> 8) & 0xFF; /*获取数据*/
        }
		
		/*清接收标记*/
		SoftUartData[serial_id].status &= (~ SOFT_UART_STATUS_RX_DATA_VALID);
		
		return 1;
	}else
		return 0;
}

/*检查接收忙状态*/
uint8_t SoftUart_CheckReceiveBusy(uint8_t serial_id)
{
	return SoftUartData[serial_id].status & SOFT_UART_STATUS_RX_BUSY;
}

/*检查接收数据有效状态*/
uint8_t SoftUart_CheckReceiveDataValid(uint8_t serial_id)
{
	return SoftUartData[serial_id].status & SOFT_UART_STATUS_RX_DATA_VALID;
}

/**********************************************************************************************************************/
/*串口发送字符串*/
void SoftUart_Puts(uint8_t serial_id,char * str)
{
	char *p;
	p = str;
	while(*p != '\0'){
		if(SoftUart_SendData(serial_id,*p))
			p ++;
	}
}


