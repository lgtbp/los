/*----------------------------------------------------------------------------/
 /  los -  system module  R0.1.1
 /-----------------------------------------------------------------------------/
 /
 / Copyright (c) 2014-2017 LP电子,All Rights Reserved.Author's mailbox:lgtbp@126.com.
 /
 / GNU LESSER GENERAL PUBLIC LICENSE  Version 2.1
 /----------------------------------------------------------------------------*/
#ifndef __LOS_T_H__
#define __LOS_T_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "string.h"
#include "stdint.h"
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>
#define los_api_vs 1
#define los_bulid_vs 1
#define REG_NUM 32
#define REG_RETURN 14

#define HEAP_REG 0
#define CREGLEN 4 * 7
#define AREGLEN (4 * REG_NUM)
#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

	typedef void (*los_cmd)(void);
	typedef union _cpu_reg_t
	{
		uint8_t u8;
		int8_t s8;
		uint16_t u16;
		int16_t s16;
		uint32_t u32;
		int32_t s32;
		//	float f32;
		uint8_t data[4];
	} cpu_reg_t;

	typedef struct _los_irq_len_t
	{
		uint16_t arg;
		uint16_t los_irq;
		jmp_buf jbuf;
		uint8_t *pc;
		cpu_reg_t reg[REG_NUM];

	} los_irq_len_t;
	typedef struct _los_t
	{
		uint16_t arg;
		uint16_t los_irq;
		jmp_buf jbuf;
		uint8_t *pc;
		cpu_reg_t reg[REG_NUM];
		uint8_t *ram;
		uint32_t ram_len;
		uint32_t lvar_start;
		uint32_t stack_end;
		uint32_t code_len;
		uint32_t data_len;
		uint32_t bss_len;
		uint8_t *code;
	} los_t;
	typedef void (*fun_os)(void);
	////////////////////////////////////////////////////////////////////////////////////////////
	/**
获取函数参数num从1开始
los_get_arg:参数
los_get_argp:参数地址
Get function parameter num from 1
*/
	uint8_t *los_get_arg(uint32_t num);
	uint8_t *los_get_argp(uint32_t num);
	/**
给los的返回参数
*/
	void los_set_return(uint32_t data);
	/**
获取los数据地址
*/
	uint8_t *los_get_datap(void);
/**
移植请修改内存管理api
Please modify the memory management API
*/
#ifndef MEM_CUSTOM
#define lpram_malloc malloc
#define lpram_free free
#endif
	/**
los中断函数,有返回值
*/
	int los_irq(int num);
	/**
设置用户函数 
Set user function
*/
	void los_set_function(fun_os *f);
	/**
运行los文件
addr参数:los文件在ram或rom中的地址
Run los file
Addr parameter: the address of the los file in ram or rom
*/
	uint32_t los_app_first(uint32_t *addr, uint8_t inter);

	/** note
运行los
*/
	int los_run(void);
#ifdef __cplusplus
}
#endif

#endif /*  */
