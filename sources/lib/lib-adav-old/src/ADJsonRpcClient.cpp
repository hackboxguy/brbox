//#include <json-c/json.h>
#include <string.h>
#include "ADJsonRpcClient.hpp"
#include "ADCmnCltCmdProcessor.hpp"
#include "JsonCmnDef.h"
#include <iostream>

using namespace std;
/*****************************************************************************/
ADJsonRpcClient::ADJsonRpcClient()
{
	req_id=0;
	connected=false;
}
/*****************************************************************************/
ADJsonRpcClient::~ADJsonRpcClient()
{
	ClientSocket.sock_disconnect();
}
/*****************************************************************************/
int ADJsonRpcClient::rpc_server_connect(string ip_addr,int port_num)
{
	if(connected==true)
		return -1;//already connected
	if(ClientSocket.sock_connect(ip_addr,port_num)==0)
	{
		connected=true;
		return 0;
	}
	else
		return -1;
}
int ADJsonRpcClient::rpc_server_disconnect(void)
{
	if(connected)
		ClientSocket.sock_disconnect();
	connected=false;
	return 0;
}
string ADJsonRpcClient::get_ip_addr()
{
	return ClientSocket.get_ip_addr();
}
/*****************************************************************************/
/*
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name,int value,char* name1, char* value1)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name, json_object_new_int(value));
	json_object_object_add(param_object, name1, json_object_new_string(value1));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
*/

int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result)
{
	json_object *main_object;//,*param_object;
	main_object = json_object_new_object();
	//param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	//json_object_object_add(param_object, name, json_object_new_string(value));
	//json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	//json_object_put(param_object);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name,char* value)//,char* result)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name, json_object_new_string(value));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
/*****************************************************************************/
//for sdps int/string type with addr argument
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name,int value)//,char* result)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name, json_object_new_int(value));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
//for sdps with int-addr and int-value
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* name1,int value1,char* name2, int value2,char* result)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));
	json_object_object_add(param_object, name2, json_object_new_int(value2));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* name1,int value1,char* name2, int value2,char* name3, int value3,char* result)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));
	json_object_object_add(param_object, name2, json_object_new_int(value2));
	json_object_object_add(param_object, name3, json_object_new_int(value3));

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

//for sdps with int-addr and string-value
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* name1,int value1,char* name2, char* value2,char* result)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));
	json_object_object_add(param_object, name2, json_object_new_string(value2));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* name,int value,char* result)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name, json_object_new_int(value));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,int value1,char* name2, int value2)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));
	json_object_object_add(param_object, name2, json_object_new_int(value2));
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,int value1,char* name2, char* value2)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));   //param1-int
	json_object_object_add(param_object, name2, json_object_new_string(value2));//param2-string
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	//printf("-->%s\n", json_object_to_json_string(main_object));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_string(value2));//param2-string
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, int value2)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
    json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}
int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,char* value3)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_string(value2));//param2-string
	json_object_object_add(param_object, name3, json_object_new_string(value3));//param3-string
	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,char* value3,char* name4,char* value4)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_string(value2));//param2-string
	json_object_object_add(param_object, name3, json_object_new_string(value3));//param3-string
	json_object_object_add(param_object, name4, json_object_new_string(value4));//param4-string

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request(char*  method,int ID,char* result,char* name1,char *value1,char* name2,char *value2,char* name3,char *value3,char* name4,char *value4,char* name5,char *value5,char* name6,char* value6,char* name7,char* value7,char* name8,char* value8,char* name9,char* value9)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_string(value2));//param2-string
	json_object_object_add(param_object, name3, json_object_new_string(value3));//param3-string
	json_object_object_add(param_object, name4, json_object_new_string(value4));//param4-string
	json_object_object_add(param_object, name5, json_object_new_string(value5));//param5-string
	json_object_object_add(param_object, name6, json_object_new_string(value6));//param6-string
	json_object_object_add(param_object, name7, json_object_new_string(value7));//param7-string
	json_object_object_add(param_object, name8, json_object_new_string(value8));//param8-string
	json_object_object_add(param_object, name9, json_object_new_string(value9));//param9-string

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));

	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, int value2,char* name3,int value3, char* name4, int value4)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
	json_object_object_add(param_object, name3, json_object_new_int(value3));//param3-int
	json_object_object_add(param_object, name4, json_object_new_int(value4));//param4-value

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,char* value1,char* name2, char* value2,char* name3,int value3, char* name4, int value4,char* name5, int value5)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_string(value2));//param2-string	
	json_object_object_add(param_object, name3, json_object_new_int(value3));//param3-int
	json_object_object_add(param_object, name4, json_object_new_int(value4));//param4-int
	json_object_object_add(param_object, name5, json_object_new_int(value5));//param5-value

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request(char* method,int ID,char* result,char* name1,unsigned long value1,char* name2, unsigned long value2,char* name3,unsigned long value3)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int64(value1));
	json_object_object_add(param_object, name2, json_object_new_int64(value2));
	json_object_object_add(param_object, name3, json_object_new_int64(value3));

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;

}

int ADJsonRpcClient::prepare_json_request(char *method_name,int ID,char* result,char* name1,char* value1,char* name2,char* value2,char* name3,char* value3,char* name4,char* value4,char* name5,char* value5)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();
	
	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method_name));

	json_object_object_add(param_object, name1, json_object_new_string(value1));
	json_object_object_add(param_object, name2, json_object_new_string(value2));
	json_object_object_add(param_object, name3, json_object_new_string(value3));
	json_object_object_add(param_object, name4, json_object_new_string(value4));
	json_object_object_add(param_object, name5, json_object_new_string(value5));

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request_three_int_two_string(char* method,int ID,char* result,char* name1,int value1,
					char* name2, int value2,char* name3,int value3, char* name4, char* value4,char* name5, char* value5)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));//param1-int
	json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
	json_object_object_add(param_object, name3, json_object_new_int(value3));//param3-int

	json_object_object_add(param_object, name4, json_object_new_string(value4));//param4-value
	json_object_object_add(param_object, name5, json_object_new_string(value5));//param5-value

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}


int ADJsonRpcClient::prepare_json_request_four_int(char* method,int ID,char* result,char* name1,int value1,char* name2, int value2,char* name3,int value3, char* name4, int value4)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));//param1-int
	json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
	json_object_object_add(param_object, name3, json_object_new_int(value3));//param3-int
	json_object_object_add(param_object, name4, json_object_new_int(value4));//param4-value

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
int ADJsonRpcClient::prepare_json_request_four_int_two_string(char* method,int ID,char* result,char* name1,int value1,
					char* name2, int value2,char* name3,int value3, char* name4, int value4,char* name5, char* value5,char* name6, char* value6)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_int(value1));//param1-int
	json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
	json_object_object_add(param_object, name3, json_object_new_int(value3));//param3-int
	json_object_object_add(param_object, name4, json_object_new_int(value4));//param4-value

	json_object_object_add(param_object, name5, json_object_new_string(value5));//param5-value
	json_object_object_add(param_object, name6, json_object_new_string(value6));//param6-value

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}

int ADJsonRpcClient::prepare_json_request_two_int_two_string(char* method,int ID,char* result,char* name1,int value1,
                char* name2, int value2,char* name3,char* value3, char* name4, char* value4)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_int(value1));//param1-int
    json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
    json_object_object_add(param_object, name3, json_object_new_string(value3));//param3-value
    json_object_object_add(param_object, name4, json_object_new_string(value4));//param4-value

    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}

int ADJsonRpcClient::prepare_json_request_two_int_one_double_one_string(char* method,int ID,char* result,char* name1,int value1,
                char* name2, double value2,char* name3,char* value3, char* name4, int value4)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_int(value1));//param1-int
    json_object_object_add(param_object, name2, json_object_new_double(value2));//param2-double
    json_object_object_add(param_object, name3, json_object_new_string(value3));//param3-value
    json_object_object_add(param_object, name4, json_object_new_int(value4));//param4-value

    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}



int ADJsonRpcClient::prepare_json_request_seven_int_param(char* method,int ID,char* result,char* name1,int value1,
                    char* name2, int value2,char* name3,int value3, char* name4, int value4,char* name5, int value5,char* name6, int value6,
                                                            char* name7, int value7)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_int(value1));
    json_object_object_add(param_object, name2, json_object_new_int(value2));
    json_object_object_add(param_object, name3, json_object_new_int(value3));
    json_object_object_add(param_object, name4, json_object_new_int(value4));
    json_object_object_add(param_object, name5, json_object_new_int(value5));
    json_object_object_add(param_object, name6, json_object_new_int(value6));
    json_object_object_add(param_object, name7, json_object_new_int(value7));

    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}


