#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include <test.h>

int tfunc(void)
{
	return 666;
}

int test_echo(int* err, int *success)
{
	for(int i=0;i<10;i++)
	{
		int ret = tfunc();
		if(ret != 666)
			(*err)++;
		else
			(*success)++;
	}
	return 0;
}

