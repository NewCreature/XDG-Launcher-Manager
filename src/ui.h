#ifndef XLM_UI_H
#define XLM_UI_H

#include "t3gui/t3gui.h"
#include "t3gui/theme.h"
#include "launcher_database.h"

typedef struct
{

	T3GUI_THEME * box_theme;
	T3GUI_THEME * button_theme;
	T3GUI_THEME * list_box_theme;
	T3GUI_THEME * slider_theme;

	T3GUI_DIALOG * dialog;

	XLM_LAUNCHER_DATABASE * launcher_database;
	int selected_launcher;

} XLM_UI;

XLM_UI * xlm_create_ui(XLM_LAUNCHER_DATABASE * ldp);
void xlm_destroy_ui(XLM_UI * uip);

void xlm_process_ui(XLM_UI * uip);
void xlm_render_ui(XLM_UI * uip);

#endif
