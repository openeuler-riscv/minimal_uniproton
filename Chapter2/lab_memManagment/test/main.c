#include <stdio.h>
#include <stdlib.h>
#include <test.h>
#include <memory_lab.h>
#include <time.h>
#define FMT_GREEN	"\e[92m"
#define FMT_RED		"\e[91m"
#define FMT_BLUE	"\e[96m"
#define FMT_DEFAULT 	"\e[39m"
#define FMT_YELLOW	"\e[93m"

#define MEM_POOL_SIZE	0x20000000
static char memory_pool[MEM_POOL_SIZE];


int main(int argc, char* argv[])
{
	srand((uintptr_t)time(NULL));
	memory_init((uint64_t)memory_pool, MEM_POOL_SIZE);
	int success = 0;
	int err = 0;
	printf(FMT_YELLOW "==================test start=================\n" FMT_DEFAULT);
	int size = sizeof(tests)/sizeof(tests[0]);
	for(int i=0;i<size;i++)
	{
		int tmp_success = 0;
		int tmp_err = 0;
		tests[i].func_test(&tmp_err, &tmp_success);
		if(tmp_err ==0)
			printf(FMT_BLUE "testcase for %s : " FMT_GREEN " %d/%d \n" FMT_DEFAULT, tests[i].name, tmp_success, (tmp_success + tmp_err));	
		else
			printf(FMT_BLUE "testcase for %s : " FMT_RED " %d/%d \n" FMT_DEFAULT, tests[i].name, tmp_success, (tmp_success + tmp_err));	

		success += tmp_success;
		err += tmp_err;
	}

	if(err ==0)
		printf(FMT_BLUE "testcase for all       : " FMT_GREEN " %d/%d \n" FMT_DEFAULT, success, (success + err));	
	else
		printf(FMT_BLUE "testcase for all       : " FMT_RED " %d/%d \n" FMT_DEFAULT, success, (success + err));	

	printf(FMT_YELLOW "==================test   end=================\n" FMT_DEFAULT);
	memory_exit();
	return 0;
}
