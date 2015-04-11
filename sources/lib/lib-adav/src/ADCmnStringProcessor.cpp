#include "ADCmnStringProcessor.h"
//#include <json.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;
/*****************************************************************************/
ADCmnStringProcessor::ADCmnStringProcessor()
{
}
/*****************************************************************************/
ADCmnStringProcessor::~ADCmnStringProcessor()
{
}
/*****************************************************************************/
int ADCmnStringProcessor::string_to_enum(const char** string_table,char* string,int max_enum_value)
{
	int index=0;
	while (index < max_enum_value)
	{
		if(strcmp(string_table[index],string) == 0)
			return index;//outEnum=(E_DISPLAY_POWER_STATE)index;
		index++;
	}
	return max_enum_value;
}
/*****************************************************************************/
int ADCmnStringProcessor::find_single_param(char *json_string,char* param_name,char* value)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
	{
		//printf("failed to allocate mem for new_obj\n");
		return -1;
	}

	struct json_object *new_obj1 = json_object_object_get(new_obj, "params");
	if(json_object_get_string(new_obj1)==NULL)
	{
		//printf("failed to find params\n");
		json_object_put(new_obj);
		return -1;
	}

	struct json_object *new_obj2 = json_object_object_get(new_obj1, param_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		//printf("failed to find params\n");
		json_object_put(new_obj);
		return -1;
	}
	sprintf(value,"%s",json_object_get_string(new_obj2));
	json_object_put(new_obj);
	return 0;
}
/*****************************************************************************/
const char *rpc_server_result_table[]   = STR_RPC_SRV_RESULT_STRING_TABLE;
int ADCmnStringProcessor::convert_server_result_to_string(RPC_SRV_RESULT result,char* result_string)
{
	//const char *rpc_server_result_table[]   = STR_RPC_SRV_RESULT_STRING_TABLE;
	if(result>=RPC_SRV_RESULT_NONE)
		strcpy(result_string,rpc_server_result_table[RPC_SRV_RESULT_UNKNOWN]);
	else
		strcpy(result_string,rpc_server_result_table[result]);
	return 0;
}
RPC_SRV_RESULT ADCmnStringProcessor::convert_string_to_server_result(char* result_string)
{
	for(int i=0;i<RPC_SRV_RESULT_NONE;i++)
	{
		if(strcmp(result_string,rpc_server_result_table[i])==0)
			return (RPC_SRV_RESULT)(i);
	}
	return RPC_SRV_RESULT_UNKNOWN;
}
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	return 0;
}
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* result_para_name,char* result_para_value)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
	return 0;
}

int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,int val1,
						char* name2,int val2,
						char* name3,float val3,
						char* name4,float val4)
{
	char result_string[512];
	char floatValue_string[512];

	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int

	sprintf(floatValue_string,"%f",val3);
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(floatValue_string));//param3-float

	sprintf(floatValue_string,"%f",val4);
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_string(floatValue_string));//param4-float

	return 0;
}
/*****************************************************************************/
//following function is used for get_wall_geometry mapper.
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,int val1,
						char* name2,int val2,
						char* name3,int val3,
						char* name4,int val4,
						char* name5,char* val5)//,
						//char* name6,char* val6)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	//json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param4-value
	json_object_object_add(pReq->json_resp_obj, name5, json_object_new_string(val5));//param5-value
	//json_object_object_add(pReq->json_resp_obj, name6, json_object_new_string(val6));//param6-value
	return 0;
}
/*****************************************************************************/
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,char* val2,
                        char* name3,char* val3,char* name4,char* val4,
                        char* name5,char* val5)
{
  	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param1-int
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_string(val2));//param2-int
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_string(val4));//param4-value
	json_object_object_add(pReq->json_resp_obj, name5, json_object_new_string(val5));//param5-value
	return 0;
}

int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,char* val1,char* name2,char* val2,
                        char* name3,char* val3,char* name4,char* val4)
{
  	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param1-int
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_string(val2));//param2-int
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_string(val4));//param4-value
	return 0;
}

