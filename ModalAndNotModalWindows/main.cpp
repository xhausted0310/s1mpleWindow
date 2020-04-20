#include<Windows.h>
#include"resource.h"

HINSTANCE hInst;
HWND hwndNotModal;

BOOL CALLBACK DlgMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgNotModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int CALLBACK  WinMain(HINSTANCE hInstance, HINSTANCE hPervInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, DlgMain, 0);
	return 0;
}

BOOL CALLBACK DlgMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hInst = GetModuleHandle(NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BTN_MODAL:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_MODAL), hwnd, DlgModal, 0);
			break;
		case ID_BTN_NOT_MODAL:
			hwndNotModal = CreateDialog(hInst, MAKEINTRESOURCE(IDD_NOT_MODAL), 0, DlgNotModal, 0);
			ShowWindow(hwndNotModal, SW_SHOW);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return FALSE;
		break;
	}
	return FALSE;
}


BOOL CALLBACK DlgModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			MessageBox(hwnd, "Кого послать?", "Question", MB_OK | MB_ICONQUESTION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
			break;
		}
		break;
	
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return FALSE;
		break;
	}
	return FALSE;
}

BOOL CALLBACK DlgNotModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			MessageBox(hwnd, "Меня послало немодальное окно", "Вопрос", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			DestroyWindow(hwnd);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return FALSE;
		break;
	}
	return FALSE;
}