#include "launcher.h"

static const char * _xlm_launcher_field_key[XLM_LAUNCHER_MAX_FIELDS] =
{
	"Name",
	"GenericName",
	"Comment",
	"Exec",
	"Path",
	"Categories",
	NULL
};

static const char * _xlm_launcher_field_name[XLM_LAUNCHER_MAX_FIELDS] =
{
	"Name",
	"Description",
	"Comment",
	"Command",
	"Working Directory",
	"Categories",
	NULL
};

XLM_LAUNCHER * xlm_create_launcher(void)
{
	XLM_LAUNCHER * lp;
	int i;

	lp = malloc(sizeof(XLM_LAUNCHER));
	if(!lp)
	{
		goto fail;
	}
	memset(lp, 0, sizeof(XLM_LAUNCHER));
	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		lp->field[i] = malloc(XLM_LAUNCHER_MAX_FIELD_SIZE);
		if(!lp->field[i])
		{
			goto fail;
		}
		memset(lp->field[i], 0, XLM_LAUNCHER_MAX_FIELD_SIZE);
	}
	return lp;

	fail:
	{
		xlm_destroy_launcher(lp);
		return NULL;
	}
}

void xlm_destroy_launcher(XLM_LAUNCHER * lp)
{
	int i;

	if(lp)
	{
		if(lp->icon)
		{
			al_destroy_bitmap(lp->icon);
		}
		for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
		{
			if(lp->field[i])
			{
				free(lp->field[i]);
			}
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
	const char * val;
	int i;

	lp = xlm_create_launcher();
	if(!lp)
	{
		goto fail;
	}

	strcpy(lp->path, fn);
	lp->ini = al_load_config_file(lp->path);
	if(!lp->ini)
	{
		goto fail;
	}
	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		if(xlm_get_launcher_field_key(i))
		{
			val = al_get_config_value(lp->ini, "Desktop Entry", xlm_get_launcher_field_key(i));
			if(val)
			{
				if(strlen(val) < XLM_LAUNCHER_MAX_FIELD_SIZE)
				{
					strcpy(lp->field[i], val);
				}
				else
				{
					goto fail;
				}
			}
		}
	}

	return lp;

	fail:
	{
		xlm_destroy_launcher(lp);
		return NULL;
	}
}

bool xlm_save_launcher(XLM_LAUNCHER * lp)
{
	int i;

	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		if(lp->field[i] && xlm_get_launcher_field_key(i))
		{
			al_set_config_value(lp->ini, "Desktop Entry", xlm_get_launcher_field_key(i), lp->field[i]);
		}
	}
	al_save_config_file(lp->path, lp->ini);
	return true;
}

const char * xlm_get_launcher_field_key(int field)
{
	return _xlm_launcher_field_key[field];
}

const char * xlm_get_launcher_field_name(int field)
{
	return _xlm_launcher_field_name[field];
}

bool xlm_set_launcher_field(XLM_LAUNCHER * lp, int i, const char * val)
{
	if(lp->field[i] && strlen(val) < XLM_LAUNCHER_MAX_FIELD_SIZE)
	{
		strcpy(lp->field[i], val);
		return true;
	}
	return false;
}

bool xlm_set_launcher_icon(XLM_LAUNCHER * lp, ALLEGRO_BITMAP * icon)
{
	lp->icon = icon;
	return true;
}
