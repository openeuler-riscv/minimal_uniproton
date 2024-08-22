TOP_DIR:= $(patsubst %/,%,$(dir $(abspath ${MAKEFILE_LIST})))
CUR_DIR:= $(patsubst %/,%,$(shell pwd))
BLD_DIR:= $(TOP_DIR)/build

ALOCTOR:=UNIPROTON_ALLOCATOR

CROSS_COMPILE			:=
CC				:= $(CROSS_COMPILE)gcc
CFLAGS				:=	

LD				:= $(CROSS_COMPILE)ld
LDFLAGS				:= 
PHONY				:=

test: lib_compile test_compile
	@LD_PRELOAD=$(LIB_BLD_DIR)/libhack.so  $(TST_BLD_DIR)/main
PHONY+= test 

LIB_SRC_DIR:= $(TOP_DIR)/lib
LIB_BLD_DIR:= $(BLD_DIR)/lib
LIB_SRC_FLE:= $(shell find $(LIB_SRC_DIR) -name "*.c")
LIB_OBJ_FLE:= $(patsubst $(LIB_SRC_DIR)/%.c,$(LIB_BLD_DIR)/%.o,$(LIB_SRC_FLE))
LIB_C_FLAGS:= -fPIC
CFLAGS += -I$(LIB_SRC_DIR)/include -DALLOCATOR=$(ALOCTOR)


lib_compile:$(LIB_BLD_DIR)/libhack.so

PHONY+=lib_compile

$(LIB_BLD_DIR)/libhack.so:$(LIB_OBJ_FLE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIB_C_FLAGS) -shared -fPIC $^ -o $@

$(LIB_BLD_DIR)/%.o:$(LIB_SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIB_C_FLAGS) -c $< -o $@


TST_SRC_DIR:= $(TOP_DIR)/test
TST_BLD_DIR:= $(BLD_DIR)/test

TST_SRCFILE:=$(shell find $(TST_SRC_DIR) -name "*.c")
TST_BLDOBJT:=$(patsubst $(TST_SRC_DIR)/%.c,$(TST_BLD_DIR)/%.o,$(TST_SRCFILE))
CFLAGS += -I$(TST_SRC_DIR)/include


test_compile:$(TST_BLD_DIR)/main
PHONY+=test_compile

$(TST_BLD_DIR)/main: $(LIB_BLD_DIR)/libhack.so $(TST_BLDOBJT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -L$(LIB_BLK_DIR) -lhack $^ -o $@

$(TST_BLD_DIR)/%.o:$(TST_SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@



BENCH_SRC_DIR:= $(TOP_DIR)/benchmark
BENCH_BLD_DIR:= $(BLD_DIR)/benchmark
BENCH_SRC_FLE:= $(shell find $(BENCH_SRC_DIR) -name "*.c")
BENCH_OBJ_FLE:= $(patsubst $(BENCH_SRC_DIR)/%.c,$(BENCH_BLD_DIR)/%.o,$(BENCH_SRC_FLE))
BENCH_C_FLAGS:= -g -Wall -O2 -std=gnu99

bench_compile:$(BENCH_BLD_DIR)/benchmark_malloc_self
	
PHONY+=bench_compile

$(BENCH_BLD_DIR)/benchmark_malloc_self: $(LIB_BLD_DIR)/libhack.so $(BENCH_OBJ_FLE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -lpthread -L$(LIB_BLK_DIR) -lhack $^ -o $@

$(BENCH_BLD_DIR)/%.o:$(BENCH_SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BENCH_C_FLAGS) -c $< -o $@

benchmark: bench_compile
	@echo -e "\n\e[93m=======================run glibc malloc benchmark start ==============================\e[96m"
	@$(BENCH_SRC_DIR)/benchmark_malloc_glibc 1
	@echo -e "\n\e[93m=======================run glibc malloc benchmark end   ==============================\e[96m"
	@echo -e "\n\e[93m=======================run self  malloc benchmark start ==============================\e[96m"
	@LD_PRELOAD=$(LIB_BLD_DIR)/libhack.so  $(BENCH_BLD_DIR)/benchmark_malloc_self 1
	@echo -e "\n\e[93m=======================run self  malloc benchmark end   ==============================\e[39m"

PHONY+=benchmark

debug:
	@echo ${PHONY}
	@echo ${BENCH_OBJ_FLE}
clean:
	rm -rf $(BLD_DIR)

PHONY += clean

.PHONY:${PHONY}
