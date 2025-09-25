#pragma once
#define ABOUT_DIALOG_SIZE 300
#define OnButtonCloseClicked 1

LRESULT CALLBACK AboutDialogMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
int DialogMain(HINSTANCE hInst);
void DisplayAboutDialog(HWND hWnd);
