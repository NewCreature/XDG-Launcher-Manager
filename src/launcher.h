#ifndef XLM_LAUNCHER_H
#define XLM_LAUNCHER_H

#include "t3f/t3f.h"

#define XLM_LAUNCHER_MAX_FIELDS                16
#define XLM_LAUNCHER_MAX_FIELD_SIZE          1024

#define XLM_LAUNCHER_FIELD_NAME                 0
#define XLM_LAUNCHER_FIELD_TYPE                 1
#define XLM_LAUNCHER_FIELD_COMMENT              2
#define XLM_LAUNCHER_FIELD_COMMAND              3
#define XLM_LAUNCHER_FIELD_WORKING_DIRECTORY    4
#define XLM_LAUNCHER_FIELD_CATEGORIES           5

typedef struct
{

	/* path to the launcher file */
	char path[1024];

	ALLEGRO_CONFIG * ini;
	char * field[XLM_LAUNCHER_MAX_FIELDS];
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
