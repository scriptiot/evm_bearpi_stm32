#include "epolling_module.h"
#include "./uart/usart.h"
#include <time.h>
#include "evm_main.h"

extern int ecma_module(evm_t * e, uint32_t number_of_timers);

//实现time头文件

time_t time_dat;
time_t time (time_t *_timer)
{
    *_timer = time_dat;
	return time_dat;
}
//设置时间（可以通过RTC获取时间）
void set_time(){
	struct tm set_time;
	set_time.tm_sec = 10;
	set_time.tm_min = 0;
	set_time.tm_hour = 8;
	set_time.tm_mday = 13;
	set_time.tm_mon = 10-1;
	set_time.tm_year = 2020-1900;
	set_time.tm_isdst = -1;
	time_dat = mktime(&set_time);
}

//实现虚拟机模块加载
const char * vm_load(evm_t * e, char * path, int type)
{
    return NULL;
}

//实现虚拟机内存分配malloc接口
void * vm_malloc(int size)
{
    void * m = malloc(size);
    if(m) memset(m, 0 ,size);
    return m;
}
//实现虚拟机内存分配free接口
void vm_free(void * mem)
{
    if(mem) free(mem);
}

char evm_repl_tty_read(evm_t * e){
	char ch;
	if(HAL_UART_Receive(&huart1,(uint8_t *)&ch,1,4)==HAL_OK)
		return ch;
	else{
		epolling_poll(e);
	}
	return 0;
}

#define VM_HEAP_SIZE	(10 * 1024)
#define VM_STACK_SIZE	(5 * 1024)
#define VM_MODULE_SIZE	5

evm_t * my_env = NULL;

int evm_main(void){
	evm_register_free((intptr_t)vm_free);
    evm_register_malloc((intptr_t)vm_malloc);
    evm_register_print((intptr_t)printf);
    evm_register_file_load((intptr_t)vm_load);
	//创建虚拟机
	my_env = (evm_t*)malloc(sizeof(evm_t));
    memset(my_env, 0, sizeof(evm_t));
	//虚拟机初始化
    int err = evm_init(my_env, VM_HEAP_SIZE, VM_STACK_SIZE, VM_MODULE_SIZE, EVM_VAR_NAME_MAX_LEN, EVM_FILE_NAME_LEN);
	if( err != ec_ok ) {evm_errcode_print(my_env);return err;}
    //加载ecma模块
	ecma_module(my_env, 5);
    if(my_env->err) {evm_errcode_print(my_env);return err;}
	
	err = epolling_module(my_env, 10, 10);
	if(err) {evm_errcode_print(my_env);return err;}
	
	err = evm_module(my_env);
	if(err) {evm_errcode_print(my_env);return err;}
    //加载main.qml文件
	err = evm_boot(my_env, "main.js");
	if(err) {
		evm_repl_run(my_env, 10, EVM_LANG_JS);
	} else {
		//虚拟机启动
		err = evm_start(my_env);
	}
	return err;
}

