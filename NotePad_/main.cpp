//
//unicode, WINAPI
//
#include <windows.h>
#include <commdlg.h>
#include <fstream>
#include <string>

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OpenFile(HWND hwnd, HWND hEdit);
void SaveFile(HWND hwnd, HWND hEdit);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SimpleNotepadClass";

    // define wc
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    //  RC
    RegisterClass(&wc);

    //  window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Simple Notepad",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Size and position
        nullptr, nullptr, hInstance, nullptr // Parent, menu, and instance handles
    );

    // check hwnd
    if (hwnd == nullptr) {
        return 0;
    }

    // Swindow
    ShowWindow(hwnd, nCmdShow);

    // msg loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// winproc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit; // Handle to the edit control

    switch (uMsg) {
    case WM_CREATE: {
        // menu bar
        HMENU hMenu = CreateMenu();
        HMENU hFileMenu = CreatePopupMenu();

        // menu2
        AppendMenu(hFileMenu, MF_STRING, 1, L"&Open...");
        AppendMenu(hFileMenu, MF_STRING, 2, L"&Save...");
        AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenu(hFileMenu, MF_STRING, 3, L"E&xit");

        // next menu
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
        SetMenu(hwnd, hMenu);

        // txt edit
        hEdit = CreateWindowEx(
            0, L"EDIT", nullptr,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            0, 0, 0, 0, // Initial position and size
            hwnd, nullptr, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr
        );
        return 0;
    }
    case WM_SIZE: {
        
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);
        SetWindowPos(hEdit, nullptr, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
        return 0;
    }
    case WM_COMMAND: {
        
        switch (LOWORD(wParam)) {
        case 1: // Open file
            OpenFile(hwnd, hEdit);
            break;
        case 2: // Sav
