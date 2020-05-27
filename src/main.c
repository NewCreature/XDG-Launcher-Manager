#include "t3f/t3f.h"
#include "t3gui/t3gui.h"
#include "t3gui/theme.h"

/* structure to hold all of our app-specific data */
typedef struct
{

	/* GUI data */
	T3GUI_THEME * box_theme;
	T3GUI_THEME * button_theme;
	T3GUI_THEME * list_box_theme;
	T3GUI_THEME * slider_theme;
	T3GUI_DIALOG * dialog;

	char launcher_folder[1024];

} APP_INSTANCE;

/* main logic routine */
void app_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

}

/* main rendering routine */
void app_render(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

}

/* initialize our app, load graphics, etc. */
bool app_initialize(APP_INSTANCE * app, int argc, char * argv[])
{

	const char * home_path;

	/* initialize T3F */
	if(!t3f_initialize(T3F_APP_TITLE, 640, 480, 60.0, app_logic, app_render, T3F_DEFAULT, app))
	{
		printf("Error initializing T3F\n");
		return false;
	}
	if(!t3gui_init())
	{
		printf("Error initializing T3GUI\n");
		return false;
	}
	home_path = getenv("HOME");
	if(!home_path)
	{
		printf("Unable to determine home directory.\n");
		return false;
	}
	sprintf(app->launcher_folder, "%s/.local/share/applications/", home_path);

	return true;
}

int main(int argc, char * argv[])
{
	APP_INSTANCE app;

	if(app_initialize(&app, argc, argv))
	{
		t3f_run();
	}
	else
	{
		printf("Error: could not initialize T3F!\n");
	}
	t3f_finish();
	return 0;
}
