#ifndef __AD_SYSINFO_H_
#define __AD_SYSINFO_H_

//#include <sys/ioctl.h>
//#include <netinet/in.h>
//#include <net/if.h>
#include "ADCommon.hpp"
#define MAX_IFS 64
#define MAX_ALLOWED_INTERFACES 8

typedef struct eth_name_ip_str_t
{
	char dev[512];
	char ip[512];
	char mac[512];
	char netmask[512];
}eth_name_ip_str;

class ADSysInfo
{
	int count_string_occurance(char const *str,char* match_string);
	int get_cpu_cores(void);
	int get_current_and_average_load(float *current,float *average);
	int get_mem_and_free_mem(int *mem_kb,int *mem_free_kb);
	int get_uptime(int* uptime);
	int get_cpu_model_name(char* model);
	int get_cpu_frequency(char* freq);
public:
	ADSysInfo();
	~ADSysInfo();
	int read_system_status(int *mem,int* memfree,int *cores,int* cur_load,int* avg_load,int* uptime,char* cpu_model);
	int read_network_info(char* eth,char *mac,char* ip,char* netmask);
	int read_network_info_new(char* eth,char *mac,char* ip,char* netmask);
	int read_network_info_ifconfig(char* eth,char* mac,char* ip,char* netmask);
	int probe_eth_interface_details(int *total_detected, eth_name_ip_str *list);
	int read_mem_info(char* mem,char *memfree,char* memused);
	int read_load_info(char* curload,char* avgload,char* uptime);
	int read_cpu_info(char* cpumodel,char *cores,char* cpufreq);
	RPC_SRV_RESULT get_total_eth_count(int &count);
	RPC_SRV_RESULT get_nth_eth_name(int index,char *name);
	RPC_SRV_RESULT run_shell_script(char* script,bool emulation);
	RPC_SRV_RESULT run_shell_script(char* cmd,char*ret_val,bool emulation);


};
#endif
