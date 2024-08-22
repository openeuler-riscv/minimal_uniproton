#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include <test.h>

int test_malloc_free_api(int* err, int *success)
{
	//test for malloc 0
	void* ptr = malloc(0);
	if(ptr != NULL)
	{
		(*success)++;
		free(ptr);
	}
	else
		(*err)++;
	
	//test for a huge(unreach able) malloc
	ptr = malloc((size_t)-1);
	if(ptr == NULL)
		(*success)++;
	else
	{
		(*err)++;
		free(ptr);
	}

	//test for a null free
	free(NULL);
	//如果没有发生segment fault , 就成功
	(*success)++;

	//test for normal case
	for(int i=0;i<1000;i++)
	{
		//test for  1000 - 10000 memory get
		int size = (rand() + 1000) % 10000;
	        void* ptr = malloc(size);
		if(ptr == NULL)
			(*err)++;
		else
		{
			(*success)++;
			free(ptr);
		}
	}
	
	//test for huge alloc(reachable)
	for(int i=0;i<1000;i++)
	{
		//test for  0x10000 - 0xa0000 memory get
		int size = (rand() + 0x10000) % 0xa0000;
	        void* ptr = malloc(size);
		if(ptr == NULL)
			(*err)++;
		else
		{
			(*success)++;
			free(ptr);
		}
	}
	
	return 0;
}

