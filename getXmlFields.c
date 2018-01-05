#include "getXmlFields.h"

void freeXmlKeys(xmlChar ** keys, size_t max_count){
	if (keys){
		for(size_t i = 0; i < max_count; ++i)
			if(keys[i])
				xmlFree(keys[i]);
		free(keys);
		keys = NULL;
	}
}

xmlChar * xmlStrConcat(xmlChar * src, xmlChar * dst){
	xmlChar * str = (xmlChar * )src;
	while(*str++ != (const xmlChar)'\0');
	--str;
	while ((*str++ = *dst++)!= (const xmlChar)'\0');
	return src;
}

void parse_UserAgent(info * _info){
	xmlNodePtr cur		= _info->cur->xmlChildrenNode;
	xmlChar **	keys	= NULL;

	const xmlChar * fields[] = {
		(const xmlChar *) "product",
		(const xmlChar *) "product-version",
		(const xmlChar *) "comment",
	};

	size_t count_field	= 0;
	size_t number_chars	= 0;
	keys = (xmlChar **)calloc(sizeof(fields)/sizeof(const xmlChar*), sizeof(const xmlChar*));
	while (cur != NULL) {
		for(size_t i = 0; i < sizeof(fields)/sizeof(const xmlChar*); ++i)
			if (xmlStrEqual(cur->name, fields[i]) && !keys[i]){
				keys[i] = xmlNodeListGetString(_info->doc, cur->xmlChildrenNode, 1);
				if (keys[i]){
					++count_field;
					number_chars += strlen((char*)keys[i]);
				}
				break;
			}
		cur = cur->next;
	}

	{
		xmlChar * separators[] = {
			(xmlChar * ) "/",
			(xmlChar * ) " ",
			(xmlChar * ) "\0"
		};

		if (count_field < sizeof(fields) / sizeof(xmlChar * ))
			fprintf(stderr, "Error!!! Not all user-agent fields have been filled in!\n");
		else{
			size_t max_sep_len = 0;
			for(size_t i = 0; i < sizeof(separators) / sizeof(xmlChar * ); ++i)
				max_sep_len += strlen((char*) separators[i]) + 1;

			_info->user_agent		=	(xmlChar*)malloc((number_chars + 1 + max_sep_len)* sizeof(xmlChar));
			*(_info->user_agent)	=	(xmlChar)'\0';
			for(size_t i = 0; i < sizeof(fields) / sizeof(xmlChar * ); ++i){
				strcat((char *)_info->user_agent, (const char * ) keys[i]);
				strcat((char *)_info->user_agent, (const char * ) separators[i]);
			}
		}
	}
	freeXmlKeys(keys, count_field);
}

void parse_URLS(info * _info){
	xmlNodePtr _node = _info->cur->xmlChildrenNode;
	while(_node != NULL){
		if (xmlStrEqual(_node->name, (const xmlChar*)"url")){
			xmlChar * url = xmlNodeListGetString(_info->doc, _node->xmlChildrenNode, 1);
			if (url) ++(_info->num_urls);
			xmlFree(url);
		}
		_node	=	_node->next;
	}
	_info->urls		=	(xmlChar**)malloc(sizeof(xmlChar * ) * _info->num_urls);
	xmlNodePtr cur	=	_info->cur->xmlChildrenNode;
	size_t	i	=	0;

	while (cur != NULL) {
		if (xmlStrEqual(cur->name, (const xmlChar*)"url")) {
			_info->urls[i] = xmlNodeListGetString(_info->doc, cur->xmlChildrenNode, 1);
			if (_info->urls[i])
				++i;
		}
		cur = cur->next;
	}
}

void parse_DirPath(info * _info){
    _info->path	= xmlNodeListGetString(_info->doc, _info->cur->xmlChildrenNode, 1);
}

void parse_NumThreads(info * _info){
	xmlChar * keys =  _info->path	= xmlNodeListGetString(_info->doc, _info->cur->xmlChildrenNode, 1);;

	if (keys){
		_info->num_threads = atoi((char * )keys);
	}
	else{
		fprintf(stderr, "Number threads was not type in configuration file!\n");
	}

	if (!_info->num_threads){
		fprintf(stderr, "Error!!! Not valid data in field user-agent in configuration file!\n");
		_info->num_threads = sysconf(_SC_NPROCESSORS_ONLN);
		if (_info->num_threads == -1){
			perror("Error!!! Can not get number cores!");
			_info->num_threads = 1;
		}
	}

	fprintf(stderr, "Default number threads set to: %i\n", _info->num_threads);
}