int ADJsonRpcClient::prepare_json_request_single_int_nine_double_param(char* method,int ID,char* result,char* name1,int value1,
                    char* name2, double value2,char* name3,double value3, char* name4, double value4,char* name5, double value5,char* name6, double value6,
                                                            char* name7, double value7,char* name8, double value8,char* name9, double value9,char* name10, double value10)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_int(value1));
    json_object_object_add(param_object, name2, json_object_new_double(value2));
    json_object_object_add(param_object, name3, json_object_new_double(value3));
    json_object_object_add(param_object, name4, json_object_new_double(value4));
    json_object_object_add(param_object, name5, json_object_new_double(value5));
    json_object_object_add(param_object, name6, json_object_new_double(value6));
    json_object_object_add(param_object, name7, json_object_new_double(value7));
    json_object_object_add(param_object, name8, json_object_new_double(value8));
    json_object_object_add(param_object, name9, json_object_new_double(value9));
    json_object_object_add(param_object, name10, json_object_new_double(value10));

    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}

int ADJsonRpcClient::prepare_json_request_three_double(char* method,int ID,char* result,char* name1,double value1,
                    char* name2, double value2,char* name3,double value3)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_double(value1));
    json_object_object_add(param_object, name2, json_object_new_double(value2));
    json_object_object_add(param_object, name3, json_object_new_double(value3));

    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}


int ADJsonRpcClient::prepare_json_request_three_string_nine_double_param(char* method,int ID,char* result,char* name1,char* value1,
                    char* name2, char* value2,char* name3,char* value3, char* name4, double value4,char* name5, double value5,char* name6, double value6,
                                                            char* name7, double value7,char* name8, double value8,char* name9, double value9,char* name10, double value10,
                                                                         char* name11, double value11,char* name12, double value12)
{
    json_object *main_object,*param_object;
    main_object = json_object_new_object();
    param_object= json_object_new_object();

    json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
    json_object_object_add(main_object, "method" , json_object_new_string(method));

    json_object_object_add(param_object, name1, json_object_new_string(value1));
    json_object_object_add(param_object, name2, json_object_new_string(value2));
    json_object_object_add(param_object, name3, json_object_new_string(value3));
    json_object_object_add(param_object, name4, json_object_new_double(value4));
    json_object_object_add(param_object, name5, json_object_new_double(value5));
    json_object_object_add(param_object, name6, json_object_new_double(value6));
    json_object_object_add(param_object, name7, json_object_new_double(value7));
    json_object_object_add(param_object, name8, json_object_new_double(value8));
    json_object_object_add(param_object, name9, json_object_new_double(value9));
    json_object_object_add(param_object, name10, json_object_new_double(value10));
    json_object_object_add(param_object, name11, json_object_new_double(value11));
    json_object_object_add(param_object, name12, json_object_new_double(value12));
    json_object_object_add(main_object,"params", param_object);

    json_object_object_add(main_object, "id", json_object_new_int(ID));
    strcpy(result,json_object_to_json_string(main_object));

    json_object_put(main_object);
    json_object_put(param_object);
    return 0;
}
/*****************************************************************************/
//i know, this is getting ugly now!!!
int ADJsonRpcClient::prepare_json_request_one_string_three_int(char* method,int ID,char* result,char* name1,char* value1,char* name2, int value2,char* name3,int value3, char* name4, int value4)
{
	json_object *main_object,*param_object;
	main_object = json_object_new_object();
	param_object= json_object_new_object();

	json_object_object_add(main_object, "jsonrpc", json_object_new_string("2.0"));
	json_object_object_add(main_object, "method" , json_object_new_string(method));

	json_object_object_add(param_object, name1, json_object_new_string(value1));//param1-string
	json_object_object_add(param_object, name2, json_object_new_int(value2));//param2-int
	json_object_object_add(param_object, name3, json_object_new_int(value3));//param3-int
	json_object_object_add(param_object, name4, json_object_new_int(value4));//param4-value

	json_object_object_add(main_object,"params", param_object);

	json_object_object_add(main_object, "id", json_object_new_int(ID));
	strcpy(result,json_object_to_json_string(main_object));
	
	json_object_put(main_object);
	json_object_put(param_object);
	return 0;
}
/*****************************************************************************/
int ADJsonRpcClient::find_json_result(char *json_string,char* param_name,char* value)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, param_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(value,"%s",json_object_get_string(new_obj2));
	json_object_put(new_obj);
	return 0;
}
int ADJsonRpcClient::find_json_result_and_single_string_param(char *json_string,char* result_name,char* result_value,char* param_name,char* param_value)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for param_name string in result object.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, param_name);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(param_value,"%s",json_object_get_string(new_obj3));
	//printf("para_val = %s\n",param_value);
	json_object_put(new_obj);
	return 0;
}
int ADJsonRpcClient::find_json_result_and_single_string_param(char *json_string,char* result_name,char* result_value,char* param_name,int* param_value)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for param_name string in result object.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, param_name);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	//sprintf(param_value,"%d",json_object_get_int(new_obj3));
	*param_value=json_object_get_int(new_obj3);

	json_object_put(new_obj);
	return 0;
}
int ADJsonRpcClient::find_json_result_and_four_int_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1name and para1val.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para1val=atoi(json_object_get_string(new_obj3));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para2name and para2val.
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para2val=atoi(json_object_get_string(new_obj4));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));

	//search for para3name and para3val.
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atoi(json_object_get_string(new_obj5));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para3name and para3val.
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atoi(json_object_get_string(new_obj6));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));

	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_and_three_int_param(char *json_string,char* result_name,char* result_value,
                                         char* para1,int* para1val,
                                         char* para2,int* para2val,
                                         char* para3,int* para3val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1name and para1val.
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_BLK_LIGHT_MEAS_RED);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        //printf("para name %s not found\n",param_name);
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val=atoi(json_object_get_string(new_obj3));
    //sprintf(para1val,"%s",json_object_get_string(new_obj3));


    //search for para2name and para2val.
    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_BLK_LIGHT_MEAS_GREEN);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        //printf("para name %s not found\n",param_name);
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atoi(json_object_get_string(new_obj4));
    //sprintf(para1val,"%s",json_object_get_string(new_obj3));

    //search for para3name and para3val.
    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_BLK_LIGHT_MEAS_BLUE);//para_value is still part of result object
    if(json_object_get_string(new_obj5)==NULL)
    {
        //printf("para name %s not found\n",param_name);
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para3val=atoi(json_object_get_string(new_obj5));
    //sprintf(para1val,"%s",json_object_get_string(new_obj3));

    json_object_put(new_obj);
    return 0;
}


int ADJsonRpcClient::find_json_result_and_three_string_nine_double_param(char *json_string,char* result_name,char* result_value,
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
                                 char* para12,double* para12val)
{
    //printf("test0\n");
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //printf("test1\n");
    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //printf("test2\n");

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //printf("test3\n");
    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_CALIB_TIME_STAMP_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    sprintf(para1val,"%s",json_object_get_string(new_obj3));


    //printf("test4\n");
    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_CALIB_PROD_DATA_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    sprintf(para2val,"%s",json_object_get_string(new_obj4));

    //printf("test5\n");
    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_ARG_CALIB_PROC_VERSION_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    sprintf(para3val,"%s",json_object_get_string(new_obj5));


    //printf("test6\n");
    struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_ARG_RED_X);//para_value is still part of result object
    if(new_obj6 == NULL)
    //if(json_object_get_double(new_obj6)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para4val=atof(json_object_get_string(new_obj6));


    //printf("test7\n");
    struct json_object *new_obj7 = json_object_object_get(new_obj1, RPC_ARG_RED_Y);//para_value is still part of result object
    if(new_obj7 == NULL)
    //if(json_object_get_double(new_obj7)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para5val=atof(json_object_get_string(new_obj7));


    //printf("test8\n");
    struct json_object *new_obj8 = json_object_object_get(new_obj1, RPC_ARG_RED_Z);//para_value is still part of result object
    if(new_obj8 == NULL)
    //if(json_object_get_double(new_obj8)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para6val=atof(json_object_get_string(new_obj8));


    //printf("test9\n");
    struct json_object *new_obj9 = json_object_object_get(new_obj1, RPC_ARG_GREEN_X);//para_value is still part of result object
    if(new_obj9 == NULL)
    //if(json_object_get_double(new_obj9)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para7val=atof(json_object_get_string(new_obj9));


    //printf("test10\n");
    struct json_object *new_obj10 = json_object_object_get(new_obj1, RPC_ARG_GREEN_Y);//para_value is still part of result object
    if(new_obj10 == NULL)
    //if(json_object_get_double(new_obj10)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para8val=atof(json_object_get_string(new_obj10));


    //printf("test11\n");
    struct json_object *new_obj11 = json_object_object_get(new_obj1, RPC_ARG_GREEN_Z);//para_value is still part of result object
    if(new_obj11 == NULL)
    //if(json_object_get_double(new_obj11)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para9val=atof(json_object_get_string(new_obj11));


    //printf("test12\n");
    struct json_object *new_obj12 = json_object_object_get(new_obj1, RPC_ARG_BLUE_X);//para_value is still part of result object
    if(new_obj12 == NULL)
    //if(json_object_get_double(new_obj12)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para10val=atof(json_object_get_string(new_obj12));


    //printf("test13\n");
    struct json_object *new_obj13 = json_object_object_get(new_obj1, RPC_ARG_BLUE_Y);//para_value is still part of result object
    if(new_obj13 == NULL)
    //if(json_object_get_double(new_obj13)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para11val=atof(json_object_get_string(new_obj13));


    //printf("test14\n");
    struct json_object *new_obj14 = json_object_object_get(new_obj1, RPC_ARG_BLUE_Z);//para_value is still part of result object
    if(new_obj14 == NULL)
    //if(json_object_get_double(new_obj14)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para12val=atof(json_object_get_string(new_obj14));

    //printf("%s\t%s\t%s\t%f\t%f\n",para1val,para2val,para3val,*para4val,*para5val);
    json_object_put(new_obj);
    return 0;
}

