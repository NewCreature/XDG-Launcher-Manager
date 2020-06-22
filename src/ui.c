#include "t3f/t3f.h"
#include "t3gui/t3gui.h"
#include "ui.h"

static const char * launcher_list_proc(int index, int *num_elem, void *dp3)
{
	XLM_LAUNCHER_DATABASE * launcher_database;

	launcher_database = (XLM_LAUNCHER_DATABASE *)dp3;
	switch(index)
	{
		case -1:
		{
			*num_elem = launcher_database->launcher_count;
			break;
		}
		default:
		{
			return launcher_database->launcher[index]->field[XLM_LAUNCHER_FIELD_NAME];
		}
	}
	return NULL;
}

static int add_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	XLM_UI * uip;

	uip = (XLM_UI *)dp3;
	xlm_add_launcher_to_database(uip->launcher_database);
	printf("add\n");

	return 0;
}

static int delete_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	XLM_UI * uip;

	uip = (XLM_UI *)dp3;
	xlm_delete_launcher_from_database(uip->launcher_database, uip->launcher_list_element->d1);
	printf("delete %d\n", uip->launcher_list_element->d1);

	return 0;
}

static int save_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	XLM_UI * uip;

	uip = (XLM_UI *)dp3;
	printf("save\n");

	return 0;
}

XLM_UI * xlm_create_ui(XLM_LAUNCHER_DATABASE * ldp)
{
	XLM_UI * uip;
	int pos_x[3];
	int pos_y = XLM_UI_MARGIN;
	int text_height;
	int usable_width;
	int button_width;
	int button_height;
	int i;

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
	uip->list_box_theme = t3gui_load_theme("data/themes/basic/listbox_theme.ini", 0);
	if(!uip->list_box_theme)
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
	text_height = al_get_font_line_height(*uip->button_theme->state[0].font);
	usable_width = (t3f_default_view->width / 3) * 2 - XLM_UI_MARGIN * 2;
	pos_x[0] = XLM_UI_MARGIN;
	pos_x[1] = t3f_default_view->width / 3 + XLM_UI_MARGIN / 2;
	pos_x[2] = (t3f_default_view->width / 3) * 2 + XLM_UI_MARGIN / 2;
	button_width = al_get_text_width(*uip->button_theme->state[0].font, "Save") + XLM_UI_MARGIN * 2;
	button_height = text_height + XLM_UI_MARGIN * 2;
	uip->launcher_list_element = t3gui_dialog_add_element(
		uip->dialog,
		uip->list_box_theme,
		t3gui_list_proc,
		pos_x[0], XLM_UI_MARGIN, t3f_default_view->width / 3 - XLM_UI_MARGIN - XLM_UI_MARGIN / 2,
		t3f_default_view->height - XLM_UI_MARGIN * 3 - button_height,
		0, 0,
		0, 0, launcher_list_proc, NULL, uip->launcher_database
	);
	uip->add_button_element = t3gui_dialog_add_element(
		uip->dialog,
		uip->button_theme,
		t3gui_push_button_proc,
		XLM_UI_MARGIN, t3f_default_view->height - XLM_UI_MARGIN - button_height,
		button_height, button_height,
		0, 0,
		0, 0, "+", add_button_proc, uip
	);
	uip->delete_button_element = t3gui_dialog_add_element(
		uip->dialog,
		uip->button_theme,
		t3gui_push_button_proc,
		XLM_UI_MARGIN + button_height, t3f_default_view->height - XLM_UI_MARGIN - button_height,
		button_height, button_height,
		0, 0,
		0, 0, "-", delete_button_proc, uip
	);
	uip->save_button_element = t3gui_dialog_add_element(
		uip->dialog,
		uip->button_theme,
		t3gui_push_button_proc,
		t3f_default_view->width - XLM_UI_MARGIN - button_width, t3f_default_view->height - XLM_UI_MARGIN - button_height,
		button_width, button_height,
		0, 0,
		0, 0, "Save", save_button_proc, uip
	);
	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		if(xlm_get_launcher_field_name(i))
		{
			t3gui_dialog_add_element(
				uip->dialog,
				uip->button_theme,
				t3gui_text_proc,
				pos_x[1], pos_y,
				usable_width / 2 - XLM_UI_MARGIN / 2,
				text_height,
				0, 0,
				0, 0, (void *)xlm_get_launcher_field_name(i), NULL, NULL
			);
			uip->edit_field_element[i] = t3gui_dialog_add_element(
				uip->dialog,
				uip->list_box_theme,
				t3gui_edit_proc,
				pos_x[2], pos_y,
				usable_width / 2 - XLM_UI_MARGIN / 2,
				text_height,
				0, 0,
				XLM_LAUNCHER_MAX_FIELD_SIZE, 0,
				uip->launcher_database->launcher[uip->selected_launcher]->field[i],
				NULL, NULL
			);
			pos_y += text_height + XLM_UI_MARGIN * 2;
		}
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

static void update_edit_fields(XLM_UI * uip)
{
	int i;

	for(i = 0; i < XLM_LAUNCHER_MAX_FIELDS; i++)
	{
		if(uip->edit_field_element[i])
		{
			if(uip->selected_launcher < uip->launcher_database->launcher_count)
			{
				uip->edit_field_element[i]->dp = uip->launcher_database->launcher[uip->selected_launcher]->field[i];
			}
			else
			{
				uip->edit_field_element[i]->dp = NULL;
			}
		}
	}
}

void xlm_process_ui(XLM_UI * uip)
{
	int current_launcher;
	int launcher_count;

	current_launcher = uip->launcher_list_element->d1;
	launcher_count = uip->launcher_database->launcher_count;
	t3gui_logic();
	uip->selected_launcher = uip->launcher_list_element->d1;
	if(uip->launcher_list_element->d1 != current_launcher || uip->launcher_database->launcher_count != launcher_count)
	{
		update_edit_fields(uip);
	}
}

void xlm_render_ui(XLM_UI * uip)
{
	al_clear_to_color(t3f_color_black);
	t3gui_render();
}
