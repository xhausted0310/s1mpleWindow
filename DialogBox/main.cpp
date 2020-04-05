#include <Windows.h>
#include "resource.h"
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1),0, DlgProc, 0);
	return  0;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG://инициализация диалогового окна
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0,LOWORD(hIcon));
	}
		break;
	case WM_COMMAND://обработка команд нажатия на кнопки и т.д
		switch (LOWORD(wParam))
		{
		case IDOK://кнопка ОК
			MessageBox(hwnd, "была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL://кнопка CANCEL
			EndDialog(hwnd,0);
		}
		break;
	case WM_CLOSE://закрытие окна кнопкой Х
		EndDialog(hwnd, 0);
		return true;
		break;
	}
	return false;
}