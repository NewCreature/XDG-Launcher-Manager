#include "t3f/t3f.h"
#include "t3gui/t3gui.h"
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
	uip->box_theme = t3gui_load_theme("data/themes/basic/box_theme.ini", 0);
	if(!uip->box_theme)
	{
		goto fail;
	}
	uip->dialog = t3gui_create_dialog();
	if(!uip->dialog)
	{
		goto fail;
	}
	t3gui_dialog_add_element(
		uip->dialog,
		uip->box_theme,
		t3gui_box_proc,
		0, 0, t3f_default_view->width, t3f_default_view->height,
		0, 0,
		0, 0, NULL, NULL, NULL
	);
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
		if(uip->dialog)
		{
			t3gui_destroy_dialog(uip->dialog);
		}
		if(uip->box_theme)
		{
			t3gui_destroy_theme(uip->button_theme);
		}
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
	t3gui_logic();
}

void xlm_render_ui(XLM_UI * uip)
{
	al_clear_to_color(t3f_color_black);
	t3gui_render();
}
