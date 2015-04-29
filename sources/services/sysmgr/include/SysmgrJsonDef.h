#ifndef __SYSMGR_JSON_DEF_H_
#define __SYSMGR_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#define SYSMGR_JSON_PORT_NUMBER 40000
/* ------------------------------------------------------------------------- */
typedef enum EJSON_SYSMGR_RPC_TYPES_T
{
	EJSON_SYSMGR_RPC_GET_MAC_ADDR=0,
	EJSON_SYSMGR_RPC_SET_MAC_ADDR,	     
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
typedef struct SYSMGR_MAC_ADDR_PACKET_T
{
	EJSON_SYSMGR_IFACE_TYPE eth_type;
	char mac_addr[512];
}SYSMGR_MAC_ADDR_PACKET;
/* ------------------------------------------------------------------------- */

//rpc-1
#define SYSMGR_RPC_FMW_VER_GET         "get_fmw_ver"
//#define SYSMGR_RPC_FMW_VER_SET         "set_fmw_ver"
#define SYSMGR_RPC_FMW_VER_ARGVER      "version"
typedef struct SYSMGR_FMW_VER_PACKET_T
{
	char fmw_ver[512];
}SYSMGR_FMW_VER_PACKET;
/* ------------------------------------------------------------------------- */
//rpc-2
//{ "jsonrpc": "2.0", "method": "get_ip_addr", "id": 43 }
#define SYSMGR_RPC_IP_ADDR_GET         "get_ip_addr"
#define SYSMGR_RPC_IP_ADDR_SET         "set_ip_addr"
#define SYSMGR_RPC_IP_ADDR_ARGVER      "iface"
typedef struct SYSMGR_IP_ADDR_PACKET_T
{
	char ip_addr[512];
}SYSMGR_IP_ADDR_PACKET;
/* ------------------------------------------------------------------------- */

#endif

