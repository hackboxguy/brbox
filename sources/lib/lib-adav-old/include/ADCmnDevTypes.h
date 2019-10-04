#ifndef __ADCMN_DEVTYPE_H_
#define __ADCMN_DEVTYPE_H_

#define ADCMN_BOARD_TYPE_TABLE  {"RASPI_A","RASPI_APLUS","RASPI_B","RASPI_BPLUS","RASPI_B2","BAYTRAIL","BAYTRAIL_I210","BBB","RASPI_0","RASPI_3","X86_64","X86_32","A5_V11","NEXX_3020","GL_MT300NV2","Unknown","none","\0"}
typedef enum ADCMN_BOARD_TYPE_T
{
	ADCMN_BOARD_TYPE_RASPI_A,
	ADCMN_BOARD_TYPE_RASPI_APLUS,
	ADCMN_BOARD_TYPE_RASPI_B,
	ADCMN_BOARD_TYPE_RASPI_BPLUS,
	ADCMN_BOARD_TYPE_RASPI_B2,
	ADCMN_BOARD_TYPE_BAYTRAIL,
	ADCMN_BOARD_TYPE_BAYTRAIL_I210,
	ADCMN_BOARD_TYPE_BBB,
	ADCMN_BOARD_TYPE_RASPI_0,
	ADCMN_BOARD_TYPE_RASPI_3,
	ADCMN_BOARD_TYPE_X86_64,
	ADCMN_BOARD_TYPE_X86_32,
	ADCMN_BOARD_TYPE_A5_V11,
	ADCMN_BOARD_TYPE_NEXX_3020,
	ADCMN_BOARD_TYPE_GL_MT300NV2,
	ADCMN_BOARD_TYPE_UNKNOWN,
	ADCMN_BOARD_TYPE_NONE
}ADCMN_BOARD_TYPE;
typedef struct ADCMN_DEV_INFO_T
{
	ADCMN_BOARD_TYPE BoardType;
	ADCMN_DEV_INFO_T()
	{
		BoardType=ADCMN_BOARD_TYPE_UNKNOWN;
	};//initialize variables here
	~ ADCMN_DEV_INFO_T(){};
}ADCMN_DEV_INFO;

#endif
