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

class SdapiHttpHandler //:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	int http_port;
public:
	SdapiHttpHandler(int portnum);
	~SdapiHttpHandler();
	int StartServer();
	onion_connection_status handle_symgr(Onion::Request &req, Onion::Response &res);
	onion_connection_status handle_gpiosrv(Onion::Request &req, Onion::Response &res);
	onion_connection_status handle_request(Onion::Request &req, Onion::Response &res);
};
#endif
