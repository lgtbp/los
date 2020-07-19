/*----------------------------------------------------------------------------/
 /  los -  system module  R0.1.1
 /-----------------------------------------------------------------------------/
 /
 / Copyright (c) 2014-2017 LP电子,All Rights Reserved.Author's mailbox:lgtbp@126.com.
 /
 / GNU LESSER GENERAL PUBLIC LICENSE  Version 2.1
 /----------------------------------------------------------------------------*/
#include "los.h"
static fun_os *func;
static uint8_t arg;
static uint8_t *gram;
static los_t *lp;
#define LOS_DEBUG 1
struct heap
{
	uint8_t los[4];
	uint16_t version;
	uint16_t pad_len;
	uint32_t stack_len;
	uint32_t heap_len;
	uint32_t txt_len;
	uint32_t global_len;
	uint32_t gvar_init_len;
	uint32_t code_len;
	uint32_t code_main;
};
static void cmd_0(void)
{
	longjmp(lp->jbuf, 1);
}
static void cmd_1(void)
{
	lp->stack_end -= CREGLEN;
	memcpy(lp->reg, &gram[lp->stack_end], CREGLEN);
	lp->stack_end -= 8;
	lp->pc = (uint8_t *)(*(uint64_t *)&gram[lp->stack_end]);
}
static void cmd_2(void)
{
	lp->reg[HEAP_REG].u32 -= arg;
#ifdef LOS_DEBUG
	if (lp->stack_end > lp->reg[HEAP_REG].u32)
	{
		printf("2no stack\n");
		cmd_0();
	}
#endif
}
static void cmd_3(void)
{
	lp->reg[HEAP_REG].u32 += arg;
	cmd_1();
}
static void cmd_4(void)
{
	lp->reg[HEAP_REG].u32 -= arg + (*(uint16_t *)lp->pc << 8);
	lp->pc += 2;
#ifdef LOS_DEBUG
	if (lp->stack_end > lp->reg[HEAP_REG].u32)
	{
		printf("4 no stack\n");
		cmd_0();
	}
#endif
}
static void cmd_5(void)
{
	lp->reg[HEAP_REG].u32 += arg + (*(uint16_t *)lp->pc << 8);
	lp->pc += 2;
	cmd_1();
}
static void cmd_6(void)
{
	cmd_0();
	lp->reg[arg & 0xf].u32 = arg >> 4;
}
static void cmd_7(void)
{
	lp->reg[arg].u32 = *(uint16_t *)lp->pc;
	lp->pc += 2;
}
static void cmd_8(void)
{
	memcpy(lp->reg[arg].data, lp->pc, 4);
	lp->pc += 4;
}
static void cmd_9(void)
{
	*(uint32_t *)(&gram[lp->reg[arg & 0x7].u32 + *(uint16_t *)lp->pc]) = lp->reg[arg >> 3].u32;
	lp->pc += 2;
}
static void cmd_10(void)
{
	gram[lp->reg[arg & 0x7].u32 + *(uint16_t *)lp->pc] = lp->reg[arg >> 3].data[0];
	lp->pc += 2;
}
static void cmd_11(void)
{
	memcpy(&gram[lp->reg[arg & 0x7].u32 + *(uint16_t *)lp->pc], lp->reg[arg >> 3].data, 2);
	lp->pc += 2;
}
static void cmd_12(void)
{
	lp->reg[arg >> 3].u32 = *(uint32_t *)&gram[lp->reg[arg & 0x7].s32 + *(uint16_t *)lp->pc];
	lp->pc += 2;
}
static void cmd_13(void)
{
	lp->reg[arg >> 3].u32 = *(uint8_t *)&gram[lp->reg[arg & 0x7].s32 + *(uint16_t *)lp->pc];
	lp->pc += 2;
}
static void cmd_14(void)
{
	lp->reg[arg >> 3].u32 = *(uint16_t *)&gram[lp->reg[arg & 0x7].u32 + *(uint16_t *)lp->pc];
	lp->pc += 2;
}
static void cmd_15(void)
{
	lp->reg[arg >> 3].s32 = *(int8_t *)&gram[lp->reg[arg & 0x7].u32 + *(uint16_t *)lp->pc];
	lp->pc += 2;
}
static void cmd_16(void)
{
	lp->reg[arg >> 3].s32 = *(int16_t *)&gram[lp->reg[arg & 0x7].u32 + *(uint16_t *)lp->pc];
	lp->pc += 2;
}
static void cmd_17(void)
{
	lp->reg[arg >> 3].u32 = lp->reg[arg & 0x7].u32;
}
static void cmd_18(void)
{
	lp->reg[arg & 0xf].u32 = (arg >> 4) + (*(uint16_t *)lp->pc << 4);
	lp->pc += 2;
}
static void cmd_19(void)
{
	lp->reg[arg & 0xf].u32 = 0xfffff + (arg >> 4) + (*(uint16_t *)lp->pc << 4);
	lp->pc += 2;
}
static void cmd_20(void)
{
	cmd_0();
}

