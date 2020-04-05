#include <Windows.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInst, LPSTR lpCmdLine, int nCmdShow)
{
	if  (MessageBox(NULL, "HELLO WINAPI;-)", "Header", MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFBUTTON2) == IDOK);
	{
		MessageBox(NULL,"LOL","LOL_Header",MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
	}
	else
	{
		MessageBox(NULL, "WHAT?", "WHAT_HEADER" , MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
	}
}

