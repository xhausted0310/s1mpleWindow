#include "Windows.h"
#include "resource.h"

HWND hCombo;

const CHAR* string[] =
{
"THIS",
 "IS",
 "MY",
"FIRST",
"COMBOBOX"
};

CHAR buffer[25]{};

//CHAR string1[] = "THIS";
//CHAR string2[] = "IS";
//CHAR string3[] = "MY";
//CHAR string4[] = "FIRST";
//CHAR string5[] = "COMBOBOX";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lpParam);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPervInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lpParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 1, (LPARAM)hIcon);
		//ƒќЅј¬Ћя≈ћ —“–ќ » ¬  ќћЅќЅќ —
		hCombo = GetDlgItem(hwnd, IDC_COMBO1);
		for (int i = 0; i < sizeof(string) / sizeof(CHAR*); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)string[i]);
		}

		/*SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)string1);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)string2);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)string3);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)string4);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)string5);*/
	}
	break;

	case WM_COMMAND:
	{
		/*CHAR buffer[25]{};*/
		switch (LOWORD(wParam))
		{
		case IDC_COMBO1:
		{
			if (HIWORD(wParam) == CBN_SELENDOK)
			{
				int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)buffer);
				/*MessageBox(hwnd, buffer, "выбраный пунк списка", MB_OK | MB_ICONINFORMATION);*/
			}
		}
		break;
		case IDOK:	
		

				MessageBox(hwnd, buffer, "выбраный пунк списка", MB_OK | MB_ICONINFORMATION);
			
			/*MessageBox(hwnd, "hi", "hello", MB_OK | MB_ICONINFORMATION);*/
			
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
	break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;


	}
	return FALSE;

}