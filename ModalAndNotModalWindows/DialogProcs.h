#pragma once

#include<Windows.h>
#include"resource.h"


BOOL CALLBACK DlgMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgNotModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
