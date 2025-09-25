#include <Windows.h>
#include "TextEditorDefinitions.h"
#include "AboutDialogDefinitions.h"
#include "EditWidgetDefinitions.h"


void SetOpenFileNameParams(HWND hWnd) {
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = fileNameBuffer;
    ofn.nMaxFile = sizeof(fileNameBuffer);
    ofn.lpstrFilter = L".txt";
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = L"E:\\LABS\\sysprog\\try1\\try3-sp\\x64\\Debug\\";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

}

void MainWndAddMenus(HWND hWnd) {
    HMENU RootMenu = CreateMenu();
    HMENU SubMenuFile = CreateMenu();
    HMENU SubMenuEdit = CreateMenu();
    HMENU SubMenuHelp = CreateMenu();

    AppendMenu(SubMenuFile, MF_STRING, OnMenuOpenFileClicked, L"Open");
    AppendMenu(SubMenuFile, MF_STRING, OnMenuSaveFileClicked, L"Save");
    AppendMenu(SubMenuFile, MFT_SEPARATOR, NULL, NULL);
    AppendMenu(SubMenuFile, MF_STRING, OnMenuExitFileClicked, L"Exit");

    AppendMenu(SubMenuEdit, MF_STRING, OnMenuCutEditClicked, L"Cut");
    AppendMenu(SubMenuEdit, MF_STRING, OnMenuCopyEditClicked, L"Copy");
    AppendMenu(SubMenuEdit, MF_STRING, OnMenuPasteEditClicked, L"Paste");
    AppendMenu(SubMenuEdit, MF_STRING, OnMenuClearEditClicked, L"Clear");

    AppendMenu(SubMenuHelp, MF_STRING, OnMenuAboutHelpClicked, L"About program");

    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuFile, L"File");
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuEdit, L"Edit");
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuHelp, L"Help");

    SetMenu(hWnd, RootMenu);
}




//работа со скринсейвером




void StopScreensaverOverlay() {
    if (hScreensaverOverlay) {
        KillTimer(hScreensaverOverlay, 2);
        DestroyWindow(hScreensaverOverlay);
        hScreensaverOverlay = NULL;
    }
}

COLORREF GetRandomColor() {
    COLORREF rgb = RGB(rand() % 256, rand() % 256, rand() % 256);
    while (rgb == spriteColor || rgb == RGB(0, 0, 0))
        rgb = RGB(rand() % 256, rand() % 256, rand() % 256);
    return rgb;
}

void ResetInactivityTimer(HWND hParent) {
    if (inactivityTimer) KillTimer(hParent, 1);
    inactivityTimer = SetTimer(hParent, 1, INACTIVITY_TIMEOUT, NULL);
}

LRESULT CALLBACK ScreensaverOverlayProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    static int x = 10, y = 10, dx = 3, dy = 2;
    static COLORREF spriteColor = RGB(255, 255, 255);

    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));

        RECT sprite = { x, y, x + 50, y + 50 };
        HBRUSH hBrush = CreateSolidBrush(spriteColor);
        FillRect(hdc, &sprite, hBrush);
        DeleteObject(hBrush);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER: {
        if (!(GetForegroundWindow() == hWnd)) {
            x += dx;
            y += dy;

            RECT r;
            GetClientRect(hWnd, &r);

            bool bounced = false;

            if (x < 0 || x + 50 > r.right) {
                dx = -dx;
                bounced = true;
            }
            if (y < 0 || y + 50 > r.bottom) {
                dy = -dy;
                bounced = true;
            }

            if (bounced) {
                spriteColor = GetRandomColor();
            }

            InvalidateRect(hWnd, NULL, TRUE);
        }
        else {
            StopScreensaverOverlay();
        }
        break;
    }
    }

    return DefWindowProcW(hWnd, msg, wp, lp);
}

void StartScreensaverOverlay(HWND parent) {
    RECT clientRect;
    GetClientRect(parent, &clientRect);

    hScreensaverOverlay = CreateWindowEx(0, L"STATIC", NULL,
        WS_CHILD | WS_VISIBLE,
        0, 0, clientRect.right, clientRect.bottom,
        parent, NULL, NULL, NULL);

    SetWindowLongPtr(hScreensaverOverlay, GWLP_WNDPROC, (LONG_PTR)ScreensaverOverlayProc);
    SetTimer(hScreensaverOverlay, 2, 30, NULL);
}





