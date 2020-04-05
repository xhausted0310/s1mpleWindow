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
	case WM_INITDIALOG://������������� ����������� ����
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0,LOWORD(hIcon));
	}
		break;
	case WM_COMMAND://��������� ������ ������� �� ������ � �.�
		switch (LOWORD(wParam))
		{
		case IDOK://������ ��
			MessageBox(hwnd, "���� ������ ������ ��", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL://������ CANCEL
			EndDialog(hwnd,0);
		}
		break;
	case WM_CLOSE://�������� ���� ������� �
		EndDialog(hwnd, 0);
		return true;
		break;
	}
	return false;
}