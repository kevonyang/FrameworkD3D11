#include "Root.h"
#include <windows.h>
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Root *root = Root::GetInstance();
	bool res = root->Initialize();
	if (res)
		root->Run();

	root->Shutdown();

	return 0;
}