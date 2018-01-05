#ifndef __STDAFX_H__
#define __STDAFX_H__

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>

	#include <pthread.h>

	#include <libxml/xmlmemory.h>
	#include <libxml/parser.h>

	typedef struct{
		char *			buff;
		char **			urls;
		char *			user_agent;
		size_t			i;
		size_t			len_path;
		pthread_mutex_t mutex;
	} t_data;

	typedef struct{
		xmlChar **	urls;
		xmlChar *	path;
		xmlChar *	user_agent;

		char *		buff;

		xmlDocPtr	doc;
		xmlNodePtr	cur;

		size_t		num_urls;
		int			num_threads;

		char *		url;

		pthread_t *	pt;
	} info;

	void cleanup(info * _info);


#endif