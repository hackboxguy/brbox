#ifndef __GPIOCTL_JSON_DEF_H_
#define __GPIOCTL_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define GPIOCTL_JSON_PORT_NUMBER    ADCMN_PORT_GPIOCTL
/* ------------------------------------------------------------------------- */
typedef enum EJSON_GPIOCTL_RPC_TYPES_T
{
	EJSON_GPIOCTL_RPC_IO_CONFIG=0,
	EJSON_GPIOCTL_RPC_IO_GET,
	EJSON_GPIOCTL_RPC_IO_SET,
	EJSON_GPIOCTL_RPC_END,
	EJSON_GPIOCTL_RPC_NONE
}EJSON_GPIOCTL_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_GPIOCTL_RPC_IO_CONFIG
#define GPIOCTL_RPC_IO_CONFIG                  "gpio_config"
//#define GPIOCTL_RPC_ID_PATTERN_CHECK_ARG       "pattern"
//#define GPIOCTL_RPC_ID_PATTERN_CHECK_ARG_TABL  {"ident","red","green","blue","white","unknown","none","\0"}
typedef enum EJSON_GPIOCTL_IFACE_TYPE_T
{
	EJSON_GPIOCTL_IDPATTERN_IDENT,
	EJSON_GPIOCTL_IDPATTERN_RED,
	EJSON_GPIOCTL_IDPATTERN_GREEN,
	EJSON_GPIOCTL_IDPATTERN_BLUE,
	EJSON_GPIOCTL_IDPATTERN_WHITE,
	EJSON_GPIOCTL_IDPATTERN_UNKNOWN,
	EJSON_GPIOCTL_IDPATTERN_NONE,
}EJSON_GPIOCTL_IFACE_TYPE;
typedef struct GPIOCTL_ID_PATTERN_PACKET_T
{
	EJSON_GPIOCTL_IFACE_TYPE pattern_type;
}GPIOCTL_ID_PATTERN_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_GPIOCTL_RPC_DEBUG_OUTFILE_GET,
//EJSON_GPIOCTL_RPC_DEBUG_OUTFILE_SET,
#define GPIOCTL_RPC_IO_GET       "gpio_get"
#define GPIOCTL_RPC_IO_SET       "gpio_set"
#define GPIOCTL_RPC_IO_ADDR_ARG  "addr"
#define GPIOCTL_RPC_IO_VALUE_ARG "value"
typedef struct GPIOCTL_DEBUG_OUTFILE_PACKET_T
{
	char filepath[512];
}GPIOCTL_DEBUG_OUTFILE_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to gpioctl-service here
typedef struct GPIOCTL_CMN_DATA_CACHE_T
{
	std::string StrImgIdDebugFile;//pChar //Char //Int //pInt //Float //Enum
}GPIOCTL_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

