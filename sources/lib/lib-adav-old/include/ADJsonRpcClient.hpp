#ifndef __ADJSONRPCCLIENT_H_
#define __ADJSONRPCCLIENT_H_
#include "ADNetClient.hpp"
//#include "DispPanelCmnDef.h"
#include "ADJsonRpcProxy.hpp"
//#include "SdpsJsonDef.h"
#include "ADCmnStringProcessor.hpp"

class ADJsonRpcClient: public ADCmnStringProcessor
{
	int req_id;//unique req id generator
public:
	bool connected;
	ADNetClient ClientSocket;
	char send_buffer[1400];//limit to MTU size
	char recv_buffer[1400];//limit to MTU size

	ADJsonRpcClient();
	~ADJsonRpcClient();
	string get_ip_addr();//returns the ip addr of the server(to connect or to be connected).
	int rpc_server_connect(string ip_addr,int port_num);
	int rpc_server_disconnect(void);

	int prepare_json_request(char* method,int ID,char* result);
	int prepare_json_request(char* method,int ID,char* result,char* name,char* value);//,char* result);
	int prepare_json_request(char* method,int ID,char* result,char* name,int value);//,char* result);

	//for sdps int type with addr argument
	int prepare_json_request(char* method,int ID,char* name  ,int value,char* result);
	int prepare_json_request(char* method,int ID,char* name1,int value1,char* name2, int value2,char* result);//int type
	int prepare_json_request(char* method,int ID,char* name1,int value1,char* name2, char* value2,char* result);//string type

	int prepare_json_request(char* method,int ID,char* result,char* name1,int value1,char* name2, int value2);
	
	int prepare_json_request(char* method,int ID,char* result,char* name1,int value1,char* name2, char* value2);
	