int ADJsonRpcClient::find_json_result_and_three_double_delta_param(char *json_string,char* result_name,char* result_value,
                                char* para1,double* para1val,
                                char* para2,double* para2val,
                                char* para3,double* para3val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //printf("test1\n");

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    //printf("test2\n");

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));
    //printf("test3\n");

    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
    if(new_obj3 == NULL)
    //if(json_object_get_double(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val=atof(json_object_get_string(new_obj3));
    //printf("test4\n");

    struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
    if(new_obj4 == NULL)
    //if(json_object_get_double(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atof(json_object_get_string(new_obj4));
    //printf("test5\n");

    struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
    if(new_obj5 == NULL)
    //if(json_object_get_double(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para3val=atof(json_object_get_string(new_obj5));
    //printf("test6\n");
    //printf("find_json_result_and_three_double_delta_param = %f and %f and %f\n",*para1val,*para2val,*para3val);

    json_object_put(new_obj);
    return 0;
}

int ADJsonRpcClient::find_json_result_and_three_double_param(char *json_string,char* result_name,char* result_value,
                                char* para1,double* para1val,
                                char* para2,double* para2val,
                                char* para3,double* para3val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_X);//para_value is still part of result object
    if(new_obj3 == NULL)
    //if(json_object_get_double(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val=atof(json_object_get_string(new_obj3));

    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_Y);//para_value is still part of result object
    if(new_obj4 == NULL)
    //if(json_object_get_double(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atof(json_object_get_string(new_obj4));

    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_ARG_Z);//para_value is still part of result object
    if(new_obj5 == NULL)
    //if(json_object_get_double(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para3val=atof(json_object_get_string(new_obj5));

    json_object_put(new_obj);
    return 0;
}

int ADJsonRpcClient::find_json_result_and_nine_double_param(char *json_string,char* result_name,char* result_value,
                                char* para1,double* para1val,
                                char* para2,double* para2val,
                                char* para3,double* para3val,
                                char* para4,double* para4val,
                                char* para5,double* para5val,
                                char* para6,double* para6val,
                                char* para7,double* para7val,
                                char* para8,double* para8val,
                                char* para9,double* para9val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_RED_X);//para_value is still part of result object
    if(new_obj3 == NULL)
    //if(json_object_get_double(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val=atof(json_object_get_string(new_obj3));

    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_RED_Y);//para_value is still part of result object
    if(new_obj4 == NULL)
    //if(json_object_get_double(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atof(json_object_get_string(new_obj4));

    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_ARG_RED_Z);//para_value is still part of result object
    if(new_obj5 == NULL)
    //if(json_object_get_double(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para3val=atof(json_object_get_string(new_obj5));

    struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_ARG_GREEN_X);//para_value is still part of result object
    if(new_obj6 == NULL)
    //if(json_object_get_double(new_obj6)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para4val=atof(json_object_get_string(new_obj6));

    struct json_object *new_obj7 = json_object_object_get(new_obj1, RPC_ARG_GREEN_Y);//para_value is still part of result object
    if(new_obj7 == NULL)
    //if(json_object_get_double(new_obj7)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para5val=atof(json_object_get_string(new_obj7));

    struct json_object *new_obj8 = json_object_object_get(new_obj1, RPC_ARG_GREEN_Z);//para_value is still part of result object
    if(new_obj8 == NULL)
    //if(json_object_get_double(new_obj8)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para6val=atof(json_object_get_string(new_obj8));

    struct json_object *new_obj9 = json_object_object_get(new_obj1, RPC_ARG_BLUE_X);//para_value is still part of result object
    if(new_obj9 == NULL)
    //if(json_object_get_double(new_obj9)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para7val=atof(json_object_get_string(new_obj9));

    struct json_object *new_obj10 = json_object_object_get(new_obj1, RPC_ARG_BLUE_Y);//para_value is still part of result object
    if(new_obj10 == NULL)
    //if(json_object_get_double(new_obj10)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para8val=atof(json_object_get_string(new_obj10));

    struct json_object *new_obj11 = json_object_object_get(new_obj1, RPC_ARG_BLUE_Z);//para_value is still part of result object
    if(new_obj11 == NULL)
    //if(json_object_get_double(new_obj11)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para9val=atof(json_object_get_string(new_obj11));

    json_object_put(new_obj);
    return 0;
}


