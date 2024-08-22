# 内存管理实验

## 说明

- 此目录为内存管理部分实验的 **测试 + 跑分** 使用的代码

## 环境要求

- OS : Linux
- 软件 ： gnu binary utils gnu make

## 使用方法

### 内置内存分配算法

- First Fit
- Best FIt
- Worst Fit
- Memory Pool
- Buddy Allocator
- easy small memory allocator (like slab)

### 测试方法

### testcase

- 在此目录下执行如下命令

  ```shell
  make test ALOCTOR=UNIPROTON_ALLOCATOR
  ```

- 其中 ， **ALOCTOR=UNIPROTON_ALLOCATOR** 是在指定内存分配的算法，此处指定为 buddy_allocator ( UniProton 中的实现)

### benchmark

- 在此目录下执行如下命令

  ```shell
  make benchmark ALOCTOR=UNIPROTON_ALLOCATOR
  ```

- 其中 ， **ALOCTOR=UNIPROTON_ALLOCATOR** 是在指定内存分配的算法，此处指定为 buddy_allocator ( UniProton 中的实现)


## clean

- 在此目录下执行下面命令用来清除产出

  ```shell
  make clean
  ```

### 支持的算法的名字

- 对应的几个算法的名字如下
  - First Fit  ---> BASIC_ALOG_FIRST_FIT
  - Best FIt ---> BASIC_ALOG_BEST_FIT
  - Worst FIt ---> BASIC_ALOG_WORST_FIT
  - Memory Pool ---> MEMORY_POOL
  - buddy allocator ---> UNIPROTON_ALLOCATOR
  - easy small memory allocator ---> SLAB_ALLOCATOR

## testcase 结果

```shell
// First Fit
==================test start=================
testcase for echo_test :  10/10 
testcase for mapi_test :  324/2003 
testcase for malc_test :  319/2003 
testcase for all       :  653/4016 
==================test   end=================
// Best Fit
==================test start=================
testcase for echo_test :  10/10 
testcase for mapi_test :  332/2003 
testcase for malc_test :  318/2003 
testcase for all       :  660/4016 
==================test   end=================
// Worst Fit
==================test start=================
testcase for echo_test :  10/10 
testcase for mapi_test :  292/2003 
testcase for malc_test :  295/2003 
testcase for all       :  597/4016 
==================test   end=================
// Memory Pool
==================test start=================
testcase for echo_test :  10/10 
testcase for mapi_test :  2/2003 
testcase for malc_test :  2/2003 
testcase for all       :  14/4016 
==================test   end=================
// Buddy Allocator (UniProton Memory Allocator)
==================test start=================
testcase for echo_test :  10/10 
testcase for mapi_test :  2003/2003 
testcase for malc_test :  2003/2003 
testcase for all       :  4016/4016 
==================test   end=================
// easy small memory allocator
==================test start=================
testcase for echo_test :  10/10 
testcase for mapi_test :  2/2003 
testcase for malc_test :  2/2003 
testcase for all       :  14/4016 
==================test   end=================
```



## benchmark 结果

```shell
==============================================================================
// glibc malloc
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 5.19652e+09,
    "time_per_iteration": 36.8788,
    "max_rss": 4032,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

==============================================================================
// First Fit
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 4.04097e+08,
    "time_per_iteration": 474.245,
    "max_rss": 4032,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

==============================================================================
// Best Fit
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 3.87924e+08,
    "time_per_iteration": 494.018,
    "max_rss": 4168,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

==============================================================================
// Worst Fit
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 3.8249e+08,
    "time_per_iteration": 501.035,
    "max_rss": 3940,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

==============================================================================
// Memory Pool
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 6.09307e+09,
    "time_per_iteration": 31.4523,
    "max_rss": 4168,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

==============================================================================
// Buddy Allocator (UniProton Memory Allocator)
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 4.72206e+08,
    "time_per_iteration": 405.842,
    "max_rss": 525260,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

==============================================================================
// easy small memory allocator
{
 "timing_type": "hp_timing",
 "functions": {
  "malloc": {
   "": {
    "duration": 1.91641e+11,
    "iterations": 6.26134e+09,
    "time_per_iteration": 30.607,
    "max_rss": 3832,
    "threads": 1,
    "min_size": 4,
    "max_size": 32768,
    "random_seed": 88
   }
  }
 }
}
==============================================================================

```





## 如何添加自己的一个 malloc , free 的实现并接入测试和跑分

1.  找到文件 **lib/include/memory_lab.h** 文件，按照格式添加自己的内存分配的算法的宏，如 #define SELF_ALLOCATOR  10
2. 在  **lib/include/memory_lab.h** 文件中 根据 VOID_ALLOCATOR 的内容，实现四个要求函数，其他的分配算法采取的是把 port user api 在/lib/include 目录下新建 .h 文件来存放，在  **lib/include/memory_lab.h** 文件中包含对应头文件，实现对应要求的四个函数
3. 在  **lib/src/** 下创建新的目录用来存放自己的源码，然后实现对应的函数即可



## 作者

项目框架 ： 罗君 Jer6y@github

项目测试用例  ：罗君 Jer6y@github

项目跑分用例

- 实现 : [项目 malloc-benchmark](https://github.com/f18m/malloc-benchmarks)
- 接入 : 罗君 Jer6y@github

算法

- UniProton Buddy Allocator 
  - 实现 : [gitee openEuler社区](https://gitee.com/openeuler/UniProton)
  - 适配接入 : 罗君 Jer6y@github 
- First Fit
  - 实现 ： 陶俊
  - 适配介入 : 罗君 Jer6y@github
- Best Fit 
  - 实现 ： 陶俊
  - 适配介入 : 罗君 Jer6y@github
- Worst Fit
  - 实现 ： 陶俊
  - 适配介入 : 罗君 Jer6y@github
- Memory Pool
  - 实现 ： 陶俊
  - 适配介入 : 罗君 Jer6y@github
- easy small memory allocator
  - 实现 ： 陶俊
  - 适配介入 : 罗君 Jer6y@github