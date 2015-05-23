#ifndef __SYSMGR_JSON_DEF_H_
#define __SYSMGR_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define SYSMGR_JSON_PORT_NUMBER    ADCMN_PORT_SYSMGR
/* ------------------------------------------------------------------------- */
typedef enum EJSON_SYSMGR_RPC_TYPES_T
{
	EJSON_SYSMGR_RPC_GET_MAC_ADDR=0,
	EJSON_SYSMGR_RPC_SET_MAC_ADDR,
	EJSON_SYSMGR_RPC_GET_ETH_COUNT, //returns number of eth interfaces available on the device
	EJSON_SYSMGR_RPC_GET_ETH_NAME,
	
	EJSON_SYSMGR_RPC_GET_LOADINFO,
	EJSON_SYSMGR_RPC_GET_MEMINFO,
	EJSON_SYSMGR_RPC_GET_CPUINFO,

	EJSON_SYSMGR_RPC_END,
	EJSON_SYSMGR_RPC_NONE
}EJSON_SYSMGR_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_MAC_ADDR
//EJSON_SYSMGR_RPC_SET_MAC_ADDR
#define SYSMGR_RPC_MAC_ADDR_GET             "get_mac_addr"
#define SYSMGR_RPC_MAC_ADDR_SET             "set_mac_addr"
#define SYSMGR_RPC_MAC_ADDR_ARG             "addr"
#define SYSMGR_RPC_MAC_ADDR_ARG_IFACE       "iface"
#define SYSMGR_RPC_MAC_ADDR_ARG_IFACE_TABL  {"eth0","eth1","wlan0","unknown","none","\0"}
typedef enum EJSON_SYSMGR_IFACE_TYPE_T
{
	EJSON_SYSMGR_IFACE_TYPE_ETH0,
	EJSON_SYSMGR_IFACE_TYPE_ETH1,
	EJSON_SYSMGR_IFACE_TYPE_WLAN0,
	EJSON_SYSMGR_IFACE_TYPE_UNKNOWN,
	EJSON_SYSMGR_IFACE_TYPE_NONE,
}EJSON_SYSMGR_IFACE_TYPE;
typedef struct SYSMGR_MAC_ADDR_PACKET_T //common communication object for different mac related rpc's
{
	EJSON_SYSMGR_IFACE_TYPE eth_type;
	char mac_addr[512];
	int EthCountIndx;//can be used as eth-index or eth-count
	char eth_name[512];
}SYSMGR_MAC_ADDR_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_ETH_COUNT
#define SYSMGR_RPC_ETH_COUNT_GET             "get_eth_count"
#define SYSMGR_RPC_ETH_COUNT_ARG             "count"
typedef SYSMGR_MAC_ADDR_PACKET SYSMGR_ETH_COUNT_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_ETH_NAME
#define SYSMGR_RPC_ETH_NAME_GET             "get_eth_name"
#define SYSMGR_RPC_ETH_NAME_ARG_INDEX       "index"
#define SYSMGR_RPC_ETH_NAME_ARG             "name"
typedef SYSMGR_MAC_ADDR_PACKET SYSMGR_ETH_NAME_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_LOADINFO
#define SYSMGR_RPC_LOADINFO_GET             "get_load_info"
#define SYSMGR_RPC_LOADINFO_ARG_CURRENT     "current"
#define SYSMGR_RPC_LOADINFO_ARG_AVERAGE     "average"
#define SYSMGR_RPC_LOADINFO_ARG_UPTIME      "uptime"
typedef struct SYSMGR_LOAD_INFO_PACKET_T
{
	char cur_load[512];
	char avg_load[512];
	char uptime[512];
}SYSMGR_LOAD_INFO_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_MEMINFO
#define SYSMGR_RPC_MEMINFO_GET             "get_mem_info"
#define SYSMGR_RPC_MEMINFO_ARG_MEM         "memMB"
#define SYSMGR_RPC_MEMINFO_ARG_MEMFREE     "memfreeMB"
#define SYSMGR_RPC_MEMINFO_ARG_MEMUSED     "memusedMB"
typedef struct SYSMGR_MEM_INFO_PACKET_T
{
	char mem[512];
	char memfree[512];
	char memused[512];
}SYSMGR_MEM_INFO_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SYSMGR_RPC_GET_CPUINFO
#define SYSMGR_RPC_CPUINFO_GET             "get_cpu_info"
#define SYSMGR_RPC_CPUINFO_ARG_MODEL       "model"
#define SYSMGR_RPC_CPUINFO_ARG_CORES       "cores"
#define SYSMGR_RPC_CPUINFO_ARG_FREQ        "frequency"
typedef struct SYSMGR_CPU_INFO_PACKET_T
{
	char cpu_model[512];
	char cpu_cores[512];
	char cpu_freq[512];
}SYSMGR_CPU_INFO_PACKET;
/* ------------------------------------------------------------------------- */
#endif

