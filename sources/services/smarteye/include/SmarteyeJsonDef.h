#ifndef __SMARTEYE_JSON_DEF_H_
#define __SMARTEYE_JSON_DEF_H_
/* ------------------------------------------------------------------------- */
#include "ADCmnPortList.h"
#define SMARTEYE_JSON_PORT_NUMBER    ADCMN_PORT_SMARTEYE
/* ------------------------------------------------------------------------- */
typedef enum EJSON_SMARTEYE_RPC_TYPES_T
{
	EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN=0,
	EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET,
	EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET,
	EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG,
	EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET,
	EJSON_SMARTEYE_RPC_CHECKWALL_FILE_GET,//on which image EJSON_SMARTEYE_IDPATTERN_CHECK_WALL to be executed?
	EJSON_SMARTEYE_RPC_CHECKWALL_FILE_SET,//on which image EJSON_SMARTEYE_IDPATTERN_CHECK_WALL to be executed?
	EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET, //base reference image for EJSON_SMARTEYE_IDPATTERN_CHECK_WALL
	EJSON_SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET, //base reference image for EJSON_SMARTEYE_IDPATTERN_CHECK_WALL
	EJSON_SMARTEYE_RPC_SCAN_QRSTRING,
	EJSON_SMARTEYE_RPC_END,
	EJSON_SMARTEYE_RPC_NONE
}EJSON_SMARTEYE_RPC_TYPES;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_CHECK_ID_PATTERN
#define SMARTEYE_RPC_ID_PATTERN_CHECK           "check_id_pattern" //checks if id pattern exists
#define SMARTEYE_RPC_ID_PATTERN_CHECK_ARG       "pattern"
#define SMARTEYE_RPC_ID_PATTERN_CHECK_ARG_TABL  {"ident","red","green","blue","white","chkwall","unknown","none","\0"}
typedef enum EJSON_SMARTEYE_IFACE_TYPE_T
{
	EJSON_SMARTEYE_IDPATTERN_IDENT,
	EJSON_SMARTEYE_IDPATTERN_RED,
	EJSON_SMARTEYE_IDPATTERN_GREEN,
	EJSON_SMARTEYE_IDPATTERN_BLUE,
	EJSON_SMARTEYE_IDPATTERN_WHITE,
	EJSON_SMARTEYE_IDPATTERN_CHECK_WALL,//checks if all tiles in wall are ON
	EJSON_SMARTEYE_IDPATTERN_UNKNOWN,
	EJSON_SMARTEYE_IDPATTERN_NONE,
}EJSON_SMARTEYE_IFACE_TYPE;
typedef struct SMARTEYE_ID_PATTERN_PACKET_T
{
	EJSON_SMARTEYE_IFACE_TYPE pattern_type;
}SMARTEYE_ID_PATTERN_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_GET,
//EJSON_SMARTEYE_RPC_DEBUG_OUTFILE_SET,
//EJSON_SMARTEYE_RPC_CAPTURE_JPG_IMG,
#define SMARTEYE_RPC_DEBUG_OUTFILE_GET       "get_debug_outfile"
#define SMARTEYE_RPC_DEBUG_OUTFILE_SET       "set_debug_outfile"
#define SMARTEYE_RPC_DEBUG_OUTFILE_ARG       "filepath"
#define SMARTEYE_RPC_CAPTURE_JPG_IMG         "capture_image"
#define	SMARTEYE_RPC_CHECKWALL_FILE_GET      "get_checkwall_file"
#define SMARTEYE_RPC_CHECKWALL_FILE_SET      "set_checkwall_file"
#define	SMARTEYE_RPC_CHECKWALL_BASE_FILE_GET "get_checkwall_base_file"
#define	SMARTEYE_RPC_CHECKWALL_BASE_FILE_SET "set_checkwall_base_file"
typedef struct SMARTEYE_DEBUG_OUTFILE_PACKET_T
{
	char filepath[512];
}SMARTEYE_DEBUG_OUTFILE_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_CAPTURE_RESOLUTION_SET
#define SMARTEYE_RPC_CAPTURE_RESOLUTION_SET       "set_capture_resolution"
#define SMARTEYE_RPC_CAPTURE_RESOLUTION_ARGH      "pixels"
#define SMARTEYE_RPC_CAPTURE_RESOLUTION_ARGV      "lines"
typedef struct SMARTEYE_CAPTURE_RESOLUTION_PACKET_T
{
	int pixels;
	int lines;
}SMARTEYE_CAPTURE_RESOLUTION_PACKET;
/* ------------------------------------------------------------------------- */
//EJSON_SMARTEYE_RPC_SCAN_QRSTRING
#define SMARTEYE_RPC_SCAN_QRSTRING               "scan_qr_string"
#define SMARTEYE_RPC_SCAN_QRSTRING_ARGFPATH      "filepath"
#define SMARTEYE_RPC_SCAN_QRSTRING_ARGQRSTR      "qrstring"
typedef struct SMARTEYE_SCAN_QRSTRING_PACKET_T
{
	char filepath[1024];
	char qrstring[1024];
}SMARTEYE_SCAN_QRSTRING_PACKET;
/* ------------------------------------------------------------------------- */
//keep all the data related to smart-eye-service here
typedef struct SMARTEYE_CMN_DATA_CACHE_T
{
	void *pDevInfo;//device-info-struct(typecast in rpc handlers)
	std::string StrImgIdDebugFile;//pChar //Char //Int //pInt //Float //Enum
	SMARTEYE_CAPTURE_RESOLUTION_PACKET captureRes;//jpg image capture resolution
	std::string StrImgIdCheckWallFile;//pChar //Char //Int //pInt //Float //Enum
	std::string StrImgIdCheckWallBaseFile;//pChar //Char //Int //pInt //Float //Enum

	//initialize variables here
	SMARTEYE_CMN_DATA_CACHE_T()
	{
		captureRes.pixels=1920;
		captureRes.lines=1080;
		StrImgIdCheckWallFile="";
		StrImgIdCheckWallBaseFile="";
	};
	~ SMARTEYE_CMN_DATA_CACHE_T(){};
}SMARTEYE_CMN_DATA_CACHE;
/* ------------------------------------------------------------------------- */

#endif

