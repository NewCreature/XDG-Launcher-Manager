#include "t3f/t3f.h"
#include "t3gui/t3gui.h"
#include "t3gui/theme.h"
#include "launcher_database.h"
#include "ui.h"

/* structure to hold all of our app-specific data */
typedef struct
{

	XLM_LAUNCHER_DATABASE * launcher_database;
	XLM_UI * ui;

} APP_INSTANCE;

/* main logic routine */
void app_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	xlm_process_ui(app->ui);
}

/* main rendering routine */
void app_render(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	xlm_render_ui(app->ui);
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
	app->launcher_database = xlm_create_launcher_database(home_path);
	if(!app->launcher_database)
	{
		printf("Unable to create launcher database!\n");
		return false;
	}
	app->ui = xlm_create_ui(app->launcher_database);
	if(!app->ui)
	{
		printf("Unable to create UI!\n");
		return false;
	}
	t3gui_show_dialog(app->ui->dialog, NULL, 0, app->ui);

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
