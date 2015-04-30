#ifndef __SMARTEYE_JSON_DEF_H_
#define __SMARTEYE_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#define SMARTEYE_JSON_PORT_NUMBER 40001
/* ------------------------------------------------------------------------- */
typedef enum EJSON_SMARTEYE_RPC_TYPES_T
{
	EJSON_SMARTEYE_RPC_GET_SCREEN_PATTERN=0,
	EJSON_SMARTEYE_RPC_END,
	EJSON_SMARTEYE_RPC_NONE
}EJSON_SMARTEYE_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_GET_SCREEN_PATTERN
#define SMARTEYE_RPC_SCREEN_PATTERN_GET     "get_mac_addr"
/* ------------------------------------------------------------------------- */

#endif

