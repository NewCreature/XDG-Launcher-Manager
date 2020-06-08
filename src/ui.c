#include "t3f/t3f.h"
#include "ui.h"

XLM_UI * xlm_create_ui(XLM_LAUNCHER_DATABASE * ldp)
{
	XLM_UI * uip;

	uip = malloc(sizeof(XLM_UI));
	if(!uip)
	{
		goto fail;
	}
	memset(uip, 0, sizeof(XLM_UI));
	uip->launcher_database = ldp;
	uip->button_theme = t3gui_load_theme("data/themes/basic/button_theme.ini", 0);
	if(!uip->button_theme)
	{
		goto fail;
	}
	return uip;

	fail:
	{
		xlm_destroy_ui(uip);
		return NULL;
	}
}

void xlm_destroy_ui(XLM_UI * uip)
{
	if(uip)
	{
		if(uip->button_theme)
		{
			t3gui_destroy_theme(uip->button_theme);
		}
		free(uip);
	}
}

void xlm_process_ui(XLM_UI * uip)
{
	if(t3f_key[ALLEGRO_KEY_LEFT])
	{
		uip->selected_launcher--;
		if(uip->selected_launcher < 0)
		{
			uip->selected_launcher = uip->launcher_database->launcher_count - 1;
			if(uip->selected_launcher < 0)
			{
				uip->selected_launcher = 0;
			}
		}
		t3f_key[ALLEGRO_KEY_LEFT] = 0;
	}
	if(t3f_key[ALLEGRO_KEY_RIGHT])
	{
		uip->selected_launcher++;
		if(uip->selected_launcher >= uip->launcher_database->launcher_count)
		{
			uip->selected_launcher = 0;
		}
		t3f_key[ALLEGRO_KEY_RIGHT] = 0;
	}
}

void xlm_render_ui(XLM_UI * uip)
{
	int pos_x = 0;
	int pos_y = 0;
	int i;

	al_clear_to_color(t3f_color_black);
	al_draw_textf(*uip->button_theme->state[0].font, t3f_color_white, pos_x, pos_y, 0, "Launcher %d/%d", uip->selected_launcher + 1, uip->launcher_database->launcher_count);
	pos_y += al_get_font_line_height(*uip->button_theme->state[0].font) * 2;
	if(uip->launcher_database->launcher_count > 0)
	{
		for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
		{
			al_draw_textf(*uip->button_theme->state[0].font, t3f_color_white, pos_x, pos_y, 0, "%d: %s", i + 1, uip->launcher_database->launcher[uip->selected_launcher]->field[i]);
			pos_y += al_get_font_line_height(*uip->button_theme->state[0].font);
		}
	}
}
