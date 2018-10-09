#ifndef __SDAPI_HTTP_HANDLER_H_
#define __SDAPI_HTTP_HANDLER_H_
//#include "ADCmdlineHelper.hpp"
//#include "ADCmnStringProcessor.hpp"
#include <onion/onion.h>
#include <onion/dict.h>
#include <onion/block.h>
#include <onion/request.h>
#include <onion/response.h>
#include <onion/url.h>
#include <onion/low.h>
#include <onion/log.h>
#include "ADCommon.hpp"
#include <iostream>
#include <onion/onion.hpp>
#include <onion/response.hpp>
#include <onion/request.hpp>
#include <onion/url.hpp>
#include <onion/dict.hpp>

//#define CONFIGURABLE_API 

#ifdef CONFIGURABLE_API 
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif

class SdapiHttpHandler //:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
#ifdef CONFIGURABLE_API 
	json JsonUrlTree;
	bool get_flat_list(json topObj, std::string key,std::string &result);
#endif
	int http_port;
	bool extract_url_string(std::string url,std::string &result,std::string &remaining);
public:
	SdapiHttpHandler(int portnum);
	~SdapiHttpHandler();
	int StartServer();
	onion_connection_status handle_symgr(Onion::Request &req, Onion::Response &res);
	onion_connection_status handle_gpiosrv(Onion::Request &req, Onion::Response &res);
	onion_connection_status handle_request(Onion::Request &req, Onion::Response &res);
	onion_connection_status handle_api_request(Onion::Request &req, Onion::Response &res);
	//onion_connection_status handle_api1_request(Onion::Request &req, Onion::Response &res);

};
#endif
