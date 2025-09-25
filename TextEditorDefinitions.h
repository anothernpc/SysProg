#pragma once
#define OnMenuFileClicked 2 
#define OnMenuOpenFileClicked 3
#define OnMenuSaveFileClicked 4
#define OnMenuExitFileClicked 5

#define OnMenuEditClicked 6
#define OnMenuCutEditClicked 7
#define OnMenuCopyEditClicked 8
#define OnMenuPasteEditClicked 9
#define OnMenuClearEditClicked 10

#define OnMenuHelpClicked 11
#define OnMenuAboutHelpClicked 12

#define INACTIVITY_TIMEOUT 30000
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 300
#define FILE_NAME_SIZE 256

HWND hScreensaverOverlay = NULL;
UINT_PTR inactivityTimer = 0;
COLORREF spriteColor = RGB(255, 255, 255);
WCHAR fileNameBuffer[MAX_PATH];
OPENFILENAME ofn;
WNDPROC originalEditProc = nullptr;
bool isTextModified = false;



LRESULT CALLBACK TextEditorMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ScreensaverOverlayProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void MainWndAddMenus(HWND hWnd);
void ResetInactivityTimer(HWND hParent);