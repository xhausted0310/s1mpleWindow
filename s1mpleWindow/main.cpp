#include<Windows.h>
#include"resource.h"

CONST CHAR szFilter[] = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";

CHAR szFileName[MAX_PATH] = {};	//Имя открытого, или сохраненного файла
LPSTR lpszFileText;		//Содержимое открытого, или сохраненного файла

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL	CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL	LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);
BOOL	SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);

BOOL __stdcall DoFileOpen(HWND hwnd);
VOID DoFileSaveAs(HWND hwnd);

BOOL FileWasChanged(HWND hwnd);

VOID WatchChanges(HWND hwnd, BOOL (__stdcall *Action)(HWND))
{
	if (FileWasChanged(hwnd))
	{
		switch (MessageBox(hwnd, "Сохранить изменения в файле?", "Нетак быстро...", MB_YESNOCANCEL | MB_ICONQUESTION))
		{
		case IDYES:	SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
		case IDNO:	Action(hwnd);
		case IDCANCEL:break;
		}
	}
	else
	{
		Action(hwnd);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	if(lpCmdLine[0])
		{
		/*strcpy_s(szFileName, MAX_PATH, lpCmdLine);*/
		for (int i = 0, j = 0; lpCmdLine[i]; i++)
		{
			if (lpCmdLine[i] != '\"')szFileName[j++], lpCmdLine[i];
		}
		}
	//1) Регистрация класса окна:
	CONST CHAR SZ_CLASS_NAME[] = "myWindowClass";
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	//wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = SZ_CLASS_NAME;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:
	HWND hwnd = CreateWindowEx
	(
		WS_EX_ACCEPTFILES|WS_EX_CLIENTEDGE,
		SZ_CLASS_NAME,
		"Title of my Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window was not created", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Message loop (цикл сообщений):
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		//Creating menu:
		HMENU hMenu = CreateMenu();
		HMENU hSubMenu;

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_NEW, "&New");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
		AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVE, "&Save");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVEAS, "&SaveAs");
		AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_HELP, "&About");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

		SetMenu(hwnd, hMenu);

		//Adding icons:
		HICON hIcon = (HICON)LoadImage(NULL, "Document.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		//Text editor:
		RECT rect;
		GetClientRect(hwnd, &rect);

		HWND hEdit = CreateWindowEx
		(
			WS_EX_CLIENTEDGE/*|WS_EX_ACCEPTFILES*/, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			0, 0, rect.right, rect.bottom,
			hwnd,
			(HMENU)IDC_MAIN_EDIT,
			GetModuleHandle(NULL),
			NULL
		);

		if (hEdit == NULL)
			MessageBox(hwnd, "Can not create edit control", "Error", MB_OK | MB_ICONERROR);

		HFONT hDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hDefault, MAKELPARAM(FALSE, 0));

		if (szFileName[0])
		{
			LoadTextFileToEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), szFileName);
		}

	}
	break;
	case WM_SIZE:
	{
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
	}
	break;
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		DragQueryFile(hDrop, 0, szFileName, MAX_PATH);
		LoadTextFileToEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), szFileName);
		DragFinish(hDrop);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case ID_FILE_OPEN:
		{
			if (FileWasChanged(hwnd))
			{
				/*switch (MessageBox(hwnd, "Сохранить изменения в файле?", "Нетак быстро...", MB_YESNOCANCEL | MB_ICONQUESTION))
				{
				case IDYES:	SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
				case IDNO:	DoFileOpen(hwnd);
				case IDCANCEL:break;
				}*/
				WatchChanges(hwnd, DoFileOpen);
			}
			else
			{
				DoFileOpen(hwnd);
			}
		}
		break;
		case ID_FILE_SAVE:
		{
			if (strlen(szFileName))
			{
				SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), szFileName);
			}
			else
			{
				SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEAS, 0);
			}
		}
		break;
		case ID_FILE_SAVEAS:
		{
			DoFileSaveAs(hwnd);
		}
		break;
		case ID_FILE_EXIT:
			//DestroyWindow(hwnd);
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_HELP:
		{
			switch (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, DlgProc))
			{
			case IDOK:		MessageBox(hwnd, "Dialog ended with OK!", "Info", MB_OK | MB_ICONINFORMATION);		break;
			case IDCANCEL:	MessageBox(hwnd, "Dialog ended with Cancel!", "Info", MB_OK | MB_ICONINFORMATION);	break;
			case -1:		MessageBox(hwnd, "Dialog Failed!", "Error", MB_OK | MB_ICONERROR);		break;
			}
		}
		break;
		}
		break;
	case WM_CLOSE:
	{
		if (FileWasChanged(hwnd))
		{
			/*switch (MessageBox(hwnd, "Сохранить изменения в файле?", "Нетак быстро...", MB_YESNOCANCEL | MB_ICONQUESTION))
			{
			case IDYES:	SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
			case IDNO:	DestroyWindow(hwnd);
			case IDCANCEL:break;
			}*/
			WatchChanges(hwnd, DestroyWindow);
		}
		else
		{
			DestroyWindow(hwnd);
		}
	}
	break;
	case WM_DESTROY:
		//MessageBox(hwnd, "От странные, лчше б дверь закрыли", "Возмущение", MB_OK | MB_ICONINFORMATION);
		PostQuitMessage(0);
		break;
	default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

