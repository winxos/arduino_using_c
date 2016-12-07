/*
 * public.c
 *
 * Created: 2016-12-7 10:33:17
 *  Author: winxos
 */ 
#include "public.h"

int split(char dst[][80], char *str, const char *spl)//for single line chars split to words
{
	int n = 0;
	char *result = NULL;
	result = strtok(str, spl);
	while (result != NULL)
	{
		strcpy(dst[n++], result);
		result = strtok(NULL, spl);
	}
	return n;
}