/*****************************************************************************/
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,int val1,
						char* name2,int val2,
						char* name3,int val3,
						char* name4,int val4,
						char* name5,char* val5,
						char* name6,char* val6)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param4-int
	json_object_object_add(pReq->json_resp_obj, name5, json_object_new_string(val5));//param5-int
	json_object_object_add(pReq->json_resp_obj, name6, json_object_new_string(val6));//param6-int

	return 0;
}

/*****************************************************************************/
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,int val1,char* name2,int val2,
						char* name3,int val3,char* name4,int val4,
						char* name5,int val5,char* name6,int val6,
						char* name7,int val7)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param4-int
	json_object_object_add(pReq->json_resp_obj, name5, json_object_new_int(val5));//param5-int
	json_object_object_add(pReq->json_resp_obj, name6, json_object_new_int(val6));//param6-int
	json_object_object_add(pReq->json_resp_obj, name7, json_object_new_int(val7));//param7-int

	return 0;
}						
/*****************************************************************************/
//following function is used for get_wall_geometry mapper.
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,int val1,
                        char* name2,int val2,
                        char* name3,int val3)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
    //json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
    return 0;
}

int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                                char* name1,int val1,
                                char* name2,int val2)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
    return 0;
}                                
                                
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,char* val1,
                        char* name2,char* val2)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
    //json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param1-char
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_string(val2));//param2-char
    return 0;
}

/*****************************************************************************/
//following function is used for get_cs_measurement.
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,char* val1,
						char* name2,char* val2,
						char* name3,int val3,
						char* name4,int val4,
						char* name5,int val5,
						char* name6,int val6,
						char* name7,int val7,
						char* name8,int val8,
						char* name9,int val9,
						char* name10,int val10)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));

	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param1-value
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_string(val2));//param2-value
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param4-int
	json_object_object_add(pReq->json_resp_obj, name5, json_object_new_int(val5));//param5-int
	json_object_object_add(pReq->json_resp_obj, name6, json_object_new_int(val6));//param6-int
	json_object_object_add(pReq->json_resp_obj, name7, json_object_new_int(val7));//param7-int
	json_object_object_add(pReq->json_resp_obj, name8, json_object_new_int(val8));//param8-int
	json_object_object_add(pReq->json_resp_obj, name9, json_object_new_int(val9));//param9-int
	json_object_object_add(pReq->json_resp_obj, name10, json_object_new_int(val10));//param10-int
	return 0;
}
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,char* name1,int val1,char* name2,int val2,
                    char* name3,int val3,char* name4,int val4,
                    char* name5,int val5,char* name6,int val6)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));

    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-value
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-value
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
    json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param4-int
    json_object_object_add(pReq->json_resp_obj, name5, json_object_new_int(val5));//param5-int
    json_object_object_add(pReq->json_resp_obj, name6, json_object_new_int(val6));//param6-int

    return 0;
}
/*****************************************************************************/
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                            char* name1,double val1,
                            char* name2,double val2,
                            char* name3,double val3)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));

    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_double(val1));//param1-double
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_double(val2));//param2-double
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_double(val3));//param3-double
    return 0;
}

int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                            char* name1,int val1,
                            char* name2,double val2,
                            char* name3,char* val3,
                                                char* name4,int val4,
                                                char* name5,double val5,
                                                char* name6,double val6)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));

    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-double
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_double(val2));//param2-double
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(val3));//param3-double
    json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param1-double
    json_object_object_add(pReq->json_resp_obj, name5, json_object_new_double(val5));//param2-double
    json_object_object_add(pReq->json_resp_obj, name6, json_object_new_double(val6));//param3-double
    return 0;
}

/*****************************************************************************/
//following function is used for get_cs_measurement.
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,double val1,
                        char* name2,double val2,
                        char* name3,double val3,
                        char* name4,double val4,
                        char* name5,double val5,
                        char* name6,double val6,
                        char* name7,double val7,
                        char* name8,double val8,
                        char* name9,double val9)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));

    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_double(val1));//param1-double
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_double(val2));//param2-double
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_double(val3));//param3-double
    json_object_object_add(pReq->json_resp_obj, name4, json_object_new_double(val4));//param4-double
    json_object_object_add(pReq->json_resp_obj, name5, json_object_new_double(val5));//param5-double
    json_object_object_add(pReq->json_resp_obj, name6, json_object_new_double(val6));//param6-double
    json_object_object_add(pReq->json_resp_obj, name7, json_object_new_double(val7));//param7-double
    json_object_object_add(pReq->json_resp_obj, name8, json_object_new_double(val8));//param8-double
    json_object_object_add(pReq->json_resp_obj, name9, json_object_new_double(val9));//param9-double
    return 0;
}

