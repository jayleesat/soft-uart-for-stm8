/*
 * common.h
 *
 *  Created on: 2017年5月28日
 *      Author: LJ
 */

#ifndef COMMON_H_
#define COMMON_H_

/*
 * 文件引用
 */
#include <stdio.h>
#include <string.h>
#include "stm8s.h"

/********************************************************************/
#define DEBUG                               1 /*定义调试模式*/

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
#define POWER_ON                            0x55    /*单机加电*/
#define POWER_OFF                           0x00    /*单机断电*/

#define LIGHT_DOWN                          0x00    /*灯灭*/
#define LIGHT_UP                            0x55    /*灯亮*/

#define DEACTIVE                            0x00    /*动作无效*/
#define ACTIVE                              0x55    /*动作有效*/

#define UNLOCK                              0x00    /*未锁定*/
#define LOCKED                              0x55    /*锁定*/

#define UNAVAILABLE                         0x00    /*不可用*/
#define AVAILABLE                           0x55    /*可用*/

#define INVALID                             0x00    /*无效*/
#define VALID                               0x55    /*有效*/

#define IDLE                                0x00    /*空闲*/
#define WORKING                             0x55    /*工作中*/

#endif /* COMMON_H_ */
