#ifndef XLM_LAUNCHER_H
#define XLM_LAUNCHER_H

#include "t3f/t3f.h"

typedef struct
{

	ALLEGRO_CONFIG * ini;
	char name[1024];
	char type[1024];
	char comment[1024];
	char command[1024];
	char working_directory[1024];
	char categories[1024];
  ALLEGRO_BITMAP * icon;

} XLM_LAUNCHER;

XLM_LAUNCHER * xlm_create_launcher(void);
void xlm_destroy_launcher(XLM_LAUNCHER * lp);

XLM_LAUNCHER * xlm_load_launcher(const char * fn);
bool xlm_save_launcher(XLM_LAUNCHER * lp, const char * fn);

bool xlm_set_launcher_name(XLM_LAUNCHER * lp, const char * name);
bool xlm_set_launcher_command(XLM_LAUNCHER * lp, const char * command);
bool xlm_set_launcher_working_directory(XLM_LAUNCHER * lp, const char * working_directory);
bool xlm_set_launcher_icon(XLM_LAUNCHER * lp, ALLEGRO_BITMAP * icon);

#endif