	int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2);
    int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, int value2);

	int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,char* value3);
	int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,char* value3,char* name4,char* value4);
	int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,char* value3,char* name4,char* value4,char* name5,char* value5);
        int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,int value3, char* name4, int value4,char* name5, int value5);
	int prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, int value2,char* name3,int value3, char* name4, int value4);

	int prepare_json_request(char*  method,int ID,char* result,char* name1,char *value1,char* name2,char *value2,char* name3,char *value3,char* name4,char *value4,char* name5,char *value5,char* name6,char* value6,char* name7,char* value7,char* name8,char* value8,char* name9,char* value9);

	int prepare_json_request_four_int(char* method,int ID,char* result,char* name1,int value1,char* name2, int value2,char* name3,int value3, char* name4, int value4);

	//new wall-config with 6param
	int prepare_json_request_four_int_two_string(char* method,int ID,char* result,char* name1,int value1,
					char* name2, int value2,char* name3,int value3, char* name4, int value4,char* name5, char* value5,char* name6, char* value6);


    //color measurement

    int prepare_json_request_three_double(char* method,int ID,char* result,char* name1,double value1,
                        char* name2, double value2,char* name3,double value3);

    int prepare_json_request_three_int_two_string(char* method,int ID,char* result,char* name1,int value1,
                char* name2, int value2,char* name3,int value3, char* name4, char* value4,char* name5, char* value5);
					
    int prepare_json_request_two_int_two_string(char* method,int ID,char* result,char* name1,int value1,
                    char* name2, int value2,char* name3,char* value3, char* name4, char* value4);

    int prepare_json_request_two_int_one_double_one_string(char* method,int ID,char* result,char* name1,int value1,
                    char* name2, double value2,char* name3,char* value3, char* name4, int value4);


    int prepare_json_request_single_int_nine_double_param(char* method,int ID,char* result,char* name1,int value1,
                        char* name2, double value2,char* name3,double value3, char* name4, double value4,char* name5, double value5,char* name6, double value6,
                                                                char* name7, double value7,char* name8, double value8,char* name9, double value9,char* name10, double value10);
    int prepare_json_request_three_string_nine_double_param(char* method,int ID,char* result,char* name1,char* value1,
                        char* name2, char* value2,char* name3,char* value3, char* name4, double value4,char* name5, double value5,char* name6, double value6,
                                                                char* name7, double value7,char* name8, double value8,char* name9, double value9,char* name10, double value10,
                                                                             char* name11, double value11,char* name12, double value12);

    int prepare_json_request_one_string_three_int(char* method,int ID,char* result,char* name1,char* value1,char* name2, int value2,char* name3,int value3, char* name4, int value4);
        
	int prepare_json_request_seven_int_param(char* method,int ID,char* result,char* name1,int value1,
                    char* name2, int value2,char* name3,int value3, char* name4, int value4,char* name5, int value5,char* name6, int value6,
                                                            char* name7, int value7);

	int find_json_result(char *json_string,char* param_name,char* value);
	int find_json_result_and_single_string_param(char *json_string,char* result_name,char* result_value,char* param_name,char* param_value);
	int find_json_result_and_four_int_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val);

	//new wall-config with 6param
	int find_json_result_and_four_int_one_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val,char* para5,char* para5val);

	int find_json_result_one_string_three_int_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val);
    int find_json_result_one_string_one_int_param(char *json_string,char* result_name,char* result_value,
                                char* para1,char* para1val,
                                char* para2,int* para2val);

	int find_json_result_and_three_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,char* para3val);


	int find_json_result_and_two_string_eight_int_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val,
							    char* para5,int* para5val,
							    char* para6,int* para6val,
							    char* para7,int* para7val,							    
							    char* para8,int* para8val,	
							    char* para9,int* para9val,							    
							    char* para10,int* para10val);
	
	int find_json_result_and_five_string_param_statusErrorInfo(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,char* para3val,
							    char* para4,char* para4val,
							    char* para5,char* para5val);
	

    int find_json_result_and_two_int_two_string_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,int* para1val,
                                    char* para2,int* para2val,
                                    char* para3,char* para3val,
                                    char* para4,char* para4val);

    int find_json_result_and_three_int_param(char *json_string,char* result_name,char* result_value,
                                             char* para1,int* para1val,
                                             char* para2,int* para2val,
                                             char* para3,int* para3val);

   /* int find_json_result_and_two_int_two_float_one_string_param(char *json_string,char* result_name,char* result_value,
    							    char* para1,int* para1val,
    							    char* para2,int* para2val,
    							    char* para3,float* para3val,
    							    char* para4,float* para4val);*/

    int find_json_result_and_two_string_param(char *json_string,char* result_name,char* result_value,
    							    char* para1,char* para1val,char* para2,char* para2val);


    int find_json_result_and_nine_double_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,double* para1val,
                                    char* para2,double* para2val,
                                    char* para3,double* para3val,
                                    char* para4,double* para4val,
                                    char* para5,double* para5val,
                                    char* para6,double* para6val,
                                    char* para7,double* para7val,
                                    char* para8,double* para8val,
                                    char* para9,double* para9val);

    int find_json_result_and_three_double_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,double* para1val,
                                    char* para2,double* para2val,
                                    char* para3,double* para3val);
    int find_json_result_and_three_double_delta_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,double* para1val,
                                    char* para2,double* para2val,
                                    char* para3,double* para3val);

    int find_json_result_and_three_string_nine_double_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,char* para1val,
                                    char* para2,char* para2val,
                                    char* para3,char* para3val,
                                    char* para4,double* para4val,
                                    char* para5,double* para5val,
                                    char* para6,double* para6val,
                                    char* para7,double* para7val,
                                    char* para8,double* para8val,
                                    char* para9,double* para9val,
                                     char* para10,double* para10val,
                                     char* para11,double* para11val,
                                     char* para12,double* para12val);
							    
    int find_json_result_and_six_int_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,int* para1val,
                                    char* para2,int* para2val,
                                    char* para3,int* para3val,
                                    char* para4,int* para4val,
                                    char* para5,int* para5val,
                                    char* para6,int* para6val);

	int find_json_result_and_seven_int_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,int* para2val,
                                char* para3,int* para3val,
                                char* para4,int* para4val,
                                char* para5,int* para5val,
                                char* para6,int* para6val,
                                char* para7,int* para7val);
                                
    int find_json_result_and_two_int_three_double_one_string_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,int* para1val,
                                    char* para2,double* para2val,
                                    char* para3,char* para3val,
                                    char* para4,int* para4val,
                                    char* para5,double* para5val,
                                    char* para6,double* para6val);

    int find_json_result_and_two_int_two_double_one_string_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,int* para1val,
                                    char* para2,int* para2val,
                                    char* para3,double* para3val,
                                    char* para4,double* para4val);

    int find_json_result_and_two_int_two_float_one_string_param(char *json_string,char* result_name,char* result_value,
                                    char* para1,int* para1val,
                                    char* para2,int* para2val,
                                    char* para3,float* para3val,
                                    char* para4,float* para4val);
                                    
	int find_json_result_and_two_int_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,int* para2val);

	int find_json_result_and_four_int_two_string_param(char *json_string,char* result_name,char* result_value,
								    char* para1,int* para1val,
								    char* para2,int* para2val,
								    char* para3,int* para3val,
								    char* para4,int* para4val,char* para5,char* para5val,
								    char* para6,char* para6val);

    RPC_SRV_RESULT get_two_string_eight_int_type_with_integer_para(void *Obj);
							    
	RPC_SRV_RESULT set_integer_type(char* method_name,char* method_param_name,char* method_param_value);
	RPC_SRV_RESULT set_integer_type(char* method_name,char* method_param_name,int method_param_value);
	RPC_SRV_RESULT get_integer_type(char* method_name,char* param_name,char* param_value);

	RPC_SRV_RESULT set_integer_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* method_param_name,int method_param_value);
	RPC_SRV_RESULT get_integer_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* resp_param_name,char* result_string);//,char* result_error)
	RPC_SRV_RESULT set_enum_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* method_param_name,char* method_param_value);
	RPC_SRV_RESULT get_enum_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* resp_param_name,char* result_string);

	RPC_SRV_RESULT get_string_type_with_string_para(char* method_name,char* str_para_name,char* str_para_value,char* result_string,char* res_para_name=NULL);

	RPC_SRV_RESULT get_string_type(char* method_name,char* param_name,char* param_value);
	RPC_SRV_RESULT set_single_string_type(char* method_name,char* method_param_name,char* method_param_value);
	RPC_SRV_RESULT set_double_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value);
	RPC_SRV_RESULT set_tripple_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_param_name,char* third_param_value);
	RPC_SRV_RESULT set_four_string_type(char* method_name,char* para1,char* para1val,char* para2,char* para2val,char* para3,char* para3val,char* para4,char* para4val);

	RPC_SRV_RESULT set_tripple_string_get_single_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_param_name,char* third_param_value,char* fourth_param_name,char*fourth_param_value);
	RPC_SRV_RESULT set_single_string_triple_int_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,int second_param_value,char* third_param_name,int third_param_value,char* fourth_param_name,int fourth_param_value);
    RPC_SRV_RESULT set_single_string_single_int_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,int second_param_value);


    RPC_SRV_RESULT set_double_string_triple_int_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_param_name,int third_param_value,char* fourth_param_name,int fourth_param_value,char* fifth_param_name,int fifth_param_value);

	RPC_SRV_RESULT set_double_string_get_single_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_para_name,char* third_para_val);
	RPC_SRV_RESULT set_single_string_get_single_string_type(char* method_name,char* method_param_name,char* method_param_value,char* ret_para_name,char* ret_para_val);
	RPC_SRV_RESULT set_single_string_get_single_string_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* method_param_name,char* method_param_value,char* ret_para_name,char* ret_para_val);

	//int set_action_noarg_type(char* method_name);
	RPC_SRV_RESULT set_action_noarg_type(char* method_name);
	RPC_SRV_RESULT set_action_noarg_get_single_string_type(char* method_name,char* ret_para_name,char* ret_para_val);

	double get_command_execution_time();

	RPC_SRV_RESULT set_four_int_type(char* method_name,char* first_name,int first_value,char* second_name,int second_value,char* third_name,int third_value,char* fourth_name,int fourth_value);
	RPC_SRV_RESULT get_four_int_type(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,int* para3val,char* para4,int* para4val);

	RPC_SRV_RESULT set_one_string_three_int_type(char* method_name,char* strname,char* strval,char* int1name,int int1val ,char* int2name,int int2val ,char* int3name,int int3val);
	//RPC_SRV_RESULT get_one_string_three_int_type(char* method_name,char* strname,char* strval,char* int1name,int* int1val,char* int2name,int* int2val,char* int3name,int* int3val);
	RPC_SRV_RESULT get_one_string_three_int_type(char* method_name,char* para1,char* para1val,char* para2,int* para2val,char* para3,int* para3val,char* para4,int* para4val);
    RPC_SRV_RESULT get_one_string_one_int_type(char* method_name,char* para1,char* para1val,char* para2,int* para2val);


	//new wall-config with 6param
	RPC_SRV_RESULT set_four_int_two_string_type(char* method_name,char* first_name,int first_value,char* second_name,int second_value,char* third_name,int third_value,char* fourth_name,int 		fourth_value,char* fifth_name,char* fifth_value,char* sixth_name,char* sixth_value);
	RPC_SRV_RESULT get_four_int_one_string_with_string_para(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,int* para3val,char* para4,int* para4val,char* para5,char* para5val,char* para6,char* para6val);
        RPC_SRV_RESULT get_string_type_with_string_para_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* str_para_name,char* str_para_value,char* result_string,char* res_para_name);

    RPC_SRV_RESULT get_three_string_with_one_string_para(char* method_name,char* para1,char* para1val, //send eth0/eth1 string type
                                           char* para2,char* para2val, //read addressing-type
                                                                               char* para3,char* para3val, //read ip addr
                                                                               char* para4,char* para4val); //read netmask



    RPC_SRV_RESULT get_three_string_type(char* method_name,char* para1,char* para1val,
							                char* para2,char* para2val,
                                                                        char* para3,char* para3val);

	//CS measurement
	RPC_SRV_RESULT get_two_string_eight_int_type_with_three_int_two_string_para(void *Obj);
    RPC_SRV_RESULT set_two_int_two_string_type(char* method_name,char* first_name,int first_value,char* second_name,int second_value,char* third_name,char* third_value,char* fourth_name,char* fourth_value);
    RPC_SRV_RESULT set_two_int_one_double_one_string_type(char* method_name,char* first_name,int first_value,char* second_name,double second_value,char* third_name,char* third_value,char* fourth_name,int fourth_value);


    RPC_SRV_RESULT get_two_int_two_string_type(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,char* para3val,char* para4,char* para4val);
    RPC_SRV_RESULT get_three_int_type_with_integer_para(void *Obj);
    RPC_SRV_RESULT get_three_double_type_with_string_para(void *Obj);

    RPC_SRV_RESULT get_two_int_two_float_one_string_with_string_para(char* method_name,
    									char* para1,char* para1val,
    									char* para2,int* para2val,
    									char* para3,int* para3val,
                                        char* para4,double* para4val,
                                        char* para5,double* para5val);

	RPC_SRV_RESULT get_two_int_two_float_one_string_with_string_para(char* method_name,
    									char* para1,char* para1val,
    									char* para2,int* para2val,
    									char* para3,int* para3val,
                                        char* para4,float* para4val,
                                        char* para5,float* para5val);

    RPC_SRV_RESULT get_two_int_three_double_one_string_type(char* method_name,char* para1,int* para1val,char* para2,double* para2val,char* para3,char* para3val,char* para4,
                                                            int* para4val,char* para5,double* para5val,char* para6,double* para6val);



    RPC_SRV_RESULT get_two_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val);
    RPC_SRV_RESULT get_three_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val,char* para3,char* para3val);
    RPC_SRV_RESULT get_single_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val);
    RPC_SRV_RESULT get_nine_double_type(void *Obj);
    RPC_SRV_RESULT set_three_double_type(char* method_name,char* first_name,double first_value,char* second_name,double second_value,char* third_name,double third_value);
    RPC_SRV_RESULT get_three_double_type(void *Obj);

    RPC_SRV_RESULT get_six_int_type(void *Obj);
    RPC_SRV_RESULT get_two_int_type_max_min_brightness(void *Obj);
    RPC_SRV_RESULT get_seven_int_type_native_params(void *Obj);
    RPC_SRV_RESULT set_seven_int_type(void *Obj);
        
    RPC_SRV_RESULT get_three_string_nine_double_type(void *Obj);
    RPC_SRV_RESULT get_five_string_type_status_error_info(void *Obj);
    RPC_SRV_RESULT set_single_int_nine_double_type(void *Obj);
    RPC_SRV_RESULT set_three_string_nine_double_type(void *Obj);
	int prepare_json_request(char* method,int ID,char* result,char* name1,unsigned long value1,char* name2, unsigned long value2,char* name3,unsigned long value3);

	//for get/set runtime
	RPC_SRV_RESULT set_three_ulong_type(char* method_name,char* first_name,unsigned long first_value,char* second_name,unsigned long second_value,char* third_name,unsigned long third_value);
	RPC_SRV_RESULT get_three_ulong_type(char* method_name,char* first_name,unsigned long *first_value,char* second_name,unsigned long *second_value,char* third_name,unsigned long *third_value);
	RPC_SRV_RESULT set_single_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val);
	
	RPC_SRV_RESULT set_single_string_with_five_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val, char* para3, char* para3val,char* para4, char* para4val, char* para5, char* para5val);
	
	RPC_SRV_RESULT set_single_string_with_nine_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val, char* para3, char* para3val,char* para4, char* para4val, char* para5, char* para5val, char* para6, char* para6val, char* para7, char* para7val, char *para8, char* para8val, char* para9, char* para9val);

	RPC_SRV_RESULT get_four_int_two_string_with_string_para(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,
										int* para3val,char* para4,int* para4val,char* para5,char* para5val,char* para6,char* para6val,char* para7,char* para7val);

	int find_json_result_and_four_string_param(char *json_string,char* result_name,char* result_value,
								    char* para1,char* para1val,
								    char* para2,char* para2val,
								    char* para3,char* para3val,
								    char* para4,char* para4val);
	RPC_SRV_RESULT get_four_string_type(char* method_name,char* para1,char* para1val,
								                char* para2,char* para2val,char* para3,char* para3val,
								                char* para4,char* para4val);
	RPC_SRV_RESULT set_three_string_type(char* method_name,char* para1,char* para1val,char* para2,char* para2val,char* para3,char* para3val);
	RPC_SRV_RESULT get_six_int_one_string_with_string_para(char* method_name,
										char* para1,char* para1val,
										char* para2,int* para2val,
										char* para3,int* para3val,
										char* para4,int* para4val,
										char* para5,int* para5val,
										char* para6,int* para6val,
										char* para7,int* para7val);
	int find_json_result_and_six_int_one_string_param(char *json_string,char* result_name,char* result_value,
								    char* para1,int* para1val,
								    char* para2,int* para2val,
								    char* para3,int* para3val,
								    char* para4,int* para4val,
								    char* para5,int* para5val,
								    char* para6,int* para6val);

	RPC_SRV_RESULT set_three_int_type(char* method_name,char* int1Nm,int int1Vl,char* int2Nm,int int2Vl,char* int3Nm,int int3Vl);
	RPC_SRV_RESULT set_three_int_get_one_int(char* method_name,char* int1Nm,int int1Vl,char* int2Nm,int int2Vl,char* int3Nm,int int3Vl,char* int4Nm,int *int4Vl);
	int find_json_result_and_single_string_param(char *json_string,char* result_name,char* result_value,char* param_name,int* param_value);

	int prepare_json_request(char* method,int ID,char* name1,int value1,char* name2, int value2,char* name3, int value3,char* result);

};

#endif
