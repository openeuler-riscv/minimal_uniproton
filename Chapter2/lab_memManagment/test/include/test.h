#ifndef __TEST_H
#define __TEST_H



extern int test_echo(int* err, int* success);
extern int test_memory_lab_api(int* err, int *success);
extern int test_malloc_free_api(int* err, int *success);

typedef int (*test_func)(int* err, int* success);

struct testcase {
	const char* name;
	test_func   func_test;
};

static struct testcase tests[] = {
	{ "echo_test", test_echo 		},
	{ "mapi_test", test_memory_lab_api 	},
	{ "malc_test", test_malloc_free_api	},
};


#endif
