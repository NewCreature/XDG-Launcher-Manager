#include "t3f/t3f.h"

static char _xlm_next_filename[1024] = {0};

const char * xlm_get_filename_extension(const char * fn)
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

const char * xlm_get_next_filename(const char * path, const char * base_name, const char * extension)
{
	char buf[2048];
	int i;

	for(i = 0; i < 10000; i++)
	{
		sprintf(buf, "%s%s%03d%s", path, base_name, i, extension);
		if(!al_filename_exists(buf))
		{
			strcpy(_xlm_next_filename, buf);
			printf("new path %s\n", _xlm_next_filename);
			return _xlm_next_filename;
		}
	}
	return NULL;
}

bool xlm_copy_file(const char * fn, const char * dest_fn)
{
	ALLEGRO_FILE * source;
	ALLEGRO_FILE * dest;
	int c = 0;
	bool ret = false;

	source = al_fopen(fn, "rb");
	if(source)
	{
		dest = al_fopen(dest_fn, "wb");
		if(dest)
		{
			while(c >= 0)
			{
				c = al_fgetc(source);
				if(c >= 0)
				{
					al_fputc(dest, c);
				}
			}
			al_fclose(dest);
			ret = true;
		}
		al_fclose(source);
	}
	return ret;
}
