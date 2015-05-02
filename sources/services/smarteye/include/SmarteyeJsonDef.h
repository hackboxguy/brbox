#ifndef __SMARTEYE_JSON_DEF_H_
#define __SMARTEYE_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define SMARTEYE_JSON_PORT_NUMBER    ADCMN_PORT_SMARTEYE
/* ------------------------------------------------------------------------- */
typedef enum EJSON_SMARTEYE_RPC_TYPES_T
{
	EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN=0,
	EJSON_SMARTEYE_RPC_END,
	EJSON_SMARTEYE_RPC_NONE
}EJSON_SMARTEYE_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN
#define SMARTEYE_RPC_ID_PATTERN_CHECK           "check_id_pattern" //checks if id pattern exists
#define SMARTEYE_RPC_ID_PATTERN_CHECK_ARG       "pattern"
#define SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL  {"ident","red","green","blue","white","unknown","none","\0"}
typedef enum EJSON_SMARTEYE_IFACE_TYPE_T
{
	EJSON_SMARTEYE_IDPATTERN_IDENT,
	EJSON_SMARTEYE_IDPATTERN_RED,
	EJSON_SMARTEYE_IDPATTERN_GREEN,
	EJSON_SMARTEYE_IDPATTERN_BLUE,
	EJSON_SMARTEYE_IDPATTERN_WHITE,
	EJSON_SMARTEYE_IDPATTERN_UNKNOWN,
	EJSON_SMARTEYE_IDPATTERN_NONE,
}EJSON_SYSMGR_IFACE_TYPE;
typedef struct SYSMGR_ID_PATTERN_PACKET_T
{
	EJSON_SYSMGR_IFACE_TYPE pattern_type;
}SYSMGR_ID_PATTERN_PACKET;
/* ------------------------------------------------------------------------- */

#endif

