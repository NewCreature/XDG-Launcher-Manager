#include "launcher.h"

XLM_LAUNCHER * xlm_create_launcher(void)
{
	XLM_LAUNCHER * lp;

	lp = malloc(sizeof(XLM_LAUNCHER));
	if(!lp)
	{
		goto fail;
	}
	memset(lp, 0, sizeof(XLM_LAUNCHER));

	fail:
	{
		xlm_destroy_launcher(lp);
		return NULL;
	}
}

void xlm_destroy_launcher(XLM_LAUNCHER * lp)
{
	if(lp)
	{
		if(lp->icon)
		{
			al_destroy_bitmap(lp->icon);
		}
		if(lp->ini)
		{
			al_destroy_config(lp->ini);
		}
		free(lp);
	}
}

XLM_LAUNCHER * xlm_load_launcher(const char * fn)
{
	XLM_LAUNCHER * lp;

	lp = xlm_create_launcher();
	if(!lp)
	{
		goto fail;
	}
	memset(lp, 0, sizeof(XLM_LAUNCHER));
	return lp;

	fail:
	{
		xlm_destroy_launcher(lp);
		return NULL;
	}
}

bool xlm_save_launcher(XLM_LAUNCHER * lp, const char * fn)
{
	return false;
}

bool xlm_set_launcher_name(XLM_LAUNCHER * lp, const char * name)
{
	strcpy(lp->name, name);
	return true;
}

bool xlm_set_launcher_command(XLM_LAUNCHER * lp, const char * command)
{
	strcpy(lp->command, command);
	return true;
}

bool xlm_set_launcher_working_directory(XLM_LAUNCHER * lp, const char * working_directory)
{
	strcpy(lp->working_directory, working_directory);
	return true;
}

bool xlm_set_launcher_icon(XLM_LAUNCHER * lp, ALLEGRO_BITMAP * icon)
{
	lp->icon = icon;
	return true;
}