int ADJsonRpcClient::find_json_result_and_seven_int_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,int* para2val,
                                char* para3,int* para3val,
                                char* para4,int* para4val,
                                char* para5,int* para5val,
                                char* para6,int* para6val,
                                char* para7,int* para7val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val= atoi(json_object_get_string(new_obj3));


    //search for para2(str)
    struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
   *para2val = atoi(json_object_get_string(new_obj4));

    //search for para3(int)
    struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
    if(json_object_get_string(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para3val=atoi(json_object_get_string(new_obj5));


    //search for para4(int)
    struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
    if(json_object_get_string(new_obj6)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para4val=atoi(json_object_get_string(new_obj6));

    //search for para5(int)
    struct json_object *new_obj7 = json_object_object_get(new_obj1, para5);//para_value is still part of result object
    if(json_object_get_string(new_obj7)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para5val=atoi(json_object_get_string(new_obj7));

    //search for para6(int)
    struct json_object *new_obj8 = json_object_object_get(new_obj1, para6);//para_value is still part of result object
    if(json_object_get_string(new_obj8)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para6val=atoi(json_object_get_string(new_obj8));
    
    //search for para7(int)
    struct json_object *new_obj9 = json_object_object_get(new_obj1, para7);//para_value is still part of result object
    if(json_object_get_string(new_obj9)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para7val=atoi(json_object_get_string(new_obj9));

    json_object_put(new_obj);
    return 0;
}


int ADJsonRpcClient::find_json_result_and_two_int_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,int* para2val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val= atoi(json_object_get_string(new_obj3));


    //search for para2(str)
    struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
   *para2val = atoi(json_object_get_string(new_obj4));

    json_object_put(new_obj);
    return 0;
}


int ADJsonRpcClient::find_json_result_and_six_int_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,int* para2val,
                                char* para3,int* para3val,
                                char* para4,int* para4val,
                                char* para5,int* para5val,
                                char* para6,int* para6val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(str)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_RED);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val= atoi(json_object_get_string(new_obj3));


    //search for para2(str)
    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_GREEN);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
   *para2val = atoi(json_object_get_string(new_obj4));

    //search for para3(int)
    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_BLUE);//para_value is still part of result object
    if(json_object_get_string(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para3val=atoi(json_object_get_string(new_obj5));


    //search for para4(int)
    struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_RED);//para_value is still part of result object
    if(json_object_get_string(new_obj6)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para4val=atoi(json_object_get_string(new_obj6));

    //search for para5(int)
    struct json_object *new_obj7 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_GREEN);//para_value is still part of result object
    if(json_object_get_string(new_obj7)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para5val=atoi(json_object_get_string(new_obj7));

    //search for para6(int)
    struct json_object *new_obj8 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_BLUE);//para_value is still part of result object
    if(json_object_get_string(new_obj8)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para6val=atoi(json_object_get_string(new_obj8));

    json_object_put(new_obj);
    return 0;
}


int ADJsonRpcClient::find_json_result_and_two_string_eight_int_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val,
							    char* para5,int* para5val,
							    char* para6,int* para6val,
							    char* para7,int* para7val,							    
							    char* para8,int* para8val,	
							    char* para9,int* para9val,							    
							    char* para10,int* para10val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(str)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_COL_SEN0_RESULT_PARAM);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para1val,"%s",json_object_get_string(new_obj3));
	

	//search for para2(str)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_COL_SEN1_RESULT_PARAM);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para2val,"%s",json_object_get_string(new_obj4));

	//search for para3(int)
	struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_RED);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atoi(json_object_get_string(new_obj5));


	//search for para4(int)
	struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_GREEN);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atoi(json_object_get_string(new_obj6));

	//search for para5(int)
	struct json_object *new_obj7 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_BLUE);//para_value is still part of result object
	if(json_object_get_string(new_obj7)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para5val=atoi(json_object_get_string(new_obj7));

	//search for para6(int)
	struct json_object *new_obj8 = json_object_object_get(new_obj1, RPC_COL_SENSOR0_WHITE);//para_value is still part of result object
	if(json_object_get_string(new_obj8)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para6val=atoi(json_object_get_string(new_obj8));

	//search for para7(int)
	struct json_object *new_obj9 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_RED);//para_value is still part of result object
	if(json_object_get_string(new_obj9)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para7val=atoi(json_object_get_string(new_obj9));

	//search for para8(int)
	struct json_object *new_obj10 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_GREEN);//para_value is still part of result object
	if(json_object_get_string(new_obj10)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para8val=atoi(json_object_get_string(new_obj10));

	//search for para9(int)
	struct json_object *new_obj11 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_BLUE);//para_value is still part of result object
	if(json_object_get_string(new_obj11)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para9val=atoi(json_object_get_string(new_obj11));

	//search for para10(int)
	struct json_object *new_obj12 = json_object_object_get(new_obj1, RPC_COL_SENSOR1_WHITE);//para_value is still part of result object
	if(json_object_get_string(new_obj12)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para10val=atoi(json_object_get_string(new_obj12));

	json_object_put(new_obj);
	return 0;
}



//To be used only for Color sensor parameters.
int ADJsonRpcClient::find_json_result_and_two_int_two_string_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,int* para2val,
                                char* para3,char* para3val,
                                char* para4,char* para4val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(int)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_COL_SENSOR_GAIN_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val=atoi(json_object_get_string(new_obj3));


    //search for para2(int)
    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_COL_SENSOR_PRESCALE_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atoi(json_object_get_string(new_obj4));

    //search for para3(string)
    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_ARG_COL_SENSOR_PUL_PERIOD_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    sprintf(para3val,"%s",json_object_get_string(new_obj5));

    //search for para4(string)
    struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_ARG_COL_SENSOR_NUM_PULSES_PARAM);//para_value is still part of result object
    if(json_object_get_string(new_obj6)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    sprintf(para4val,"%s",json_object_get_string(new_obj6));

    json_object_put(new_obj);
    return 0;
}

int ADJsonRpcClient::find_json_result_and_four_int_one_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val,char* para5,char* para5val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(int)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para1val=atoi(json_object_get_string(new_obj3));


	//search for para2(int)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para2val=atoi(json_object_get_string(new_obj4));

	//search for para3(int)
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atoi(json_object_get_string(new_obj5));

	//search for para4(int)
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atoi(json_object_get_string(new_obj6));

	//search for para5(string)
	struct json_object *new_obj7 = json_object_object_get(new_obj1, para5);//para_value is still part of result object
	if(json_object_get_string(new_obj7)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para5val,"%s",json_object_get_string(new_obj7));

	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_and_four_int_two_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val,
							    char* para5,char* para5val,
							    char* para6,char* para6val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;

	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(int)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para1val=atoi(json_object_get_string(new_obj3));


	//search for para2(int)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para2val=atoi(json_object_get_string(new_obj4));

	//search for para3(int)
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atoi(json_object_get_string(new_obj5));

	//search for para4(int)
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atoi(json_object_get_string(new_obj6));

	//search for para5(string)
	struct json_object *new_obj7 = json_object_object_get(new_obj1, para5);//para_value is still part of result object
	if(json_object_get_string(new_obj7)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para5val,"%s",json_object_get_string(new_obj7));
	//search for para6(string)
	struct json_object *new_obj8 = json_object_object_get(new_obj1, para6);//para_value is still part of result object
	if(json_object_get_string(new_obj8)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para6val,"%s",json_object_get_string(new_obj8));
	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_and_two_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,char* para2,char* para2val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;

	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(string)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para1val,"%s",json_object_get_string(new_obj3));

	//search for para2(string)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para2val,"%s",json_object_get_string(new_obj4));

	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_and_two_int_three_double_one_string_param(char *json_string,char* result_name,char* result_value,
                                char* para1,int* para1val,
                                char* para2,double* para2val,
                                char* para3,char* para3val,
                                char* para4,int* para4val,
                                char* para5,double* para5val,
                                char* para6,double* para6val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1(int)
    struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_TARGET_COLOR_TEMP);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para1val=atoi(json_object_get_string(new_obj3));

    struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_TARGET_LOCUS_OFFSET);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atof(json_object_get_string(new_obj4));

    //search for para3(int)
    struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_ARG_TARGET_GAMMA_FACTOR);//para_value is still part of result object
    if(json_object_get_string(new_obj5)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    sprintf(para3val,"%s",json_object_get_string(new_obj5));

    //search for para3(int)
    struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_ARG_TARGET_BRIGHTNESS);//para_value is still part of result object
    if(json_object_get_string(new_obj6)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para4val=atoi(json_object_get_string(new_obj6));

    struct json_object *new_obj7 = json_object_object_get(new_obj1, RPC_ARG_TARGET_x);//para_value is still part of result object
    if(json_object_get_string(new_obj7)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para5val=atof(json_object_get_string(new_obj7));

    struct json_object *new_obj8 = json_object_object_get(new_obj1, RPC_ARG_TARGET_y);//para_value is still part of result object
    if(json_object_get_string(new_obj8)==NULL)
    {
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para6val=atof(json_object_get_string(new_obj8));

    json_object_put(new_obj);
    return 0;
}

int ADJsonRpcClient::find_json_result_and_two_int_two_double_one_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
                                char* para3,double* para3val,
                                char* para4,double* para4val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;

	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(int)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para1val=atoi(json_object_get_string(new_obj3));


	//search for para2(int)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}

	*para2val=atoi(json_object_get_string(new_obj4));

	//search for para3(int)
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atof(json_object_get_string(new_obj5));

	//search for para3(int)
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atof(json_object_get_string(new_obj6));

	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_and_two_int_two_float_one_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
                                char* para3,float* para3val,
                                char* para4,float* para4val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;

	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(int)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para1val=atoi(json_object_get_string(new_obj3));


	//search for para2(int)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}

	*para2val=atoi(json_object_get_string(new_obj4));

	//search for para3(int)
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atof(json_object_get_string(new_obj5));

	//search for para3(int)
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atof(json_object_get_string(new_obj6));

	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_one_string_three_int_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1name and para1val.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	//*para1val=atoi(json_object_get_string(new_obj3));
	sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para2name and para2val.
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para2val=atoi(json_object_get_string(new_obj4));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));

	//search for para3name and para3val.
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atoi(json_object_get_string(new_obj5));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para3name and para3val.
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		//printf("para name %s not found\n",param_name);
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atoi(json_object_get_string(new_obj6));
	//sprintf(para1val,"%s",json_object_get_string(new_obj3));

	json_object_put(new_obj);
	return 0;
}
int ADJsonRpcClient::find_json_result_one_string_one_int_param(char *json_string,char* result_name,char* result_value,
                            char* para1,char* para1val,
                            char* para2,int* para2val)
{
    struct json_object *new_obj=NULL;
    new_obj = json_tokener_parse(json_string);
    if(new_obj==NULL)
        return -1;

    //first search for "result" string in response object
    struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
    if(json_object_get_string(new_obj1)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }

    //search for "return" string in result object.
    struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
    if(json_object_get_string(new_obj2)==NULL)
    {
        json_object_put(new_obj);
        return -1;
    }
    sprintf(result_value,"%s",json_object_get_string(new_obj2));

    //search for para1name and para1val.
    struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
    if(json_object_get_string(new_obj3)==NULL)
    {
        //printf("para name %s not found\n",param_name);
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    //*para1val=atoi(json_object_get_string(new_obj3));
    sprintf(para1val,"%s",json_object_get_string(new_obj3));


    //search for para2name and para2val.
    struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
    if(json_object_get_string(new_obj4)==NULL)
    {
        //printf("para name %s not found\n",param_name);
        json_object_put(new_obj);
        return 0;//!!! dont return -1 here, atleast result string was found.
    }
    *para2val=atoi(json_object_get_string(new_obj4));
    //sprintf(para1val,"%s",json_object_get_string(new_obj3));

    json_object_put(new_obj);
    return 0;
}

int ADJsonRpcClient::find_json_result_and_five_string_param_statusErrorInfo(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,char* para3val,
							    char* para4,char* para4val,
							    char* para5,char* para5val)
{
  	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1name and para1val.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, RPC_ARG_COLOR_OUT_OF_TOLERANCE);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para2name and para2val.
	struct json_object *new_obj4 = json_object_object_get(new_obj1, RPC_ARG_COLOR_NOT_REACHABLE);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para2val,"%s",json_object_get_string(new_obj4));

	//search for para3name and para3val.
	struct json_object *new_obj5 = json_object_object_get(new_obj1, RPC_ARG_BRIGHTNESS_NOT_REACHABLE);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para3val,"%s",json_object_get_string(new_obj5));
	
	//search for para4name and para4val.
	struct json_object *new_obj6 = json_object_object_get(new_obj1, RPC_ARG_SPECTRAL_CALIB_MISSING);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para4val,"%s",json_object_get_string(new_obj6));

      //search for para5name and para5val.
	struct json_object *new_obj7 = json_object_object_get(new_obj1, RPC_ARG_SENSOR_STATUS);//para_value is still part of result object
	if(json_object_get_string(new_obj7)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para5val,"%s",json_object_get_string(new_obj7));
	
	json_object_put(new_obj);
	return 0;
  
}

