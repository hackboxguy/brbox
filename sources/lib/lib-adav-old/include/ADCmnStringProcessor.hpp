#ifndef __AD_CMN_STRING_PROCESSOR_H_
#define __AD_CMN_STRING_PROCESSOR_H_
#include "ADCommon.hpp"
//#include <json.h>
#include "JsonCmnDef.h"
//#include "DispPanelCmnDef.h"
//this is a common string processing library function used by all services and clients
class ADCmnStringProcessor
{
public:
	ADCmnStringProcessor();
	~ADCmnStringProcessor();
	int string_to_enum(const char** string_table,char* string,int max_enum_value);
	int find_single_param(char *json_string,char* param_name,char* value);
	int convert_server_result_to_string(RPC_SRV_RESULT result,char* result_string);//result_enum to string
	RPC_SRV_RESULT convert_string_to_server_result(char* result_string);//string to result_enum
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq);
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* result_para_name,char* result_para_value);
	
	//int prepare_result_string(EDISP_PANEL_RESULT result,JsonDataCommObj* pReq);
	//int prepare_result_string(EDISP_PANEL_RESULT result,JsonDataCommObj* pReq,char* result_para_name,char* result_para_value);

	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,int   val1,char* name2,int val2,char* name3,int val3,char* name4,int val4,
											char* name5,char* val5);//,char* name6, char* val6);
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,int val2,char* name3,int val3,char* name4,int val4);
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,char* val2,char* name3,char* val3);
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,int val1,char* name2,int val2,char* name3,char* val3,char* name4,char* val4);
    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,char* val2,
						char* name3,int val3,char* name4,int val4,
						char* name5,int val5,char* name6,int val6,
						char* name7,int val7,char* name8,int val8,
						char* name9,int val9,char* name10,int val0);
						
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,int val1,char* name2,int val2,
						char* name3,int val3,char* name4,int val4,
						char* name5,int val5,char* name6,int val6,
						char* name7,int val7);						

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,int val1,char* name2,int val2,
                        char* name3,int val3,char* name4,int val4,
                        char* name5,int val5,char* name6,int val6);
    
    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,char* val2,
                        char* name3,char* val3,char* name4,char* val4,
                        char* name5,char* val5);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                                char* name1,double val1,
                                char* name2,double val2,
                                char* name3,double val3,
                                char* name4,double val4,
                                char* name5,double val5,
                                char* name6,double val6,
                                char* name7,double val7,
                                char* name8,double val8,
                                char* name9,double val9);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                                char* name1,double val1,
                                char* name2,double val2,
                                char* name3,double val3);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                                char* name1,char* val1,
                                char* name2,char* val2,
                                char* name3,char* val3,
                                char* name4,double val4,
                                char* name5,double val5,
                                char* name6,double val6,
                                char* name7,double val7,
                                char* name8,double val8,
                                char* name9,double val9,
                              char* name10,double val10,
                              char* name11,double val11,
                              char* name12,double val12);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                                char* name1,int val1,
                                char* name2,double val2,
                                char* name3,char* val3,
                                                    char* name4,int val4,
                                                    char* name5,double val5,
                                                    char* name6,double val6);

	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                                char* name1,int val1,
                                char* name2,int val2,
                                char* name3,int val3);
                                
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
    						char* name1,int val1,
    						char* name2,int val2,
    						char* name3,float val3,
    						char* name4,float val4);
	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                            char* name1,int val1,
                            char* name2,int val2);

	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,unsigned long val1,
                        char* name2,unsigned long val2,
                        char* name3,unsigned long val3);

	int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
	                        char* name1,char* val1,
	                        char* name2,char* val2);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                            char* name1,int val1,
                            char* name2,int val2,
                            char* name3,int val3,
                            char* name4,int val4,
                            char* name5,char* val5,
                            char* name6,char* val6);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                            char* name1,char* val1,
                            char* name2,int val2);

    int prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,char* val2,
                        char* name3,char* val3,char* name4,char* val4);

    int prepare_req_object(JsonDataCommObj* pReq,RPC_SRV_ACT action,int cmd);
};
#endif
