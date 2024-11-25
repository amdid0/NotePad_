#include <windows.h>
#include <commdlg.h> 
#include <fstream>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void OpenFile(HWND hwnd, HWND hEdit);
void SaveFile(HWND hwnd, HWND hEdit);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SimpleNotepadClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Simple Notepad",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit;

    switch (uMsg) {
    case WM_CREATE: {
        // bar
        HMENU hMenu = CreateMenu();
        HMENU hFileMenu = CreatePopupMenu();

        AppendMenu(hFileMenu, MF_STRING, 1, L"&Open...");
        AppendMenu(hFileMenu, MF_STRING, 2, L"&Save...");
        AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenu(hFileMenu, MF_STRING, 3, L"E&xit");

        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
        SetMenu(hwnd, hMenu);


        hEdit = CreateWindowEx(
            0, L"EDIT", nullptr,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            0, 0, 0, 0,
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
        case 1: // Open
            OpenFile(hwnd, hEdit);
            break;
        case 2: // Save
            SaveFile(hwnd, hEdit);
            break;
        case 3: // Exit
            PostQuitMessage(0);
            break;
        }
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OpenFile(HWND hwnd, HWND hEdit) {
    OPENFILENAME ofn = {};
    wchar_t szFileName[MAX_PATH] = L"";

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"txt";

    if (GetOpenFileName(&ofn)) {
        std::ifstream file(szFileName);
        if (file) {
            std::string content((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            SetWindowTextA(hEdit, content.c_str());
        }
    }
}

void SaveFile(HWND hwnd, HWND hEdit) {
    OPENFILENAME ofn = {};
    wchar_t szFileName[MAX_PATH] = L"";

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"txt";

    if (GetSaveFileName(&ofn)) {
        int length = GetWindowTextLength(hEdit);
        if (length > 0) {
            char* buffer = new char[length + 1];
            GetWindowTextA(hEdit, buffer, length + 1);

            std::ofstream file(szFileName);
            if (file) {
                file << buffer;
            }
            delete[] buffer;
        }
    }
}