int ADJsonRpcClient::find_json_result_and_four_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,char* para3val,
							    char* para4,char* para4val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;

	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1name and para1val.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para2name and para2val.
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para2val,"%s",json_object_get_string(new_obj4));

	//search for para3name and para3val.
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para3val,"%s",json_object_get_string(new_obj5));

	//search for para4name and para4val.
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para4val,"%s",json_object_get_string(new_obj6));

	json_object_put(new_obj);
	return 0;
}

int ADJsonRpcClient::find_json_result_and_three_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,char* para1val,
							    char* para2,char* para2val,
							    char* para3,char* para3val)
							    //char* para4,int* para4val)
{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;
	
	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1name and para1val.
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para1val,"%s",json_object_get_string(new_obj3));


	//search for para2name and para2val.
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para2val,"%s",json_object_get_string(new_obj4));

	//search for para3name and para3val.
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	sprintf(para3val,"%s",json_object_get_string(new_obj5));


	json_object_put(new_obj);
	return 0;
}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::set_integer_type(char* method_name,char* method_param_name,int method_param_value)
{
	char result_string[255];
	//{ "jsonrpc": "2.0", "method": "set_brightness", "params": {"value": 50 }, "id": 0 }
	prepare_json_request(method_name,req_id++,send_buffer,method_param_name,method_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}


RPC_SRV_RESULT ADJsonRpcClient::set_integer_type(char* method_name,char* method_param_name,char* method_param_value)
{
	char result_string[255];
	//{ "jsonrpc": "2.0", "method": "set_brightness", "params": {"value": 50 }, "id": 0 }
	prepare_json_request(method_name,req_id++,send_buffer,method_param_name,method_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_integer_type(char* method_name,char* param_name,char* param_value)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "get_fpga_dev_access", "id": 0 }
	//<--{ "jsonrpc": "2.0", "result": { "return": "Success", "access": "on" }, "id": 0 }
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	sprintf(param_value,"NotFound");//initialize this string as NotFound in the beginning, after parsing result string, it will be filled with correct value.
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,param_name,param_value)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
//get/set just the integer type without extra device_address(for uart use case) parameter
RPC_SRV_RESULT ADJsonRpcClient::set_integer_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* method_param_name,int method_param_value)
{
	char result_string[255];
	//{ "jsonrpc": "2.0", "method": "set_brightness", "params": { "addr": 0, "value": 50 }, "id": 0 }
	prepare_json_request(method_name,req_id++,addr_para_name,addr_para_value,method_param_name,method_param_value,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//6sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_integer_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* resp_param_name,char* result_string)//,char* result_error)
{
	char return_string[255];
	//-->{ "jsonrpc": "2.0", "method": "get_active_source","params": {"addr": 0},"id": 10}
	prepare_json_request(method_name,req_id++,addr_para_name,addr_para_value,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

	sprintf(result_string,"unknown");//initialize this string as NotFound in the beginning, after parsing result string, it will be filled with correct value.
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,return_string,resp_param_name,/*result_int_str*/result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(return_string);
}
//get/set three integers (notify event)
RPC_SRV_RESULT ADJsonRpcClient::set_three_int_type(char* method_name,char* int1Nm,int int1Vl,char* int2Nm,int int2Vl,char* int3Nm,int int3Vl)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,int1Nm,int1Vl,int2Nm,int2Vl,int3Nm,int3Vl,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//6sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
//event subscription
RPC_SRV_RESULT ADJsonRpcClient::set_three_int_get_one_int(char* method_name,char* int1Nm,int int1Vl,char* int2Nm,int int2Vl,char* int3Nm,int int3Vl,char* int4Nm,int *int4Vl)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,int1Nm,int1Vl,int2Nm,int2Vl,int3Nm,int3Vl,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//6sec timeout
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,int4Nm,int4Vl)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
//get/set enum type without extra device_address(for uart use case) parameter
RPC_SRV_RESULT ADJsonRpcClient::set_enum_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* method_param_name,char* method_param_value)
{
	char result_string[255];
	//{ "jsonrpc": "2.0", "method": "set_brightness", "params": { "addr": 0, "power": "on" }, "id": 0 }
	prepare_json_request(method_name,req_id++,addr_para_name,addr_para_value,method_param_name,method_param_value,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_enum_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* resp_param_name,char* result_string)
{
	char return_string[255];
	//-->{ "jsonrpc": "2.0", "method": "get_active_source","params": {"addr": 0},"id": 10}
	prepare_json_request(method_name,req_id++,addr_para_name,addr_para_value,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

	sprintf(result_string,"unknown");//initialize this string as NotFound in the beginning, after parsing result string, it will be filled with correct value.
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,return_string,resp_param_name,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(return_string);
}

/*****************************************************************************/
//get/set just the string type without extra device_address(for uart use case) parameter
RPC_SRV_RESULT ADJsonRpcClient::get_string_type_with_string_para(char* method_name,char* str_para_name,char* str_para_value,char* result_string,char* res_para_name)
{
	char return_string[255];
	//-->{ "jsonrpc": "2.0", "method": "get_active_source","params": {"interface": "eth0"},"id": 10}
	prepare_json_request(method_name,req_id++,send_buffer,str_para_name,str_para_value);
	//printf("send buffer = %s\n",send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

	sprintf(result_string,"unknown");//initialize this string as NotFound in the beginning, after parsing result string, it will be filled with correct value.
	if(res_para_name!=NULL)//return string param name is different
	{
		if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,return_string,res_para_name,result_string)!=0)
			return RPC_SRV_RESULT_UNKNOWN;
	}
	else
	{
		if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,return_string,str_para_name,result_string)!=0)
			return RPC_SRV_RESULT_UNKNOWN;
	}	
	return convert_string_to_server_result(return_string);
}

//get/set just the string type with extra device_address(for uart use case) parameter

RPC_SRV_RESULT ADJsonRpcClient::get_string_type_with_string_para_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* str_para_value,char* str_para_name,char* result_string,char* res_para_name)
{
	char return_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,addr_para_name,addr_para_value,str_para_name,str_para_value);
	
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

	sprintf(result_string,"unknown");//initialize this string as NotFound in the beginning, after parsing result string, it will be filled with correct value.
	if(res_para_name!=NULL)//return string param name is different
	{
		if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,return_string,res_para_name,result_string)!=0)
			return RPC_SRV_RESULT_UNKNOWN;
	}
	else
	{
		if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,return_string,str_para_name,result_string)!=0)
			return RPC_SRV_RESULT_UNKNOWN;
	}	
	return convert_string_to_server_result(return_string);
}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::set_double_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_edid_data", "params": { "input": "dvi1", "edidtype": "custom", "filepath": "/etc/edid/1080p60.bin" }, "id": 0 }
	prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::set_tripple_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_param_name,char* third_param_value)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_fpga_logic","params": {"action": "config","filepath": "/tmp/fpga.bit"} ,"id": 10}
	prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value,third_param_name,third_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::set_four_string_type(char* method_name,char* para1,char* para1val,char* para2,char* para2val,char* para3,char* para3val,char* para4,char* para4val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val,para2,para2val,para3,para3val,para4,para4val);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_three_string_type(char* method_name,char* para1,char* para1val,char* para2,char* para2val,char* para3,char* para3val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val,para2,para2val,para3,para3val);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}

/*****************************************************************************/

RPC_SRV_RESULT ADJsonRpcClient::set_tripple_string_get_single_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_param_name,char* third_param_value,char* fourth_param_name,char*fourth_param_value)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "download_tftp_file", "params": { "srcaddr": "192.168.1.112","srcfile": "MrcSpiImg.bin", "targetpath": "\/tmp\/test.bin" }, "id": 0 }
	//<--{ "jsonrpc": "2.0", "result": { "return": "InProgress", "taskId": "1" }, "id": 0 }
	prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value,third_param_name,third_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	//if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
	//	return RPC_SRV_RESULT_UNKNOWN;//return -1;

	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,fourth_param_name,fourth_param_value)!=0)
		return RPC_SRV_RESULT_UNKNOWN;

	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::set_single_string_triple_int_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,int second_param_value,char* third_param_name,int third_param_value,char* fourth_param_name,int fourth_param_value)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value,third_param_name,third_param_value,fourth_param_name,fourth_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;

	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_single_string_single_int_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,int second_param_value)
{
    char result_string[255];
    prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_double_string_triple_int_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_param_name,int third_param_value,char* fourth_param_name,int fourth_param_value,char* fifth_param_name,int fifth_param_value)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value,third_param_name,third_param_value,fourth_param_name,fourth_param_value,fifth_param_name,fifth_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;

	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
