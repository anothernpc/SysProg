#include <Windows.h>
#include "TextEditorDefinitions.h"
#include "AboutDialogDefinitions.h"


COLORREF GetRandomColor() {
    COLORREF rgb = RGB(rand() % 256, rand() % 256, rand() % 256);
    while (rgb == spriteColor || rgb == RGB(0, 0, 0))
        rgb = RGB(rand() % 256, rand() % 256, rand() % 256);
    return rgb;
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
        break;
    }

    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        KillTimer(hWnd, 2);
        DestroyWindow(hWnd);
        hScreensaverOverlay = NULL;
        break;
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

void ResetInactivityTimer(HWND hParent) {
    if (inactivityTimer) KillTimer(hParent, 1);
    inactivityTimer = SetTimer(hParent, 1, INACTIVITY_TIMEOUT, NULL);
}

LRESULT CALLBACK TextEditorMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wp) == OnMenuAboutHelpClicked) {
            DisplayAboutDialog(hWnd);
            SetActiveWindow(hWnd);
        }
        break;

    case WM_CREATE:
        MainWndAddMenus(hWnd);
        ResetInactivityTimer(hWnd);
        break;

    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        if (hScreensaverOverlay) {
            KillTimer(hScreensaverOverlay, 2);
            DestroyWindow(hScreensaverOverlay);
            hScreensaverOverlay = NULL;
        }
        ResetInactivityTimer(hWnd);
        break;

    case WM_TIMER:
        if (wp == 1 && !hScreensaverOverlay) {
            StartScreensaverOverlay(hWnd);
        }
        break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
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

    AppendMenu(SubMenuHelp, MF_STRING, OnMenuAboutHelpClicked, L"About program");

    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuFile, L"File");
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuEdit, L"Edit");
    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenuHelp, L"Help");

    SetMenu(hWnd, RootMenu);
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
