#include "SdapiHttpHandler.h"
using namespace std;
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
using namespace Onion;

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
	//root.add("^rpcapi/sysmgr", "Some static text", HTTP_OK );
	//root.add("", "Some static text", HTTP_OK );
	//root.add("lambda", [](Onion::Request &req, Onion::Response &res){
	//	res<<"Lambda handler";
	//	return OCS_PROCESSED;
	//});

	//root.add("^rpcapi/sysmgr" ,this, &SdapiHttpHandler::handle_symgr);
	//root.add("^rpcapi/gpiosrv",this, &SdapiHttpHandler::handle_gpiosrv);
	root.add("^rpcapi/" ,this, &SdapiHttpHandler::handle_request);

	server.setPort(http_port);
	server.listen();
	return 0;
}
/*****************************************************************************/
