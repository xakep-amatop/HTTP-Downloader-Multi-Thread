#include "info.h"
#include "download.h"
#include "fpath.h"
#include "getXmlFields.h"

void PrintHelp(char * name, FILE * _file);

void * t_func(void * _data){
	while(1){
		t_data * data = (t_data * )_data;

		pthread_mutex_lock(& data->mutex);

		if (data->i){
			--(data->i);
		}else{
			pthread_mutex_unlock(& data->mutex);
			break;
		}
		size_t i = data->i;

		pthread_mutex_unlock(& data->mutex);


		char * file_name	= strrchr((char *)data->urls[i], '/');
		char * url			= NULL;

		if (file_name){
			size_t len_url	= strlen((char*)data->urls[i]);
			url				= (char*)malloc(++len_url * sizeof(char));

			memcpy(url, data->urls[i], len_url);
			char * start = NULL;
			while (file_name != start && !*(file_name+1)){
				*file_name		= '\0';
				start			= strchr ((char *)url, '/');
				file_name		= strrchr((char *)url, '/');
			}
		}

		char * name			= (file_name ? ++file_name : (char *) data->urls[i]);
		char * tmp_buff		= NULL;

		if (!data->len_path){
			tmp_buff	= (char * )malloc(sizeof(char) * (strlen(name) + 1));
		}
		else{
			size_t buff_len	= strlen(data->buff);
			size_t full_len	= buff_len + strlen(name) + 1;
			tmp_buff	= (char*)malloc(sizeof(char) * full_len);
			strncpy(tmp_buff, data->buff, buff_len);
		}

		strncpy(tmp_buff + data->len_path, name, strlen(name)+1);

		GetSaveData(data->user_agent, tmp_buff, data->urls[i]);

		free(url);
		free(tmp_buff);
	}

	return NULL;
}

int main(int argc, char * argv[]){
	if(argc < 2){
		fprintf(stderr, "Error!!! Pass the XML configuration file as an argument to the program!\n");
		PrintHelp(argv[0], stderr);
		return EXIT_FAILURE;
	}
	else{
		info _info = {0};

		_info.doc = xmlParseFile(argv[1]);
		if (_info.doc == NULL ) {
			fprintf(stderr,"Document not parsed successfully. \n");
			cleanup(& _info);
			return EXIT_FAILURE;
		}

		_info.cur = xmlDocGetRootElement(_info.doc);
		if (_info.cur == NULL) {
			fprintf(stderr,"empty document\n");
			cleanup(& _info);
			return EXIT_FAILURE;
		}

		is_root_configuration(& _info);

		ReadConfiguration(& _info);

		if (_info.urls && _info.user_agent){
			size_t	len_path	= 0;

			if(_info.path){
				len_path	= GetPath(&_info);
			}

			_info.pt 		= (pthread_t *	)malloc(sizeof(pthread_t) * _info.num_threads);
			t_data _data = {0};

			_data.i				= _info.num_urls;
			_data.user_agent	= (char *)	_info.user_agent;
			_data.urls			= (char **)	_info.urls;
			_data.buff			= (char *)	_info.buff;
			_data.len_path		= len_path;

			pthread_mutex_init(& _data.mutex, NULL);

			for (size_t i = 0; i < _info.num_threads; ++i){
				if (pthread_create(& _info.pt[i], NULL, t_func, (void *) & _data)){
					fprintf(stderr, "Error!!! Can not create thread #%u!\n", i);
				}
			}

			for (size_t i = 0; i < _info.num_threads; ++i){
				pthread_join(_info.pt[i], NULL);
			}

			pthread_mutex_destroy(& _data.mutex);
		}
		cleanup(& _info);
	}
	return EXIT_SUCCESS;
}

void PrintHelp(char * name, FILE * _file){
	char * _n = strrchr(name, '/');
	fprintf(_file,
		"\tUsage: %s <name xml configuration file>\n\n",
		_n ? ++_n : name
	);
}

void cleanup(info * _info){
	freeXmlKeys(_info->urls, _info->num_urls);
	if (_info->user_agent)	free(_info->user_agent);
	if (_info->buff)		free(_info->buff);
	if (_info->path)		xmlFree(_info->path);
	if (_info->doc)			xmlFreeDoc(_info->doc);
	if (_info->pt)			free(_info->pt);

	xmlCleanupParser();
}
