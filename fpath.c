#include "fpath.h"

static unsigned int _mkdir(info * _info, const char *dir, size_t len) {
	char *	p = NULL;
	snprintf(_info->buff, len + 1,"%s",dir);
	if(_info->buff[len - 1] == '/')
		_info->buff[len - 1] = 0;
	for(p = _info->buff + 1; *p; p++)
		if(*p == '/') {
			*p = 0;
			struct stat _stat = {0};
			if (stat((char*)_info->buff, &_stat) < 0){
				if(mkdir(_info->buff, S_IRWXU)){
					perror((char*)_info->buff);
					return 1;
				}
			}
			*p = '/';
		}

	if(mkdir(_info->buff, S_IRWXU)){		perror((char*)_info->buff);
		return 1;
	}
	return 0;
}

size_t GetPath(info * _info){
	struct stat _stat = {0};

	int len_path	= strlen((char*)_info->path);
	_info->buff		= (char*)malloc(sizeof(char) * (len_path + 2));

	if (stat((char*)_info->path, &_stat) < 0)
			if(_mkdir(_info, ( char * ) _info->path, len_path)){
				cleanup(_info);
				exit(EXIT_FAILURE);
			}

	sprintf(_info->buff, "%s", _info->path);

	if (_info->buff[len_path - 1]	!=	'/'){
		_info->buff[len_path]		=	'/';
		_info->buff[len_path + 1]	=	0;
		return (len_path + 1);
	}
	else return len_path;
}
