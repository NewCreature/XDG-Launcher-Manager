#include "launcher_database.h"

XLM_LAUNCHER_DATABASE * xlm_create_launcher_database(const char * path)
{
	const char * home_path;
	XLM_LAUNCHER_DATABASE * launcher_database;

	launcher_database = malloc(sizeof(XLM_LAUNCHER_DATABASE));
	if(!launcher_database)
	{
		goto fail;
	}
	memset(launcher_database, 0, sizeof(XLM_LAUNCHER_DATABASE));
	home_path = getenv("HOME");
	if(!home_path)
	{
		printf("Unable to determine home directory.\n");
		return false;
	}
	sprintf(launcher_database->folder, "%s/.local/share/applications/", home_path);

	return launcher_database;

	fail:
	{
		xlm_destroy_launcher_database(launcher_database);
		return NULL;
	}
}

void xlm_destroy_launcher_database(XLM_LAUNCHER_DATABASE * ldp)
{
	int i;

	if(ldp)
	{
		for(i = 0; i < ldp->launcher_count; i++)
		{
			xlm_destroy_launcher(ldp->launcher[i]);
		}
		free(ldp);
	}
}
