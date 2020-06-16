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

static bool path_used(XLM_LAUNCHER_DATABASE * ldp, const char * path)
{
	int i;

	if(al_filename_exists(path))
	{
		printf("found %s\n", path);
		return true;
	}
	for(i = 0; i < ldp->launcher_count; i++)
	{
		if(!strcmp(ldp->launcher[i]->path, path))
		{
			printf("used %s\n", path);
			return true;
		}
	}
	return false;
}

static char _xlm_next_launcher_path[1024] = {0};

static const char * get_next_path(XLM_LAUNCHER_DATABASE * ldp)
{
	char buf[2048];
	int i;

	for(i = 0; i < XLM_LAUNCHER_DATABASE_MAX; i++)
	{
		sprintf(buf, "%sxlm_launcher_%03d.desktop", ldp->folder, i);
		if(!path_used(ldp, buf))
		{
			strcpy(_xlm_next_launcher_path, buf);
			printf("new path %s\n", _xlm_next_launcher_path);
			return _xlm_next_launcher_path;
		}
	}
	return NULL;
}

XLM_LAUNCHER * xlm_add_launcher_to_database(XLM_LAUNCHER_DATABASE * ldp)
{
	XLM_LAUNCHER * lp = NULL;
	const char * next_path;

	if(ldp->launcher_count < XLM_LAUNCHER_DATABASE_MAX)
	{
		lp = xlm_create_launcher();
		if(lp)
		{
			next_path = get_next_path(ldp);
			if(next_path)
			{
				strcpy(lp->path, next_path);
				strcpy(lp->field[XLM_LAUNCHER_FIELD_NAME], "Unnamed");
				ldp->launcher[ldp->launcher_count] = lp;
				ldp->launcher_count++;
			}
			else
			{
				xlm_destroy_launcher(lp);
			}
		}
	}
	return lp;
}

bool xlm_delete_launcher_from_database(XLM_LAUNCHER_DATABASE * ldp, int i)
{
	int j;

	if(i >= 0 && i < ldp->launcher_count)
	{
		xlm_destroy_launcher(ldp->launcher[i]);
		for(j = i; j < ldp->launcher_count - 1; j++)
		{
			ldp->launcher[j] = ldp->launcher[j + 1];
		}
		return true;
	}
	return false;
}
