#include "t3f/t3f.h"
#include "t3f/file_utils.h"
#include "launcher_database.h"

static const char * get_extension(const char * fn)
{
	int i;

	for(i = strlen(fn) - 1; i >= 0; i--)
	{
		if(fn[i] == '.')
		{
			return &fn[i];
		}
	}
	return NULL;
}

static bool process_file(const char * fn, bool isfolder, void * data)
{
	XLM_LAUNCHER_DATABASE * dbp = (XLM_LAUNCHER_DATABASE *)data;
	const char * extension;

	if(!isfolder)
	{
		extension = get_extension(fn);
		if(extension && !strcmp(extension, ".desktop"))
		{
			printf("add launcher: %s\n", fn);
			dbp->launcher[dbp->launcher_count] = xlm_load_launcher(fn);
			if(dbp->launcher[dbp->launcher_count])
			{
				dbp->launcher_count++;
				return true;
			}
		}
	}

	return false;
}

XLM_LAUNCHER_DATABASE * xlm_create_launcher_database(const char * home_path)
{
	XLM_LAUNCHER_DATABASE * launcher_database;

	launcher_database = malloc(sizeof(XLM_LAUNCHER_DATABASE));
	if(!launcher_database)
	{
		goto fail;
	}
	memset(launcher_database, 0, sizeof(XLM_LAUNCHER_DATABASE));
	sprintf(launcher_database->folder, "%s/.local/share/applications/", home_path);
	t3f_scan_files(launcher_database->folder, process_file, false, launcher_database);

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
