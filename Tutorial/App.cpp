#include "App.h"

void App::OnCreate()
{
	Window::OnCreate();
	Graphics::GetInstance()->Initialize();
}

void App::OnUpdate()
{
}

void App::OnDestroy()
{
	Window::OnDestroy();
	Graphics::GetInstance()->Release();
}
