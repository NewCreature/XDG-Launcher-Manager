#ifndef XLM_LAUNCHER_DATABASE_H
#define XLM_LAUNCHER_DATABASE_H

#define XLM_LAUNCHER_DATABASE_MAX 1024

#include "launcher.h"

typedef struct
{

	char folder[1024];
	XLM_LAUNCHER * launcher[XLM_LAUNCHER_DATABASE_MAX];
	int launcher_count;

} XLM_LAUNCHER_DATABASE;

XLM_LAUNCHER_DATABASE * xlm_create_launcher_database(const char * path);
void xlm_destroy_launcher_database(XLM_LAUNCHER_DATABASE * ldp);

#endif