//four int's as argument(needed for sdic:get/set wall geometry command)
RPC_SRV_RESULT ADJsonRpcClient::set_four_int_type(char* method_name,char* first_name,int first_value,char* second_name,int second_value,char* third_name,int third_value,char* fourth_name,int fourth_value)
{
	char result_string[255];
	prepare_json_request_four_int(method_name,req_id++,send_buffer,first_name,first_value,second_name,second_value,third_name,third_value,fourth_name,fourth_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_four_int_type(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,int* para3val,char* para4,int* para4val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_four_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_five_string_type_status_error_info(void *Obj)
{
	char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_five_string_param_statusErrorInfo(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
                                pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
                                pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value,
                                pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_value,
                                pCmdObj->fifth_arg_param_name,pCmdObj->fifth_arg_param_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}


//following two functions are for new-wall-config command(old ones to be deleted)
RPC_SRV_RESULT ADJsonRpcClient::set_four_int_two_string_type(char* method_name,char* first_name,int first_value,char* second_name,int second_value,char* third_name,int third_value,char* fourth_name,int fourth_value,char* fifth_name,char* fifth_value,char* sixth_name,char* sixth_value)
{
	char result_string[255];
	prepare_json_request_four_int_two_string(method_name,req_id++,send_buffer,first_name,first_value,second_name,second_value,third_name,third_value,
						fourth_name,fourth_value,fifth_name,fifth_value,sixth_name,sixth_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_single_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val,para2,para2val);//send input type for setting video-bypass
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_single_string_with_five_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val, char* para3,char* para3val, char* para4,char* para4val,char* para5,char* para5val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val,para2,para2val,para3,para3val,para4,para4val,para5,para5val);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_single_string_with_nine_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val, char* para3,char* para3val, char* para4,char* para4val,char* para5,char* para5val, char* para6,char* para6val, char* para7,char* para7val, char* para8,char* para8val,char* para9,char* para9val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val,para2,para2val,para3,para3val,para4,para4val,para5,para5val,para6,para6val,para7,para7val,para8,para8val,para9,para9val);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_four_int_one_string_with_string_para(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,
									int* para3val,char* para4,int* para4val,char* para5,char* para5val,char* para6,char* para6val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer,para6,para6val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_four_int_one_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val,
							    para5,para5val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_four_int_two_string_with_string_para(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,
									int* para3val,char* para4,int* para4val,char* para5,char* para5val,char* para6,char* para6val,char* para7,char* para7val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer,para7,para7val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_four_int_two_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val,
							    para5,para5val,
							    para6,para6val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_single_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val);//send input type for getting video-bypass
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,para2,para2val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_two_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_two_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_three_string_with_string_para(char* method_name,char* para1,char* para1val,char* para2,char* para2val,char* para3,char* para3val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_three_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_two_int_three_double_one_string_type(char* method_name,char* para1,int* para1val,char* para2,double* para2val,char* para3,char* para3val,char* para4,
                                                        int* para4val,char* para5,double* para5val,char* para6,double* para6val)
{
    char result_string[255];
    //prepare_json_request(method_name,req_id++,send_buffer);
    prepare_json_request(method_name,req_id++,send_buffer);//send input type for getting wall-config
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

    if(find_json_result_and_two_int_three_double_one_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                para1,para1val,
                                para2,para2val,
                                para3,para3val,
                                para4,para4val,
                                para5,para5val,
                                para6,para6val ) != 0)
        return RPC_SRV_RESULT_UNKNOWN;
    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_two_int_two_float_one_string_with_string_para(char* method_name,
									char* para1,char* para1val,
									char* para2,int* para2val,
									char* para3,int* para3val,
                                    char* para4,double* para4val,
                                    char* para5,double* para5val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

    if(find_json_result_and_two_int_two_double_one_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val,
							    para5,para5val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_two_int_two_float_one_string_with_string_para(char* method_name,
									char* para1,char* para1val,
									char* para2,int* para2val,
									char* para3,int* para3val,
                                    char* para4,float* para4val,
                                    char* para5,float* para5val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

    if(find_json_result_and_two_int_two_float_one_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val,
							    para5,para5val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}


//following functions are for CS readout.
RPC_SRV_RESULT ADJsonRpcClient::get_three_int_type_with_integer_para(void *Obj)
{
	char result_string[255];
	CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;
				
    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value);
	ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_three_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,&pCmdObj->first_arg_param_int_value,
                                pCmdObj->second_arg_param_name,&pCmdObj->second_arg_param_int_value,
                                pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_int_value ) != 0)
	return RPC_SRV_RESULT_UNKNOWN;

	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_two_string_eight_int_type_with_three_int_two_string_para(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request_three_int_two_string(pCmdObj->get_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,
                                pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_int_value,
                                pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_int_value,
                                pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_value,
                                pCmdObj->fifth_arg_param_name,pCmdObj->fifth_arg_param_value);
    ClientSocket.send_data(send_buffer);
        ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_two_string_eight_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
                                pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
                                pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_int_value,
                                pCmdObj->fourth_arg_param_name,&pCmdObj->fourth_arg_param_int_value,
                                pCmdObj->fifth_arg_param_name,&pCmdObj->fifth_arg_param_int_value,
                                pCmdObj->sixth_arg_param_name,&pCmdObj->sixth_arg_param_int_value,
                                pCmdObj->seventh_arg_param_name,&pCmdObj->seventh_arg_param_int_value,
                                pCmdObj->eighth_arg_param_name,&pCmdObj->eighth_arg_param_int_value,
                                pCmdObj->ninth_arg_param_name,&pCmdObj->ninth_arg_param_int_value,
                                pCmdObj->tenth_arg_param_name,&pCmdObj->tenth_arg_param_int_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_two_int_type_max_min_brightness(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_two_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                (char*)"max_brightness",&pCmdObj->first_arg_param_int_value,
                                (char*)"min_brightness",&pCmdObj->second_arg_param_int_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_six_int_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_six_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,&pCmdObj->first_arg_param_int_value,
                                pCmdObj->second_arg_param_name,&pCmdObj->second_arg_param_int_value,
                                pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_int_value,
                                pCmdObj->fourth_arg_param_name,&pCmdObj->fourth_arg_param_int_value,
                                pCmdObj->fifth_arg_param_name,&pCmdObj->fifth_arg_param_int_value,
                                pCmdObj->sixth_arg_param_name,&pCmdObj->sixth_arg_param_int_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_seven_int_type_native_params(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_seven_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                (char*)"backlight",&pCmdObj->first_arg_param_int_value,
                                (char*)"rOffset",&pCmdObj->second_arg_param_int_value,
                                (char*)"gOffset",&pCmdObj->third_arg_param_int_value,
                                (char*)"bOffset",&pCmdObj->fourth_arg_param_int_value,
                                (char*)"rGain",&pCmdObj->fifth_arg_param_int_value,
                                (char*)"gGain",&pCmdObj->sixth_arg_param_int_value,
                                (char*)"bGain",&pCmdObj->seventh_arg_param_int_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}


RPC_SRV_RESULT ADJsonRpcClient::get_three_double_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
        ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_three_double_delta_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,&pCmdObj->first_arg_param_double_value,
                                pCmdObj->second_arg_param_name,&pCmdObj->second_arg_param_double_value,
                                pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_double_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_three_double_type_with_string_para(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value);
    ClientSocket.send_data(send_buffer);
        ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_three_double_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,&pCmdObj->first_arg_param_double_value,
                                pCmdObj->second_arg_param_name,&pCmdObj->second_arg_param_double_value,
                                pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_double_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_nine_double_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
        ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_nine_double_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,&pCmdObj->first_arg_param_double_value,
                                pCmdObj->second_arg_param_name,&pCmdObj->second_arg_param_double_value,
                                pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_double_value,
                                pCmdObj->fourth_arg_param_name,&pCmdObj->fourth_arg_param_double_value,
                                pCmdObj->fifth_arg_param_name,&pCmdObj->fifth_arg_param_double_value,
                                pCmdObj->sixth_arg_param_name,&pCmdObj->sixth_arg_param_double_value,
                                pCmdObj->seventh_arg_param_name,&pCmdObj->seventh_arg_param_double_value,
                                pCmdObj->eighth_arg_param_name,&pCmdObj->eighth_arg_param_double_value,
                                pCmdObj->ninth_arg_param_name,&pCmdObj->ninth_arg_param_double_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_three_string_nine_double_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    printf("client2\n");
    prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
        ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result_and_three_string_nine_double_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
                                pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
                                pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value,
                                pCmdObj->fourth_arg_param_name,&pCmdObj->fourth_arg_param_double_value,
                                pCmdObj->fifth_arg_param_name,&pCmdObj->fifth_arg_param_double_value,
                                pCmdObj->sixth_arg_param_name,&pCmdObj->sixth_arg_param_double_value,
                                pCmdObj->seventh_arg_param_name,&pCmdObj->seventh_arg_param_double_value,
                                pCmdObj->eighth_arg_param_name,&pCmdObj->eighth_arg_param_double_value,
                                pCmdObj->ninth_arg_param_name,&pCmdObj->ninth_arg_param_double_value,
                                              pCmdObj->tenth_arg_param_name,&pCmdObj->tenth_arg_param_double_value,
                                              pCmdObj->eleventh_arg_param_name,&pCmdObj->eleventh_arg_param_double_value,
                                              pCmdObj->twelfth_arg_param_name,&pCmdObj->twelfth_arg_param_double_value) != 0)
    return RPC_SRV_RESULT_UNKNOWN;

    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_three_string_nine_double_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request_three_string_nine_double_param(pCmdObj->set_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
                         pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
                         pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_value,
                         pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_double_value,
                         pCmdObj->fifth_arg_param_name,pCmdObj->fifth_arg_param_double_value,
                         pCmdObj->sixth_arg_param_name,pCmdObj->sixth_arg_param_double_value,
                         pCmdObj->seventh_arg_param_name,pCmdObj->seventh_arg_param_double_value,
                         pCmdObj->eighth_arg_param_name,pCmdObj->eighth_arg_param_double_value,
                         pCmdObj->ninth_arg_param_name,pCmdObj->ninth_arg_param_double_value,
                         pCmdObj->tenth_arg_param_name,pCmdObj->tenth_arg_param_double_value,
                        pCmdObj->eleventh_arg_param_name,pCmdObj->eleventh_arg_param_double_value,
                        pCmdObj->twelfth_arg_param_name,pCmdObj->twelfth_arg_param_double_value);
    
    printf("sendBuffer=%s\n",send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;
    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_seven_int_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request_seven_int_param(pCmdObj->set_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,
                         pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_int_value,
                         pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_int_value,
                         pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_int_value,
                         pCmdObj->fifth_arg_param_name,pCmdObj->fifth_arg_param_int_value,
                         pCmdObj->sixth_arg_param_name,pCmdObj->sixth_arg_param_int_value,
                         pCmdObj->seventh_arg_param_name,pCmdObj->seventh_arg_param_int_value);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;
    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_single_int_nine_double_type(void *Obj)
{
    char result_string[255];
    CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;

    prepare_json_request_single_int_nine_double_param(pCmdObj->set_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value,
                         pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_double_value,
                         pCmdObj->third_arg_param_name,pCmdObj->third_arg_param_double_value,
                         pCmdObj->fourth_arg_param_name,pCmdObj->fourth_arg_param_double_value,
                         pCmdObj->fifth_arg_param_name,pCmdObj->fifth_arg_param_double_value,
                         pCmdObj->sixth_arg_param_name,pCmdObj->sixth_arg_param_double_value,
                         pCmdObj->seventh_arg_param_name,pCmdObj->seventh_arg_param_double_value,
                         pCmdObj->eighth_arg_param_name,pCmdObj->eighth_arg_param_double_value,
                         pCmdObj->ninth_arg_param_name,pCmdObj->ninth_arg_param_double_value,
                         pCmdObj->tenth_arg_param_name,pCmdObj->tenth_arg_param_double_value);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),8000);//8sec timeout

    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;
    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_two_int_two_string_type(char* method_name,char* first_name,int first_value,char* second_name,int second_value,char* third_name,char* third_value,char* fourth_name,char* fourth_value)
{
    char result_string[255];
    prepare_json_request_two_int_two_string(method_name,req_id++,send_buffer,first_name,first_value,second_name,second_value,third_name,third_value,
                        fourth_name,fourth_value);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;
    return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::set_three_double_type(char* method_name,char* first_name,double first_value,char* second_name,double second_value,char* third_name,double third_value)
{
    char result_string[255];
    prepare_json_request_three_double(method_name,req_id++,send_buffer,first_name,first_value,second_name,second_value,third_name,third_value);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;
    return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::set_two_int_one_double_one_string_type(char* method_name,char* first_name,int first_value,char* second_name,double second_value,char* third_name,char* third_value,char* fourth_name,int fourth_value)
{
    char result_string[255];
    prepare_json_request_two_int_one_double_one_string(method_name,req_id++,send_buffer,first_name,first_value,second_name,second_value,third_name,third_value,
                        fourth_name,fourth_value);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
    if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
        return RPC_SRV_RESULT_UNKNOWN;//return -1;
    return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_two_int_two_string_type(char* method_name,char* para1,int* para1val,char* para2,int* para2val,char* para3,
                                    char* para3val,char* para4,char* para4val)
{
    char result_string[255];
    //prepare_json_request(method_name,req_id++,send_buffer);
    prepare_json_request(method_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
    if(find_json_result_and_two_int_two_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                para1,para1val,
                                para2,para2val,
                                para3,para3val,
                                para4,para4val) != 0)
        return RPC_SRV_RESULT_UNKNOWN;
    return convert_string_to_server_result(result_string);
}

/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::get_two_string_eight_int_type_with_integer_para(void *Obj)
{
	char result_string[255];
	CmdExecutionObj *pCmdObj = (CmdExecutionObj*)Obj;
				
        prepare_json_request(pCmdObj->get_rpc_name,req_id++,send_buffer,pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_int_value);
	ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),80000);//80sec timeout

	if(find_json_result_and_two_string_eight_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    pCmdObj->first_arg_param_name,pCmdObj->first_arg_param_value,
							    pCmdObj->second_arg_param_name,pCmdObj->second_arg_param_value,
							    pCmdObj->third_arg_param_name,&pCmdObj->third_arg_param_int_value,
							    pCmdObj->fourth_arg_param_name,&pCmdObj->fourth_arg_param_int_value,
							    pCmdObj->fifth_arg_param_name,&pCmdObj->fifth_arg_param_int_value,
							    pCmdObj->sixth_arg_param_name,&pCmdObj->sixth_arg_param_int_value,
							    pCmdObj->seventh_arg_param_name,&pCmdObj->seventh_arg_param_int_value,
							    pCmdObj->eighth_arg_param_name,&pCmdObj->eighth_arg_param_int_value,
							    pCmdObj->ninth_arg_param_name,&pCmdObj->ninth_arg_param_int_value,
							    pCmdObj->tenth_arg_param_name,&pCmdObj->tenth_arg_param_int_value) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
//get net-settings
RPC_SRV_RESULT ADJsonRpcClient::get_three_string_with_one_string_para(char* method_name,char* para1,char* para1val, //send eth0/eth1 string type
									                char* para2,char* para2val, //read addressing-type
                                                                                        char* para3,char* para3val, //read ip addr
                                                                                        char* para4,char* para4val) //read netmask
								                        //char* para5,char* para5val,
                                                                                        //char* para6,char* para6val)
{
	char result_string[255];
	//prepare_json_request(method_name,req_id++,send_buffer);
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_three_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val) != 0)
							    //para4,para4val,
							    //para5,para5val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
//get net-settings
RPC_SRV_RESULT ADJsonRpcClient::get_three_string_type(char* method_name,char* para1,char* para1val,
							                char* para2,char* para2val,
                                                                        char* para3,char* para3val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer);//,para1,para1val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_three_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_four_string_type(char* method_name,char* para1,char* para1val,
							                char* para2,char* para2val,char* para3,char* para3val,
							                char* para4,char* para4val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_four_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
//one string and three int's as argument(needed for sdic:get/set custom background)
RPC_SRV_RESULT ADJsonRpcClient::set_one_string_three_int_type(char* method_name,char* strname,char* strval,char* int1name,int int1val,char* int2name,int int2val,char* int3name,int int3val)
{
	char result_string[255];
	prepare_json_request_one_string_three_int(method_name,req_id++,send_buffer,strname,strval,int1name,int1val,int2name,int2val,int3name,int3val);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_one_string_three_int_type(char* method_name,char* para1,char* para1val,char* para2,int* para2val,char* para3,int* para3val,char* para4,int* para4val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_one_string_three_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para1,para1val,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}

RPC_SRV_RESULT ADJsonRpcClient::get_one_string_one_int_type(char* method_name,char* para1,char* para1val,char* para2,int* para2val)
{
    char result_string[255];
    prepare_json_request(method_name,req_id++,send_buffer);
    ClientSocket.send_data(send_buffer);
    ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
    if(find_json_result_one_string_one_int_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
                                para1,para1val,
                                para2,para2val) != 0)
        return RPC_SRV_RESULT_UNKNOWN;
    return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
//RPC_SRV_RESULT ADJsonRpcClient::get_one_string_three_int_type(char* method_name,char* strname,char* strval,char* int1name,int* int1val,char* int2name,int* int2val,char* int3name,int* int3val)
//{
//	char result_string[255];
//	prepare_json_request(method_name,req_id++,send_buffer);
//	ClientSocket.send_data(send_buffer);
//	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
//		return RPC_SRV_RESULT_UNKNOWN;
//	return convert_string_to_server_result(result_string);
//}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::set_double_string_get_single_string_type(char* method_name,char* first_param_name,char* first_param_value,char* second_param_name,char* second_param_value,char* third_para_name,char* third_para_val)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_fpga_logic","params": {"action": "config","filepath": "/tmp/fpga.bit"} ,"id": 10}
	prepare_json_request(method_name,req_id++,send_buffer,first_param_name,first_param_value,second_param_name,second_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	//if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
	//	return RPC_SRV_RESULT_UNKNOWN;//return -1;
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,third_para_name,third_para_val)!=0)
		return RPC_SRV_RESULT_UNKNOWN;

	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::set_single_string_type(char* method_name,char* method_param_name,char* method_param_value)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_active_source","params": {"source": "pc"} ,"id": 10}
	prepare_json_request(method_name,req_id++,send_buffer,method_param_name,method_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::set_single_string_get_single_string_type(char* method_name,char* method_param_name,char* method_param_value,char* ret_para_name,char* ret_para_val)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_active_source","params": {"source": "pc"} ,"id": 10}
	prepare_json_request(method_name,req_id++,send_buffer,method_param_name,method_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	//if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
	//	return RPC_SRV_RESULT_UNKNOWN;//return -1;
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,ret_para_name,ret_para_val)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::set_single_string_get_single_string_type_with_addr_para(char* method_name,char* addr_para_name,int addr_para_value,char* method_param_name,char* method_param_value,char* ret_para_name,char* ret_para_val)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_active_source","params": {"source": "pc"} ,"id": 10}
	
	prepare_json_request(method_name,req_id++,send_buffer,addr_para_name,addr_para_value,method_param_name,method_param_value);
	//prepare_json_request(method_name,req_id++,send_buffer,method_param_name,method_param_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	//if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
	//	return RPC_SRV_RESULT_UNKNOWN;//return -1;
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,ret_para_name,ret_para_val)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::set_action_noarg_type(char* method_name)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "set_reset_fpga","id": 10}
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
	{
		return RPC_SRV_RESULT_UNKNOWN;
	}
	return convert_string_to_server_result(result_string);
}
//for set actions which gets back taskID(for time-consuming long actions, ex: test-usb,test-emmc,run-script ..etc)
RPC_SRV_RESULT ADJsonRpcClient::set_action_noarg_get_single_string_type(char* method_name,char* ret_para_name,char* ret_para_val)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "test_mmc","id": 10}
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,ret_para_name,ret_para_val)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::get_string_type(char* method_name,char* param_name,char* param_value)
{
	char result_string[255];
	//-->{ "jsonrpc": "2.0", "method": "get_fpga_dev_access", "id": 0 }
	//<--{ "jsonrpc": "2.0", "result": { "return": "Success", "access": "on" }, "id": 0 }
	prepare_json_request(method_name,req_id++,send_buffer);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	sprintf(param_value,"NotFound");//initialize this string as NotFound in the beginning, after parsing result string, it will be filled with correct value.
	if(find_json_result_and_single_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,param_name,param_value)!=0)
		return RPC_SRV_RESULT_UNKNOWN;
	return convert_string_to_server_result(result_string);
}
/*****************************************************************************/
double ADJsonRpcClient::ADJsonRpcClient::get_command_execution_time()
{
	return ClientSocket.get_communication_time_in_ms();
}
/*****************************************************************************/
RPC_SRV_RESULT ADJsonRpcClient::set_three_ulong_type(char* method_name,char* first_name,unsigned long first_value,char* second_name,unsigned long second_value,char* third_name,unsigned long third_value)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,first_name,first_value,second_name,second_value,third_name,third_value);
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout
	if(find_json_result(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string)!=0)
		return RPC_SRV_RESULT_UNKNOWN;//return -1;
	return convert_string_to_server_result(result_string);
}
RPC_SRV_RESULT ADJsonRpcClient::get_three_ulong_type(char* method_name,char* first_name,unsigned long *first_value,char* second_name,unsigned long *second_value,char* third_name,unsigned long *third_value)
{
	RPC_SRV_RESULT tmp_res;
	char str1[255];char str2[255];char str3[255];
	tmp_res = get_three_string_type(method_name,first_name ,str1,second_name,str2,third_name ,str3);
	if(tmp_res==RPC_SRV_RESULT_SUCCESS)
	{
		sscanf(str1,"%lu", first_value);
		sscanf(str2,"%lu", second_value);
		sscanf(str3,"%lu", third_value);
	}
	else
	{
		*first_value=0;*second_value=0;*third_value=0;
	}
	return tmp_res;
}
/*****************************************************************************/
//send_one_argument==>
//<==receive six int + result string
RPC_SRV_RESULT ADJsonRpcClient::get_six_int_one_string_with_string_para(char* method_name,
									char* para1,char* para1val,
									char* para2,int* para2val,
									char* para3,int* para3val,
									char* para4,int* para4val,
									char* para5,int* para5val,
									char* para6,int* para6val,
									char* para7,int* para7val)
{
	char result_string[255];
	prepare_json_request(method_name,req_id++,send_buffer,para1,para1val);//send input type for getting wall-config
	ClientSocket.send_data(send_buffer);
	ClientSocket.receive_data_blocking(recv_buffer,sizeof(recv_buffer),4000);//4sec timeout

    if(find_json_result_and_six_int_one_string_param(recv_buffer,(char*)RPC_NAME_ARG_RESULT_PARAM,result_string,
							    para2,para2val,
							    para3,para3val,
							    para4,para4val,
							    para5,para5val,
							    para6,para6val,
							    para7,para7val) != 0)
		return RPC_SRV_RESULT_UNKNOWN;

	return convert_string_to_server_result(result_string);
}
int ADJsonRpcClient::find_json_result_and_six_int_one_string_param(char *json_string,char* result_name,char* result_value,
							    char* para1,int* para1val,
							    char* para2,int* para2val,
							    char* para3,int* para3val,
							    char* para4,int* para4val,
							    char* para5,int* para5val,
							    char* para6,int* para6val)

