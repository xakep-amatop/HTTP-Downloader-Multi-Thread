#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__

	#include <curl/curl.h>
	#include "getXmlFields.h"

	void 	is_root_configuration	(info	* _info);
	void 	ReadConfiguration		(info	* _info);
	void 	GetSaveData				(const char * user_agent, const char * path, const char * url);

#endif