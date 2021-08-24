/*
 * common.h
 *
 *  Created on: 2017��5��28��
 *      Author: LJ
 */

#ifndef COMMON_H_
#define COMMON_H_

/*
 * �ļ�����
 */
#include <stdio.h>
#include <string.h>
#include "stm8s.h"

/********************************************************************/
#define DEBUG                               1 /*�������ģʽ*/

/********************************************************************/
#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned int
#endif

#ifndef uint32_t
#define uint32_t unsigned long
#endif

/********************************************************************/
#define POWER_ON							0x55	/*�����ӵ�*/
#define POWER_OFF							0x00	/*�����ϵ�*/

#define LIGHT_DOWN							0x00	/*����*/
#define LIGHT_UP							0x55	/*����*/

#define DEACTIVE							0x00	/*������Ч*/
#define ACTIVE								0x55	/*������Ч*/

#define UNLOCK								0x00	/*δ����*/
#define LOCKED								0x55	/*����*/

#define UNAVAILABLE							0x00	/*������*/
#define AVAILABLE							0x55	/*����*/

#define INVALID								0x00	/*��Ч*/
#define VALID								0x55	/*��Ч*/

#define IDLE								0x00	/*����*/
#define WORKING								0x55	/*������*/

#endif /* COMMON_H_ */
