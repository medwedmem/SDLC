#pragma once

#include <windows.h>

#include <string>

// Вспомогательные функции создания элементов управления WinAPI.
namespace ui {

inline HFONT font() {
    static HFONT f = CreateFontW(-15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                 DEFAULT_PITCH, L"Segoe UI");
    return f;
}

inline HFONT boldFont() {
    static HFONT f = CreateFontW(-15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                 DEFAULT_PITCH, L"Segoe UI");
    return f;
}

inline HWND create(HWND parent, const wchar_t* cls, const wchar_t* text, DWORD style, int x, int y,
                   int w, int h, int id = 0, DWORD exStyle = 0) {
    HWND hwnd = CreateWindowExW(exStyle, cls, text, WS_CHILD | WS_VISIBLE | style, x, y, w, h,
                                parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                                GetModuleHandleW(nullptr), nullptr);
    SendMessageW(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(font()), TRUE);
    return hwnd;
}

inline HWND label(HWND parent, const wchar_t* text, int x, int y, int w, int h) {
    return create(parent, L"STATIC", text, SS_LEFT, x, y, w, h);
}

inline HWND edit(HWND parent, int x, int y, int w, int h) {
    return create(parent, L"EDIT", L"", WS_TABSTOP | ES_AUTOHSCROLL, x, y, w, h, 0,
                  WS_EX_CLIENTEDGE);
}

inline HWND combo(HWND parent, int x, int y, int w, int h) {
    return create(parent, L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST, x, y, w, h);
}

inline HWND button(HWND parent, const wchar_t* text, int id, int x, int y, int w, int h,
                   bool isDefault) {
    return create(parent, L"BUTTON", text, WS_TABSTOP | (isDefault ? BS_DEFPUSHBUTTON : 0), x, y,
                  w, h, id);
}

inline std::wstring text(HWND hwnd) {
    std::wstring s(GetWindowTextLengthW(hwnd) + 1, L'\0');
    GetWindowTextW(hwnd, s.data(), static_cast<int>(s.size()));
    s.resize(s.size() - 1);
    return s;
}

}  // namespace ui
