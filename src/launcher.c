#include "launcher.h"

static const char * _xlm_launcher_field_key[XLM_LAUNCHER_MAX_FIELDS] =
{
	"Name",
	"GenericName",
	"Comment",
	"Exec",
	"Path",
	"Categories",
	"Type",
	"Icon",
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
	NULL,
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
	int icon_field;
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
	icon_field = xlm_get_launcher_field_by_key("Icon");
	if(icon_field >= 0)
	{
		if(strlen(lp->field[icon_field]))
		{
			lp->icon = al_load_bitmap(lp->field[icon_field]);
			if(!lp->icon)
			{
				printf("Failed to load icon!\n");
			}
		}
		if(!lp->icon)
		{
			lp->icon = al_load_bitmap("data/bitmaps/default_icon.png");
			if(!lp->icon)
			{
				printf("Failed to load default icon!\n");
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
			if(strlen(lp->field[i]) > 0)
			{
				al_set_config_value(lp->ini, "Desktop Entry", xlm_get_launcher_field_key(i), lp->field[i]);
			}
			else
			{
				al_remove_config_key(lp->ini, "Desktop Entry", xlm_get_launcher_field_key(i));
			}
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

int xlm_get_launcher_field_by_key(const char * name)
{
	int i;

	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		if(_xlm_launcher_field_key[i])
		{
			if(!strcmp(name, _xlm_launcher_field_key[i]))
			{
				return i;
			}
		}
	}
	return -1;
}

int xlm_get_launcher_field_by_name(const char * name)
{
	int i;

	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		if(_xlm_launcher_field_name[i])
		{
			if(!strcmp(name, _xlm_launcher_field_name[i]))
			{
				return i;
			}
		}
	}
	return -1;
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

bool xlm_set_launcher_icon(XLM_LAUNCHER * lp, const char * fn)
{
	const char * icon_copy_filename;
	const char * icon_folder;
	const char * icon_extension;
	int icon_field;

	icon_folder = al_path_cstr(t3f_data_path, '/');
	if(icon_folder)
	{
		icon_extension = xlm_get_filename_extension(fn);
		if(icon_extension)
		{
			icon_copy_filename = xlm_get_next_filename(icon_folder, "xlm_icon_", icon_extension);
			if(icon_copy_filename)
			{
				if(xlm_copy_file(fn, icon_copy_filename))
				{
					icon_field = xlm_get_launcher_field_by_key("Icon");
					if(icon_field >= 0)
					{
						xlm_set_launcher_field(lp, icon_field, icon_copy_filename);
						lp->icon = al_load_bitmap(icon_copy_filename);
						if(!lp->icon)
						{
							// load placeholder for icons that can't be loaded
						}
					}
				}
			}
		}
	}
	return true;
}
