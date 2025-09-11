#include <Windows.h>
#include "AboutDialogDefinitions.h"

int DialogMain(HINSTANCE hInst) {

	WNDCLASS AboutDialogWindowClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_APPLICATION), L"AboutDialogWndClass", AboutDialogMainProcedure);
	if (!RegisterClassW(&AboutDialogWindowClass)) { return -1; }

	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {

	WNDCLASS NWC = { 0 };

	NWC.hCursor = Cursor;
	NWC.hIcon = Icon;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK AboutDialogMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
		case OnButtonCloseClicked:		
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CREATE:
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		/*case WM_PAINT:
			break;*/

	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void DisplayAboutDialog(HWND hWnd) {
	HWND hDlg = CreateWindowExW(WS_EX_CLIENTEDGE, L"AboutDialogWndClass", L"About Dialog", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, ABOUT_DIALOG_SIZE, ABOUT_DIALOG_SIZE, NULL, NULL, NULL, NULL);
	
	CreateWindow(L"static", L"About this program", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 0, ABOUT_DIALOG_SIZE, 40, hDlg, NULL, NULL, NULL);
	CreateWindow(L"edit", L"This is a simple (or not so simple! it's actually a ULTRA+ 3000 MEGA GIGA TURBO NEWEST TextEditor, you know?) text editor. Idk what to say here even it's a text editor literally.", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY , 0, 40, ABOUT_DIALOG_SIZE-20, 100, hDlg, NULL, NULL, NULL);
	CreateWindow(L"Button", L"Close", WS_VISIBLE | WS_CHILD, 80, ABOUT_DIALOG_SIZE - 100, 100, 40, hDlg, (HMENU)OnButtonCloseClicked, NULL, NULL);

	EnableWindow(hWnd, false);

	MSG AboutDialogMainMessage = { 0 };

	while (GetMessage(&AboutDialogMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&AboutDialogMainMessage);
		DispatchMessage(&AboutDialogMainMessage);
	}

	EnableWindow(hWnd, true);
	
}