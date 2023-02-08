#include "App.h"

int main()
{
	App app;
	if (app.Init())
	{
		while (app.IsRun())
		{
			app.Broadcast();
		}
	}

	return 0;
}