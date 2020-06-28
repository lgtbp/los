/*----------------------------------------------------------------------------/
 /  los -  system module  R0.1
 /-----------------------------------------------------------------------------/
 /
 / Copyright (c) 2014-2017 LP电子,All Rights Reserved.Author's mailbox:lgtbp@126.com.
 /
 / 未经授权，禁止商用。Commercial use is prohibited without authorization.
 /----------------------------------------------------------------------------*/
#include "../los/los.h"
#include<windows.h>

//buf_path当前路径
//len代码空间
uint8_t *read_file_bin(char *buf_path, uint32_t *len)
{
	FILE *pF;
	uint8_t *buf;
	pF = fopen(buf_path, "rb");
	if (NULL == pF)
	{
		fprintf(stderr, "open bin ERR= %s\r\n", buf_path);
		return NULL;
	}
	fseek(pF, 0, SEEK_END);
	len[0] = ftell(pF);
	buf = (uint8_t *)malloc(len[0]);
	rewind(pF);
	fread(buf, 1, len[0], pF);
	fclose(pF);
	return buf;
}
void los_function(int num)
{
	uint8_t buf[256];
	if (num == 1) //printf
	{
		los_sprintf_low(buf, los_get_argp(0), 1);
		printf("%s", buf);
		return;
	}
}
int main(int argc, char **argv)
{
	int res;
	uint32_t len;
	uint8_t *los;
	char path[128];

	GetCurrentDirectory(100, path); 
	len=strlen(path);
	strcpy(&path[len],"\\test.bin");

	los = read_file_bin(path, &len);
	los_set_function(los_function);
	res = los_app_first((uint32_t *)los, 0);
	printf("res:%d\n", res);
	return 0;
}
