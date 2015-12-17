#ifndef __SDAPI_HTTP_HANDLER_H_
#define __SDAPI_HTTP_HANDLER_H_
//#include "ADCmdlineHelper.hpp"
//#include "ADCmnStringProcessor.hpp"

class SdapiHttpHandler //:public ADCmdlineHelperConsumer ,public ADCmnStringProcessor
{
	int http_port;
public:
	SdapiHttpHandler(int portnum);
	~SdapiHttpHandler();
	int StartServer();

};
#endif
