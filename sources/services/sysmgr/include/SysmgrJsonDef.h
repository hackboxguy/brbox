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

#endif

