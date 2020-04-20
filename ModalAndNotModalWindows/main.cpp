#include "DialogProcs.h"


int CALLBACK  WinMain(HINSTANCE hInstance, HINSTANCE hPervInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, DlgMain, 0);
	return 0;
}