{
	struct json_object *new_obj=NULL;
	new_obj = json_tokener_parse(json_string);
	if(new_obj==NULL)
		return -1;

	//first search for "result" string in response object
	struct json_object *new_obj1 = json_object_object_get(new_obj, RPC_NAME_ARG_RESULT_OBJ);
	if(json_object_get_string(new_obj1)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}

	//search for "return" string in result object.
	struct json_object *new_obj2 = json_object_object_get(new_obj1, result_name);
	if(json_object_get_string(new_obj2)==NULL)
	{
		json_object_put(new_obj);
		return -1;
	}
	sprintf(result_value,"%s",json_object_get_string(new_obj2));

	//search for para1(int)
	struct json_object *new_obj3 = json_object_object_get(new_obj1, para1);//para_value is still part of result object
	if(json_object_get_string(new_obj3)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para1val=atoi(json_object_get_string(new_obj3));


	//search for para2(int)
	struct json_object *new_obj4 = json_object_object_get(new_obj1, para2);//para_value is still part of result object
	if(json_object_get_string(new_obj4)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para2val=atoi(json_object_get_string(new_obj4));

	//search for para3(int)
	struct json_object *new_obj5 = json_object_object_get(new_obj1, para3);//para_value is still part of result object
	if(json_object_get_string(new_obj5)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para3val=atoi(json_object_get_string(new_obj5));

	//search for para4(int)
	struct json_object *new_obj6 = json_object_object_get(new_obj1, para4);//para_value is still part of result object
	if(json_object_get_string(new_obj6)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para4val=atoi(json_object_get_string(new_obj6));

	//search for para5(int)
	struct json_object *new_obj7 = json_object_object_get(new_obj1, para5);//para_value is still part of result object
	if(json_object_get_string(new_obj7)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para5val=atoi(json_object_get_string(new_obj7));

	//search for para6(int)
	struct json_object *new_obj8 = json_object_object_get(new_obj1, para6);//para_value is still part of result object
	if(json_object_get_string(new_obj8)==NULL)
	{
		json_object_put(new_obj);
		return 0;//!!! dont return -1 here, atleast result string was found.
	}
	*para6val=atoi(json_object_get_string(new_obj8));

	json_object_put(new_obj);
	return 0;
}
