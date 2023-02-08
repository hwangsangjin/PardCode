#include "App.h"

int main()
{
	App app;
	if (app.Initialize())
	{
		while (app.IsRunning())
		{
			app.Broadcast();
		}
	}

	return 0;
}