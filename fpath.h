#ifndef __FPATH_H__
#define __FPATH_H__

	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>

	#include "info.h"

	size_t	GetPath(info * _info);

#endif