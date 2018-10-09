#include "SdapiHttpHandler.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "route.h"
#include <regex>
#include <iostream>
#include <fstream>
using namespace std;
using namespace Onion;
#define JSON_URL_TREE_FILE "/home/adav/tmp/nlohmann/test.json"


/// Same as strcmp, but also checks for NULL values.
bool safe_strcmp(const char *a, const char *b)
{
	if (!a && !b)
		return true;
	if (!a || !b)
		return false;
	return strcmp(a,b)==0;
}
//curl -s --data-binary '{"jsonrpc": "2.0", "method": "strip","params": {"str": "helloadav"}, "id":1}' -H 'content-type: application/json;' http://localhost/sdapi/temps
//curl http://localhost/sdapi/temps

onion_connection_status strip_rpc(void *_, onion_request *req, onion_response *res)
{
	cout<<"onion path     = "<<onion_request_get_path(req)<<endl;
	cout<<"onion fullpath = "<<onion_request_get_fullpath(req)<<endl;

	const onion_block *dreq=onion_request_get_data(req);
	onion_dict *jreq=NULL;
	if (dreq)
		jreq=onion_dict_from_json( onion_block_data( dreq ) );
	if (jreq)
	{ 
		// safe_strcmp(onion_request_get_header(req, "content-type"), "application/json") // Not used in tinyrpc, not really mandatory
		//onion_dict_print_dot(jreq);

		/// Check is the proper call.
		if (!safe_strcmp(onion_dict_get(jreq,"jsonrpc"), "2.0"))
		{
	onion_response_write0(res, "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}");
			return OCS_PROCESSED;
		}
		if (!safe_strcmp(onion_dict_get(jreq,"method"), "strip"))
		{
	onion_response_printf(res, "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Method not found\"}, \"id\": \"%s\"}", 
			onion_dict_get(jreq,"id"));
			return OCS_PROCESSED;
		}
		char *str=onion_low_strdup( onion_dict_rget(jreq,"params", "str", NULL) );
		/// check real start and end. To prepare to write back the stripped string.
		int start=0, end=strlen(str);
		while(start<end && isspace(str[start])) start++;
			end--;
		while(start<end && isspace(str[end])) end--;
			end++;
		str[end]=0;

		/// Prepare message
		onion_dict *jres=onion_dict_new();
		onion_dict_add(jres, "jsonrpc", "2.0", 0);
		onion_dict_add(jres, "result", str+start, OD_DUP_VALUE);
		onion_dict_add(jres, "id", onion_dict_get(jreq, "id"), 0);

		/// Write
		onion_block *jresb=onion_dict_to_json(jres);
		onion_response_write( res,  onion_block_data( jresb ), onion_block_size( jresb ) );

		/// Clean up.
		onion_block_free(jresb);
		onion_dict_free(jres);
		onion_low_free(str);
		onion_dict_free(jreq);
		return OCS_PROCESSED;
	}
	else
	{
		//onion_response_write0(res, "This is a JSON rpc service. Please send jsonrpc requests.\n");
		//return OCS_PROCESSED;

		/// Prepare message
		onion_dict *jres=onion_dict_new();
		onion_dict_add(jres, "jsonrpc", "2.0", 0);
		onion_dict_add(jres, "result", "hello", OD_DUP_VALUE);
		onion_dict_add(jres, "id", "5", 0);
		/// Write
		onion_block *jresb=onion_dict_to_json(jres);
		onion_response_write( res,  onion_block_data( jresb ), onion_block_size( jresb ) );
		return OCS_PROCESSED;
	}
}
/*****************************************************************************/
onion_connection_status sysmgr_handler(void *_, onion_request *req, onion_response *res)
{
	cout<<"sysmgr :onion path     = "<<onion_request_get_path(req)<<endl;
	return OCS_PROCESSED;
}
/*****************************************************************************/
onion_connection_status xmproxysrv_handler(void *_, onion_request *req, onion_response *res)
{
	cout<<"xmproxy:onion path     = "<<onion_request_get_path(req)<<endl;
	return OCS_PROCESSED;
}
/*****************************************************************************/
onion_connection_status gpiosrv_handler(void *_, onion_request *req, onion_response *res)
{
	cout<<"gpiosrv:onion path     = "<<onion_request_get_path(req)<<endl;
	return OCS_PROCESSED;
}
/*****************************************************************************/
SdapiHttpHandler::SdapiHttpHandler(int port):http_port(port)
{
	std::ifstream ifs(JSON_URL_TREE_FILE);//"test.json");
#ifdef CONFIGURABLE_API 
	JsonUrlTree = json::parse(ifs);
#endif
}
/*****************************************************************************/
SdapiHttpHandler::~SdapiHttpHandler()
{
}
/*****************************************************************************/
onion_connection_status SdapiHttpHandler::handle_symgr(Request &req, Response &res)
{
	cout<<"cpp:sysmgr : path = "<<onion_request_get_path(req.c_handler())<<endl;
	Dict data;
	data.add("jsonrpc","2.0");
	data.add("result","xmproxy");
	data.add("id","5");
	res<<data.toJSON();
	return OCS_PROCESSED;
}
onion_connection_status SdapiHttpHandler::handle_gpiosrv(Request &req, Response &res)
{
	if (onion_request_get_flags(req.c_handler())&OR_POST)
	{
		const onion_block *dreq=onion_request_get_data(req.c_handler());
		cout<<onion_block_data( dreq )<<endl;
		onion_response_write( res.c_handler(),  onion_block_data( dreq ), onion_block_size( dreq ) );
	}
	else
	{
		cout<<"GET :gpiosrv: path = "<<onion_request_get_path(req.c_handler())<<endl;
		Dict data;
		data.add("jsonrpc","2.0");
		data.add("result","GET");
		data.add("id","5");
		res<<data.toJSON();
	}
	return OCS_PROCESSED;
}
#include "ADJsonRpcClient.hpp"
onion_connection_status SdapiHttpHandler::handle_request(Request &req, Response &res)
{
	char url[1024];strcpy(url,onion_request_get_path(req.c_handler()));
	char* port=url;//(char*)&url[0];
	if (onion_request_get_flags(req.c_handler())&OR_POST)
	{
		const onion_block *dreq=onion_request_get_data(req.c_handler());
		int rpc_port=atoi(port);
		//cout<<"port="<<atoi(port)<<endl;
		ADJsonRpcClient Client;
		if(Client.rpc_server_connect("127.0.0.1",rpc_port)!=0)
		{
			const char *errmsg="{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Rpc Comm error\"}, \"id\": null}\n";
			onion_response_write(res.c_handler(),errmsg,strlen(errmsg));
			return OCS_PROCESSED;
		}
		char* rpc_resp=Client.send_raw_data_and_receive_resp((char*)onion_block_data(dreq));
		Client.rpc_server_disconnect();
		//cout<<onion_block_data( dreq )<<endl;
		onion_response_write( res.c_handler(),  rpc_resp, strlen(rpc_resp));
	}
	else
	{
		cout<<"GET :gpiosrv: path = "<<url<<endl;//onion_request_get_path(req.c_handler())<<endl;
		Dict data;
		data.add("jsonrpc","2.0");
		data.add("result","GET method called");
		data.add("id","5");
		res<<data.toJSON();
	}
	return OCS_PROCESSED;
}
/*****************************************************************************/
onion_connection_status SdapiHttpHandler::handle_api_request(Request &req, Response &res)
{
#ifdef CONFIGURABLE_API 
	char url[1024];strcpy(url,onion_request_get_path(req.c_handler()));
	//cout<<"SdapiHttpHandler::handle_api_request url="<<url<<endl;
	//std::cout << std::setw(4) << JsonUrlTree.flatten() << '\n';

	//prints the query string for cmd: curl localhost:8080/api/temps?search=testtest
	char query[1024];query[0]='\0';strcpy(query,onion_request_get_query(req.c_handler(),"search"));
	query[1000]='\0';
	cout<<"query = "<<query<<endl;//testtest will be printed

	int r1,r2;
	route::Route myRoute;
	auto match = myRoute.set(url);//"energy/ab");

	//serve top-level url
	int r = match.test(":str/.*");
	if(r!=1) //read tree
	{
		r1 = match.test(":str");
		r2 = match.test(":str/");
		//get on http://ip/api (return top-level flat list)
		if(r1!=1 && r2!=1)
		{
			//if(JsonUrlTree.is_primitive())
			//	;//this is the leaf

			json toplevellist = json::array();
			for (auto it = JsonUrlTree.begin(); it != JsonUrlTree.end(); ++it)
					toplevellist.emplace_back(it.key());
			std::string s = toplevellist.dump();s+='\n';
		
	 		onion_response_write( res.c_handler(), s.c_str(), strlen(s.c_str()));
			return OCS_PROCESSED;

		}
	}


	//level-1
	std::string level1,level2,level3,level4,level5;
	std::string remain,remain1,remain2,remain3,remain4,result;
	bool res1=extract_url_string(url,level1,remain);
	if(res1==false)
	{
		onion_response_write( res.c_handler(), "wrong-url1", strlen("wrong-url1"));
		return OCS_PROCESSED;//wrong url
	}
	if(remain.size()==0)
	{
		if(get_flat_list(JsonUrlTree, level1,result)==true)
		{
			onion_response_write( res.c_handler(), result.c_str(), strlen(result.c_str()));
			return OCS_PROCESSED;
		}
	}

	//level-2
	res1=extract_url_string(remain,level2,remain1);
	if(res1==false)
	{
		onion_response_write( res.c_handler(), "wrong-url2", strlen("wrong-url2"));
		return OCS_PROCESSED;//wrong url
	}
	if(remain1.size()==0)
	{
		//if(JsonUrlTree.count()); //TODO: check if level1 string exists, else new object will be added
		if(get_flat_list(JsonUrlTree[level1], level2,result)==true)
		{
			onion_response_write( res.c_handler(), result.c_str(), strlen(result.c_str()));
			return OCS_PROCESSED;
		}
	}

	//level-3
	res1=extract_url_string(remain1,level3,remain2);
	if(res1==false)
	{
		onion_response_write( res.c_handler(), "wrong-url3", strlen("wrong-url3"));
		return OCS_PROCESSED;//wrong url
	}
	if(remain2.size()==0)
	{
		if(get_flat_list(JsonUrlTree[level1][level2], level3,result)==true)
		{
			onion_response_write( res.c_handler(), result.c_str(), strlen(result.c_str()));
			return OCS_PROCESSED;
		}
	}

	//level-4
	res1=extract_url_string(remain2,level4,remain3);
	if(res1==false)
	{
		onion_response_write( res.c_handler(), "wrong-url4", strlen("wrong-url4"));
		return OCS_PROCESSED;//wrong url
	}
	if(remain3.size()==0)
	{
		if(get_flat_list(JsonUrlTree[level1][level2][level3], level4,result)==true)
		{
			onion_response_write( res.c_handler(), result.c_str(), strlen(result.c_str()));
			return OCS_PROCESSED;
		}
	}

	//level-5
	res1=extract_url_string(remain3,level5,remain4);
	if(res1==false)
	{
		onion_response_write( res.c_handler(), "wrong-url5", strlen("wrong-url5"));
		return OCS_PROCESSED;//wrong url
	}
	if(remain4.size()==0)
	{
		if(get_flat_list(JsonUrlTree[level1][level2][level3][level4], level5,result)==true)
		{
			onion_response_write( res.c_handler(), result.c_str(), strlen(result.c_str()));
			return OCS_PROCESSED;
		}
	}
#endif
	return OCS_PROCESSED;
}
/*****************************************************************************/
#ifdef CONFIGURABLE_API 
bool SdapiHttpHandler::get_flat_list(json topObj, std::string key,std::string &result)
{
	auto test = topObj.find(key);//check if this object exists
	if(test != topObj.end())
	{
		json jobj = topObj[key];
		if(jobj.is_primitive())
		{
			//cout<<"primitive object key="<<key<<endl;//this is the last stage - return value
			result=jobj.dump();result+='\n';

			//result as json-key-value-pair
			json j;j[key]=result;
			result=j.dump();result+='\n';
			return true;
		}
		if(jobj.is_object())
		{
			json toplevellist = json::array();
			for (auto it = jobj.begin(); it != jobj.end(); ++it)
				toplevellist.emplace_back(it.key());
			result = toplevellist.dump();result+='\n';

			//result as json-key-value-pair
			json j;j[key]=result;
			result=j.dump();result+='\n';

			return true;
	 		//onion_response_write( res.c_handler(), s.c_str(), strlen(s.c_str()));
			//return OCS_PROCESSED;
		}	
	}
	//else
		//cout<<"SdapiHttpHandler::get_flat_list:leaf-node"<<endl;
	return false;
}
#endif
/*****************************************************************************/
bool SdapiHttpHandler::extract_url_string(std::string url,std::string &result,std::string &remaining)
{
#ifdef CONFIGURABLE_API 
	//cout<<"SdapiHttpHandler::extract_url_string:url="<<url<<endl;
	route::Route myRoute;
	auto match = myRoute.set(url);
	
	int r = match.test(":str");
	if(r==1)
	{
		result = match.get("str");
		//cout<<"SdapiHttpHandler::extract_url_string:str="<<result<<endl;
		return true;
	}

	r = match.test(":str/");
	if(r==1)
	{
		result = match.get("str");
		return true;
	}

	r = match.test(":str/.*");
	if(r==1)
	{
		result = match.get("str");
		std::string t = url;
		std::string::size_type i = t.find(result);
		if (i != std::string::npos)
		{
			t.erase(i, result.length()+1);//+1 is for removing last char '/'
			remaining=t;
			//cout<<"SdapiHttpHandler::extract_url_string:res1="<<result<<endl;
		}
		return true;
	}
#endif
	return false;
}
/*****************************************************************************/
int SdapiHttpHandler::StartServer()
{
/*	onion *o=onion_new(O_THREADED);
	onion_url *url=onion_root_url(o);
	onion_url_add(url, "^rpcapi/sysmgr", (void*)sysmgr_handler);
	onion_url_add(url, "^rpcapi/xmproxysrv", (void*)xmproxysrv_handler);
	onion_url_add(url, "^rpcapi/gpiosrv", (void*)gpiosrv_handler);
	char port[512];sprintf(port,"%d",http_port);
	onion_set_port(o, port);
	//CFLOG_PANEL_INFO << "sdapi::Listening on http port ="<< http_port<<CFLOG_ENDL;
	//onion_log_flags|=OF_NOINFO;//disables log output prints (logFlags: OF_INIT/OF_NOCOLOR/OF_SYSLOGINIT/OF_NOINFO/OF_NODEBUG)
	//log levels = O_DEBUG0/O_DEBUG/O_INFO/O_WARNING/O_ERROR
	//onion_log=onion_log_stderr; //re-directs the log to stderr
	//onion_log=onion_log_syslog; //re-directs the log to syslog
	onion_listen(o);*/


	onion_log_flags|=OF_NOINFO;
	Onion::Onion server(O_THREADED);
	Onion::Url root(&server);
	root.add("^rpcapi/" ,this, &SdapiHttpHandler::handle_request);
	root.add("^api/" ,this, &SdapiHttpHandler::handle_api_request);

	server.setPort(http_port);
	server.listen();
	return 0;
}
/*****************************************************************************/

