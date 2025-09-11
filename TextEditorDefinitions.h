#pragma once
#define OnMenuFileClicked 2 
#define OnMenuOpenFileClicked 3
#define OnMenuSaveFileClicked 4
#define OnMenuExitFileClicked 5

#define OnMenuEditClicked 6
#define OnMenuCutEditClicked 7
#define OnMenuCopyEditClicked 8
#define OnMenuPasteEditClicked 9

#define OnMenuHelpClicked 10
#define OnMenuAboutHelpClicked 11

#define INACTIVITY_TIMEOUT 30000


HWND hScreensaverOverlay = NULL;
UINT_PTR inactivityTimer = 0;
COLORREF spriteColor = RGB(255, 255, 255);


LRESULT CALLBACK TextEditorMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void MainWndAddMenus(HWND hWnd);
void ResetInactivityTimer(HWND hParent);