int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
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
                          char* name12,double val12)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));


    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param1-value
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_string(val2));//param2-value
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(val3));//param2-value

    json_object_object_add(pReq->json_resp_obj, name4, json_object_new_double(val4));//param4-double
    json_object_object_add(pReq->json_resp_obj, name5, json_object_new_double(val5));//param5-double
    json_object_object_add(pReq->json_resp_obj, name6, json_object_new_double(val6));//param6-double
    json_object_object_add(pReq->json_resp_obj, name7, json_object_new_double(val7));//param7-double
    json_object_object_add(pReq->json_resp_obj, name8, json_object_new_double(val8));//param8-double
    json_object_object_add(pReq->json_resp_obj, name9, json_object_new_double(val9));//param9-double

    json_object_object_add(pReq->json_resp_obj, name10, json_object_new_double(val10));//param10-double
    json_object_object_add(pReq->json_resp_obj, name11, json_object_new_double(val11));//param11-double
    json_object_object_add(pReq->json_resp_obj, name12, json_object_new_double(val12));//param12-double
    return 0;
}
/*****************************************************************************/
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,int val1,
                        char* name2,int val2,
                        char* name3,char* val3,
                        char* name4,char* val4)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
    //json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int(val1));//param1-int
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(val3));//param3-value
    json_object_object_add(pReq->json_resp_obj, name4, json_object_new_string(val4));//param4-value
    return 0;
}
/*****************************************************************************/
//following function is used for sending back custom background object(1-string,3-int).
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,char* val1,
						char* name2,int val2,
						char* name3,int val3,
						char* name4,int val4)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	//json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param-string
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int(val3));//param3-int
	json_object_object_add(pReq->json_resp_obj, name4, json_object_new_int(val4));//param4-int
	return 0;
}
//following function is used for sending back custom background object(1-string,3-int).
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
						char* name1,char* val1,
						char* name2,char* val2,
						char* name3,char* val3)
						//char* name4,int val4)
{
	char result_string[512];
	pReq->rpc_code=result;
	convert_server_result_to_string(pReq->rpc_code,result_string);
	pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
	json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
	//json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
	json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param-string
	json_object_object_add(pReq->json_resp_obj, name2, json_object_new_string(val2));//param-string
	json_object_object_add(pReq->json_resp_obj, name3, json_object_new_string(val3));//param-string

	return 0;
}

/*****************************************************************************/
//following function is used for get_runtimes
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,unsigned long val1,
                        char* name2,unsigned long val2,
                        char* name3,unsigned long val3)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));

    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_int64(val1));
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int64(val2));
    json_object_object_add(pReq->json_resp_obj, name3, json_object_new_int64(val3));
    return 0;
}
/*****************************************************************************/
int ADCmnStringProcessor::prepare_result_string(RPC_SRV_RESULT result,JsonDataCommObj* pReq,
                        char* name1,char* val1,
                        char* name2,int val2)
{
    char result_string[512];
    pReq->rpc_code=result;
    convert_server_result_to_string(pReq->rpc_code,result_string);
    pReq->json_resp_obj=json_object_new_object();//remember to delete this object after sending response
    json_object_object_add(pReq->json_resp_obj,RPC_NAME_ARG_RESULT_PARAM, json_object_new_string(result_string));
    //json_object_object_add(pReq->json_resp_obj,result_para_name, json_object_new_string(result_para_value));
    json_object_object_add(pReq->json_resp_obj, name1, json_object_new_string(val1));//param1-char
    json_object_object_add(pReq->json_resp_obj, name2, json_object_new_int(val2));//param2-int
    return 0;
}