//работа c Edit




LRESULT CALLBACK EditSubclassProc(HWND hEdit, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_KEYDOWN:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        StopScreensaverOverlay();
        ResetInactivityTimer(GetParent(hEdit));
        break;
    }
    return CallWindowProc(originalEditProc, hEdit, msg, wp, lp);
}




//работа с главным окном





LRESULT CALLBACK TextEditorMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COMMAND:
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == hEditWidget) {
            isTextModified = true;
        }
        switch (wp) {
        case OnMenuAboutHelpClicked:
            DisplayAboutDialog(hWnd);
            SetActiveWindow(hWnd);
            break;
        case OnMenuClearEditClicked:
            SetWindowTextW(hEditWidget, L"");
            break;
        case OnMenuSaveFileClicked:
            if (GetSaveFileNameW(&ofn)) {
                SaveData(fileNameBuffer);
                isTextModified = false;
            }
            break;
        case OnMenuOpenFileClicked:
            if (GetOpenFileNameW(&ofn)) {
                LoadData(fileNameBuffer);
            }
            break;
        case OnMenuCopyEditClicked:
            SendMessage(hEditWidget, WM_COPY, 0, 0);
            break;
        case OnMenuCutEditClicked:
            SendMessage(hEditWidget, WM_CUT, 0, 0);
            break;
        case OnMenuPasteEditClicked:
            SendMessage(hEditWidget, WM_PASTE, 0, 0);
            break;
        default: break;
        }
        break;

    case WM_CREATE:
        MainWndAddMenus(hWnd);
        CreateEditWidget(hWnd);
        ResetInactivityTimer(hWnd);
        SetOpenFileNameParams(hWnd);

        originalEditProc = (WNDPROC)SetWindowLongPtr(hEditWidget, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);
        break;


    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        StopScreensaverOverlay();
        ResetInactivityTimer(hWnd);
        break;

    case WM_TIMER:
        if (wp == 1 && !hScreensaverOverlay && !(GetForegroundWindow() == hWnd)) {
            StartScreensaverOverlay(hWnd);
        }
        break;
    case WM_SIZE: {
        if (hEditWidget) {
            int width = LOWORD(lp);
            int height = HIWORD(lp);
            MoveWindow(hEditWidget, 0, 0, width, height, TRUE);
        }
        break;
    }
    case WM_CLOSE:
        if (isTextModified) {
            int result = MessageBox(hWnd,
                L"Would you like to save changes before you exit?",
                L"Exit",
                MB_YESNOCANCEL | MB_ICONQUESTION);
            if (result == IDYES) {
                if (GetSaveFileNameW(&ofn)) {
                    SaveData(fileNameBuffer);
                }
                DestroyWindow(hWnd);
            }
            else if (result == IDNO) {
                DestroyWindow(hWnd);
            }
        }
        else {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);

        if (originalEditProc) {
            SetWindowLongPtr(hEditWidget, GWLP_WNDPROC, (LONG_PTR)originalEditProc);
            originalEditProc = nullptr;
        }

        PostQuitMessage(0);
        break;


    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}








int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    srand(GetTickCount64()); 

    WNDCLASS TextEditorMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_APPLICATION), L"MainWndClass", TextEditorMainProcedure);
    if (!RegisterClassW(&TextEditorMainClass)) return -1;

    CreateWindowExW(WS_EX_CLIENTEDGE, L"MainWndClass", L"ULTRA+ 3000 MEGA GIGA TURBO NEWEST TextEditor", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 300, NULL, NULL, hInst, NULL);
    DialogMain(hInst);

    MSG TextEditorMainMessage = { 0 };
    while (GetMessage(&TextEditorMainMessage, NULL, 0, 0)) {
        TranslateMessage(&TextEditorMainMessage);
        DispatchMessage(&TextEditorMainMessage);
    }
    return 0;
}
