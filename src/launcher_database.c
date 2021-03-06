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
	bool ret = true;

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
			}
			else
			{
				ret = false;
			}
			dbp->saved_launcher[dbp->saved_launcher_count] = xlm_load_launcher(fn);
			if(dbp->saved_launcher[dbp->saved_launcher_count])
			{
				dbp->saved_launcher_count++;
			}
			else
			{
				ret = false;
			}
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}

	return ret;
}

static bool _build_launcher_database(XLM_LAUNCHER_DATABASE * ldp)
{
	t3f_scan_files(ldp->folder, process_file, false, ldp);
	return true;
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
	if(!_build_launcher_database(launcher_database))
	{
		goto fail;
	}
	return launcher_database;

	fail:
	{
		xlm_destroy_launcher_database(launcher_database);
		return NULL;
	}
}

static void _clear_launcher_database(XLM_LAUNCHER_DATABASE * ldp)
{
	int i;

	for(i = 0; i < ldp->launcher_count; i++)
	{
		xlm_destroy_launcher(ldp->launcher[i]);
		ldp->launcher_count = 0;
	}
	for(i = 0; i < ldp->saved_launcher_count; i++)
	{
		xlm_destroy_launcher(ldp->saved_launcher[i]);
		ldp->saved_launcher_count = 0;
	}
}

void xlm_destroy_launcher_database(XLM_LAUNCHER_DATABASE * ldp)
{
	if(ldp)
	{
		_clear_launcher_database(ldp);
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
	int i;

	if(ldp->launcher_count < XLM_LAUNCHER_DATABASE_MAX)
	{
		lp = xlm_create_launcher();
		if(lp)
		{
			lp->ini = al_create_config();
			if(!lp->ini)
			{
				goto fail;
			}
			next_path = get_next_path(ldp);
			if(next_path)
			{
				strcpy(lp->path, next_path);
				i = xlm_get_launcher_field_by_key("Type");
				if(i >= 0)
				{
					xlm_set_launcher_field(lp, i, "Application");
				}
				ldp->launcher[ldp->launcher_count] = lp;
				ldp->launcher_count++;
			}
			else
			{
				goto fail;
			}
		}
	}
	return lp;

	fail:
	{
		xlm_destroy_launcher(lp);
	}
	return NULL;
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
		ldp->launcher_count--;
		return true;
	}
	return false;
}

static bool should_delete_launcher(XLM_LAUNCHER_DATABASE * ldp, int i)
{
	int j;

	for(j = 0; j < ldp->launcher_count; j++)
	{
		printf(" c %s\n", ldp->launcher[j]->path);
		if(!strcmp(ldp->launcher[j]->path, ldp->saved_launcher[i]->path))
		{
			return false;
		}
	}
	return true;
}

static void remove_launcher(XLM_LAUNCHER * lp)
{
	al_remove_filename(lp->path);
}

static void remove_deleted_launchers(XLM_LAUNCHER_DATABASE * ldp)
{
	int i;

	for(i = 0; i < ldp->saved_launcher_count; i++)
	{
		if(should_delete_launcher(ldp, i))
		{
			remove_launcher(ldp->saved_launcher[i]);
		}
	}
}

static bool icon_used(XLM_LAUNCHER_DATABASE * ldp, const char * fn)
{
	int icon_field;
	int i;

	icon_field = xlm_get_launcher_field_by_key("Icon");
	if(icon_field >= 0)
	{
		for(i = 0; i < ldp->launcher_count; i++)
		{
			if(!strcmp(ldp->launcher[i]->field[icon_field], fn))
			{
				return true;
			}
		}
	}
	return false;
}

static void remove_unreferenced_icons(XLM_LAUNCHER_DATABASE * ldp)
{
	char buf[1024];
	const char * icon_path;
	int i;

	icon_path = al_path_cstr(t3f_data_path, '/');
	if(icon_path)
	{
		for(i = 0; i < 10000; i++)
		{
			sprintf(buf, "%sxlm_icon_%03d.png", icon_path, i);
			if(al_filename_exists(buf))
			{
				if(!icon_used(ldp, buf))
				{
					al_remove_filename(buf);
				}
			}
			sprintf(buf, "%sxlm_icon_%03d.svg", icon_path, i);
			if(al_filename_exists(buf))
			{
				if(!icon_used(ldp, buf))
				{
					al_remove_filename(buf);
				}
			}
		}
	}
}

bool xlm_save_launcher_database(XLM_LAUNCHER_DATABASE * ldp)
{
	bool ret = true;
	int i;

	for(i = 0; i < ldp->launcher_count; i++)
	{
		if(!xlm_save_launcher(ldp->launcher[i]))
		{
			printf("failed to save launcher %d\n", i);
			ret = false;
		}
	}
	remove_deleted_launchers(ldp);
	remove_unreferenced_icons(ldp);
	_clear_launcher_database(ldp);
	if(!_build_launcher_database(ldp))
	{
		ret = false;
		printf("Could not rebuild launcher database!\n");
	}
	return ret;
}
