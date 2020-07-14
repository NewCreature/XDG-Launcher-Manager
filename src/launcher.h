#ifndef XLM_LAUNCHER_H
#define XLM_LAUNCHER_H

#include "t3f/t3f.h"

#define XLM_LAUNCHER_MAX_FIELDS                16
#define XLM_LAUNCHER_MAX_FIELD_SIZE          1024

typedef struct
{

	/* path to the launcher file */
	char path[1024];

	ALLEGRO_CONFIG * ini;
	char * field[XLM_LAUNCHER_MAX_FIELDS];
  ALLEGRO_BITMAP * icon;
	bool hidden;

} XLM_LAUNCHER;

XLM_LAUNCHER * xlm_create_launcher(void);
void xlm_destroy_launcher(XLM_LAUNCHER * lp);

XLM_LAUNCHER * xlm_load_launcher(const char * fn);
bool xlm_save_launcher(XLM_LAUNCHER * lp);

const char * xlm_get_launcher_field_key(int field);
const char * xlm_get_launcher_field_name(int field);
int xlm_get_launcher_field_by_key(const char * name);
int xlm_get_launcher_field_by_name(const char * name);
bool xlm_set_launcher_field(XLM_LAUNCHER * lp, int i, const char * val);
bool xlm_set_launcher_icon(XLM_LAUNCHER * lp, const char * fn);

#endif