static void cmd_21(void)
{
	uint32_t call_addr = arg + (*(uint16_t *)lp->pc << 8);
	lp->pc += 2;
	*(uint64_t *)&gram[lp->stack_end] = (uint64_t)(&lp->pc[0]);
	lp->stack_end += 8;
	lp->pc = &lp->code[call_addr];
	memcpy(&gram[lp->stack_end], lp->reg, CREGLEN);
	lp->stack_end += CREGLEN;
#ifdef LOS_DEBUG
	if (lp->stack_end > lp->reg[HEAP_REG].u32)
	{
		printf("no stack\n");
		cmd_0();
	}
#endif
}
static void cmd_22(void)
{
	*(uint64_t *)&gram[lp->stack_end] = (uint64_t)lp->pc;
	lp->stack_end += 8;
	lp->pc = &lp->code[lp->reg[arg].u32];
	memcpy(&gram[lp->stack_end], lp->reg, CREGLEN);
	lp->stack_end += CREGLEN;
#ifdef LOS_DEBUG
	if (lp->stack_end > lp->reg[HEAP_REG].u32)
	{
		printf("no stack\n");
		cmd_0();
	}
#endif
}
static void cmd_23(void)
{
	lp->reg[arg >> 4].u32 += lp->reg[arg & 0xf].u32;
}
static void cmd_24(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 + (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_25(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 + *(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16);
	lp->pc += 4;
}
static void cmd_26(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 - lp->reg[arg >> 4].u32;
}
static void cmd_27(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 - (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_28(void)
{
	printf("28\n");
	cmd_0();
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 - ((*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16)));
	lp->pc += 4;
}
static void cmd_29(void)
{
	lp->reg[arg >> 4].u32 ^= lp->reg[arg & 0xf].u32;
}
static void cmd_30(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 ^ (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_31(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 ^ (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_32(void)
{
	lp->reg[arg >> 4].u32 |= lp->reg[arg & 0xf].u32;
}
static void cmd_33(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 | (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_34(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 | (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_35(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 << lp->reg[arg >> 4].u32;
}
static void cmd_36(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 << (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_37(void)
{
	printf("37\n");
	cmd_0();
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 << (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_38(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> lp->reg[arg >> 4].u32;
}
static void cmd_39(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_40(void)
{
	printf("40\n");
	cmd_0();
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_41(void)
{
	uint32_t mov = lp->reg[arg >> 4].u32;
	if (lp->reg[arg & 0xf].u32 & 0x80000000)
	{
		lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> mov;
		if (mov > 31)
		{
			lp->reg[arg >> 4].u32 = 0x80000000;
		}
		else
			lp->reg[arg >> 4].u32 |= (0xffffffff << (32 - mov));
	}
	else
		lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> lp->reg[arg >> 4].u32;
}
static void cmd_42(void)
{
	uint16_t mov = (*(uint16_t *)lp->pc);
	if (lp->reg[arg & 0xf].u32 & 0x80000000)
	{
		lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> mov;
		lp->reg[arg >> 4].u32 |= (0xffffffff << (32 - mov));
#ifdef LOS_DEBUG
		if (mov > 31)
		{
			printf("42\n");
			cmd_0();
		}
#endif
	}
	else
		lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> mov;
	lp->pc += 2;
}
static void cmd_43(void)
{
	printf("43\n");
	cmd_0();
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 >> (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_44(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 * lp->reg[arg >> 4].u32;
}
static void cmd_45(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 * (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_46(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 * (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_47(void)
{
	printf("46\n");
	cmd_0();
}
static void cmd_48(void)
{
	printf("47\n");
	cmd_0();
}
static void cmd_49(void)
{
	printf("48\n");
	cmd_0();
}
static void cmd_50(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 / lp->reg[arg >> 4].u32;
}
static void cmd_51(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 / (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_52(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 / (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_53(void)
{
#ifdef LOS_DEBUG
	if (0 == lp->reg[arg >> 4].s32)
	{
		printf("53\n");
		cmd_0();
	}
#endif
	lp->reg[arg >> 4].s32 = lp->reg[arg & 0xf].s32 / lp->reg[arg >> 4].s32;
}
static void cmd_54(void)
{
	lp->reg[arg >> 4].s32 = lp->reg[arg & 0xf].s32 / (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_55(void)
{
	printf("55\n");
	cmd_0();
	lp->reg[arg >> 4].s32 = lp->reg[arg & 0xf].s32 / (int32_t)(*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_56(void)
{
	lp->reg[arg >> 4].u32 &= lp->reg[arg & 0xf].u32;
}
static void cmd_57(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 & (*(uint16_t *)lp->pc);
	lp->pc += 2;
}
static void cmd_58(void)
{
	lp->reg[arg >> 4].u32 = lp->reg[arg & 0xf].u32 & (*(uint16_t *)lp->pc + (*(uint16_t *)(lp->pc + 2) << 16));
	lp->pc += 4;
}
static void cmd_59(void)
{
	lp->pc += (uint8_t)arg;
}
static void cmd_60(void)
{
	lp->pc -= (uint8_t)arg;
}
static void cmd_61(void)
{
	lp->reg[arg >> 5].u32 = lp->reg[arg & 0x1F].u32;
}
static void cmd_62(void)
{
	lp->pc += *(int16_t *)(lp->pc);
}
static void cmd_63(void)
{
	lp->reg[*lp->pc].u32 = lp->reg[arg].u32;
	lp->pc += 2;
}
static void cmd_64(void)
{
	printf("64\n");
	cmd_0();
}
static void cmd_65(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg & 0xf].s32 == lp->reg[arg >> 4].s32)
		lp->pc += imm;
}
static void cmd_66(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].s32 == cmp)
		lp->pc += imm;
}
static void cmd_67(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg & 0xf].s32 != lp->reg[arg >> 4].s32)
		lp->pc += imm;
}
static void cmd_68(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].s32 != cmp)
		lp->pc += imm;
}
static void cmd_69(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].u32 > lp->reg[arg & 0xf].u32)
		lp->pc += imm;
}
static void cmd_70(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].u32 > cmp)
		lp->pc += imm;
}
static void cmd_71(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].u32 >= lp->reg[arg & 0xf].u32)
		lp->pc += imm;
}
static void cmd_72(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].u32 >= cmp)
		lp->pc += imm;
}
static void cmd_73(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].s32 > lp->reg[arg & 0xf].s32)
		lp->pc += imm;
}
static void cmd_74(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].s32 > cmp)
		lp->pc += imm;
}
static void cmd_75(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].s32 >= lp->reg[arg & 0xf].s32)
		lp->pc += imm;
}
static void cmd_76(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].s32 >= cmp)
		lp->pc += imm;
}
static void cmd_77(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].u32 < lp->reg[arg & 0xf].u32)
		lp->pc += imm;
}
static void cmd_78(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].u32 < cmp)
		lp->pc += imm;
}
static void cmd_79(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].u32 <= lp->reg[arg & 0xf].u32)
		lp->pc += imm;
}
static void cmd_80(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].u32 <= cmp)
		lp->pc += imm;
}
static void cmd_81(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].s32 < lp->reg[arg & 0xf].s32)
		lp->pc += imm;
}
static void cmd_82(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].s32 < cmp)
		lp->pc += imm;
}
static void cmd_83(void)
{
	int32_t imm = *(int16_t *)(lp->pc);
	lp->pc += 2;
	if (lp->reg[arg >> 4].s32 <= lp->reg[arg & 0xf].s32)
		lp->pc += imm;
}
static void cmd_84(void)
{
	int32_t cmp = *(int16_t *)(lp->pc);
	int32_t imm = *(int16_t *)(&lp->pc[2]);
	lp->pc += 4;
	if (lp->reg[arg].s32 <= cmp)
		lp->pc += imm;
}
static void cmd_85(void)
{
	printf("85\n");
	cmd_0();
}
static void cmd_86(void)
{
	printf("86\n");
	cmd_0();
}
static void cmd_87(void)
{
	lp->reg[arg & 0xf].s32 = (int32_t)((arg >> 4) - 16);
}
static void cmd_88(void)
{
	int len = ((*(uint16_t *)lp->pc) << 8) + arg;
	lp->pc += 2;
	if (func[len])
		func[len]();
}
static void cmd_89(void)
{
	lp->reg[HEAP_REG].u32 += arg;
	longjmp(lp->jbuf, 89);
}
const los_cmd los_cmd_api[] = {
	cmd_0,
	cmd_1,
	cmd_2,
	cmd_3,
	cmd_4,
	cmd_5,
	cmd_6,
	cmd_7,
	cmd_8,
	cmd_9,
	cmd_10,
	cmd_11,
	cmd_12,
	cmd_13,
	cmd_14,
	cmd_15,
	cmd_16,
	cmd_17,
	cmd_18,
	cmd_19,
	cmd_20,
	cmd_21,
	cmd_22,
	cmd_23,
	cmd_24,
	cmd_25,
	cmd_26,
	cmd_27,
	cmd_28,
	cmd_29,
	cmd_30,
	cmd_31,
	cmd_32,
	cmd_33,
	cmd_34,
	cmd_35,
	cmd_36,
	cmd_37,
	cmd_38,
	cmd_39,
	cmd_40,
	cmd_41,
	cmd_42,
	cmd_43,
	cmd_44,
	cmd_45,
	cmd_46,
	cmd_47,
	cmd_48,
	cmd_49,
	cmd_50,
	cmd_51,
	cmd_52,
	cmd_53,
	cmd_54,
	cmd_55,
	cmd_56,
	cmd_57,
	cmd_58,
	cmd_59,
	cmd_60,
	cmd_61,
	cmd_62,
	cmd_63,
	cmd_64,
	cmd_65,
	cmd_66,
	cmd_67,
	cmd_68,
	cmd_69,
	cmd_70,
	cmd_71,
	cmd_72,
	cmd_73,
	cmd_74,
	cmd_75,
	cmd_76,
	cmd_77,
	cmd_78,
	cmd_79,
	cmd_80,
	cmd_81,
	cmd_82,
	cmd_83,
	cmd_84,
	cmd_85,
	cmd_86,
	cmd_87,
	cmd_88,
	cmd_89,
};
int los_irq(int num)
{
	int res;
	if (0 == lp->los_irq)
		return 0;
	lp->arg = arg;
	memcpy(&gram[lp->stack_end], &lp->arg, sizeof(los_irq_len_t));
	lp->stack_end += sizeof(los_irq_len_t);
	lp->pc = lp->code;
	res = setjmp(lp->jbuf);
	if (res)
	{
		lp->stack_end -= sizeof(los_irq_len_t);
		memcpy(&lp->arg, &gram[lp->stack_end], sizeof(los_irq_len_t));
		arg = lp->arg;
		return 0;
	}
	lp->reg[8].u32 = num;
	res = los_run();
	return res;
}
void los_set_function(fun_os *f)
{
	func = f;
}
void los_set_return(uint32_t data)
{
	lp->reg[REG_RETURN].u32 = data;
}
uint8_t *los_get_arg(uint32_t num)
{
	return (uint8_t *)(&lp->reg[8 + num].u32);
}
uint8_t *los_get_argp(uint32_t num)
{
	return (uint8_t *)(&gram[lp->reg[8 + num].u32]);
}
uint8_t *los_get_datap(void)
{
	return gram;
}
static uint8_t los_init_code(los_t *lp, uint32_t *data, uint8_t inter)
{
	uint32_t temp = 0;
	struct heap *los_heap;
	los_heap = (struct heap *)data;
	los_heap->stack_len = 1024 * 12;
	printf("size %d\r\n", sizeof(struct heap));
	printf("version=%x\r\n", los_heap->version);
	printf("pad_len=%d\r\n", los_heap->pad_len);
	printf("stack_len=%d\r\n", los_heap->stack_len);
	printf("heap_len=%d\r\n", los_heap->heap_len);
	printf("txt_len=%d\r\n", los_heap->txt_len);
	printf("gvar_init_len=%d\r\n", los_heap->gvar_init_len);
	printf("global_len=%d\r\n", los_heap->global_len);
	printf("code_len=%d\r\n", los_heap->code_len);
	lp->los_irq = (los_heap->code_main & 0x80000000) > 0 ? 1 : 0;
	los_heap->code_main &= 0x7fffffff;
	if (0 != strcmp((char *)los_heap->los, "Los"))
		return 0xff;
	if (los_api_vs < los_heap->version)
		return 0xfe;

	lp->code_len = los_heap->code_len;
	lp->data_len = los_heap->txt_len;
	lp->bss_len = los_heap->txt_len + los_heap->gvar_init_len;

	lp->lvar_start = los_heap->txt_len + los_heap->gvar_init_len + los_heap->global_len;
	lp->stack_end = lp->lvar_start;
	temp = lp->lvar_start + los_heap->stack_len;
	temp = temp + los_heap->heap_len;
	lp->reg[HEAP_REG].u32 = temp;
	if (inter)
	{
		lp->ram = (uint8_t *)lpram_malloc(temp);
		if (0 == lp->ram)
			return 3;
		memset(lp->ram, 0, temp);
		memcpy(lp->ram, (uint8_t *)&data[9] + los_heap->code_len, los_heap->txt_len + los_heap->gvar_init_len);
		lp->code = (uint8_t *)&data[9];
		gram = lp->ram - los_heap->code_len;
		lp->pc = lp->code + los_heap->code_main;

		lp->reg[HEAP_REG].u32 += los_heap->code_len;
		lp->stack_end += los_heap->code_len;
	}
	else
	{
		lp->ram = (uint8_t *)lpram_malloc(temp + los_heap->code_len);
		if (0 == lp->ram)
			return 3;
		memset(lp->ram, 0, temp + los_heap->code_len);
		memcpy(lp->ram, &data[9], los_heap->txt_len + los_heap->gvar_init_len + los_heap->code_len);
		lp->code = lp->ram;
		gram = lp->ram;
		lp->pc = lp->code + los_heap->code_main;

		lp->reg[HEAP_REG].u32 += los_heap->code_len;
		lp->stack_end += los_heap->code_len;
		lpram_free(data);
	}

	return 0;
}
int los_run(void)
{
	int res;
	for (;;)
	{
		arg = *(lp->pc + 1);
		res = *lp->pc;
#ifdef LOS_DEBUG
		if (res > ARRLEN(los_cmd_api))
		{
			printf("err op[%d]\r\n", res);
			return 4;
		}
#endif
		lp->pc += 2;
		los_cmd_api[res]();
	}
	return res;
}
uint32_t los_app_first(uint32_t *addr, uint8_t inter)
{
	int res;
	if (addr == 0)
		return 1;
	lp = (los_t *)lpram_malloc(sizeof(los_t));
	if (0 == lp)
		return 22;
	memset(lp, 0, sizeof(los_t));
	res = los_init_code(lp, addr, inter);
	if (res)
		return res;
	res = setjmp(lp->jbuf);
	if (res)
	{
		lpram_free(lp->ram);
		lpram_free(lp);
		return res;
	}
	res = los_run();
	return res;
}