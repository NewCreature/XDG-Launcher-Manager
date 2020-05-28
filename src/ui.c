#include "t3f/t3f.h"
#include "ui.h"

XLM_UI * xlm_create_ui(void)
{
	XLM_UI * uip;

	uip = malloc(sizeof(XLM_UI));
	if(!uip)
	{
		goto fail;
	}
	memset(uip, 0, sizeof(XLM_UI));
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
		free(uip);
	}
}