BOOL	CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:return FALSE;
	}
	return TRUE;
}

BOOL	LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
	BOOL bSuccess = FALSE;

	MessageBox(hEdit, pszFileName, "OpeningFile:", MB_OK | MB_ICONINFORMATION);

	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != UINT_MAX)
		{
			if (lpszFileText)GlobalFree(lpszFileText);
			lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (lpszFileText)
			{
				DWORD dwRead;

				if (ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL))
				{
					lpszFileText[dwFileSize] = 0;
					if (SetWindowText(hEdit, lpszFileText))bSuccess = TRUE;
				}
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL	SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength = GetWindowTextLength(hEdit);
		if (dwTextLength > 0)
		{
			if (lpszFileText)GlobalFree(lpszFileText);
			lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (lpszFileText != NULL)
			{
				if (GetWindowText(hEdit, lpszFileText, dwTextLength + 1))
				{
					DWORD dwWritten;
					if (WriteFile(hFile, lpszFileText, dwTextLength, &dwWritten, NULL))bSuccess = TRUE;
				}
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}


BOOL __stdcall DoFileOpen(HWND hwnd)
{
	//Создадим стандартное окно открытия файла:
	OPENFILENAME ofn;
	//CHAR szFileName[MAX_PATH]{};

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//ofn.lpstrDefExt = "txt";

	if (GetOpenFileName(&ofn))
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		LoadTextFileToEdit(hEdit, szFileName);
		return TRUE;
	}
	return FALSE;
}
VOID DoFileSaveAs(HWND hwnd)
{
	OPENFILENAME ofn;
	//CHAR szFileName[MAX_PATH]{};

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "txt";

	if (GetSaveFileName(&ofn))
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SaveTextFileFromEdit(hEdit, szFileName);
	}
}

BOOL FileWasChanged(HWND hwnd)
{
	BOOL bWasChanged = FALSE;
	HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
	DWORD dwCurrentTextLength = GetWindowTextLength(hEdit);
	DWORD dwFileTextLength = lpszFileText ? strlen(lpszFileText) : 0;
	if (dwCurrentTextLength != dwFileTextLength)return TRUE;
	else
	{
		//Если строки совпадают по размеру, то их нужно сравнивать:
		//1) Выделяем память, для текста в редакторе:
		LPSTR lpszCurrentText = (LPSTR)GlobalAlloc(GPTR, dwCurrentTextLength + 1);

		//2) Загружаем текст из редактора в строку: 
		GetWindowText(hEdit, lpszCurrentText, dwCurrentTextLength + 1);
		if (lpszFileText && strcmp(lpszCurrentText, lpszFileText) == 0)bWasChanged = FALSE;
		GlobalFree(lpszCurrentText);
	}
	return bWasChanged;
}