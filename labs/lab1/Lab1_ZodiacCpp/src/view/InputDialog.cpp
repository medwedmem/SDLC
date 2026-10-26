#include "InputDialog.h"

#include "Ui.h"

namespace {
constexpr wchar_t CLASS_NAME[] = L"ZodiacInputDialog";
constexpr int ID_OK = 1;
constexpr int ID_CANCEL = 2;
}  // namespace

std::optional<InputData> InputDialog::show(HWND owner, const std::optional<InputData>& initial) {
    static bool registered = false;
    if (!registered) {
        WNDCLASSW wc{};
        wc.lpfnWndProc = windowProc;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
        wc.lpszClassName = CLASS_NAME;
        RegisterClassW(&wc);
        registered = true;
    }

    InputDialog dialog(owner, initial);
    RECT ownerRect;
    GetWindowRect(owner, &ownerRect);
    const int width = 380, height = 230;
    const int x = ownerRect.left + (ownerRect.right - ownerRect.left - width) / 2;
    const int y = ownerRect.top + (ownerRect.bottom - ownerRect.top - height) / 2;
    CreateWindowExW(WS_EX_DLGMODALFRAME, CLASS_NAME, L"Дата рождения",
                    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, x, y, width, height,
                    owner, nullptr, GetModuleHandleW(nullptr), &dialog);

    // Модальность: главное окно недоступно, пока открыто окно ввода.
    EnableWindow(owner, FALSE);
    MSG msg;
    while (!dialog.done_ && GetMessageW(&msg, nullptr, 0, 0)) {
        if (!IsDialogMessageW(dialog.hwnd_, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    EnableWindow(owner, TRUE);
    SetForegroundWindow(owner);
    DestroyWindow(dialog.hwnd_);
    return dialog.result_;
}

InputDialog::InputDialog(HWND owner, const std::optional<InputData>& initial)
    : owner_(owner), initial_(initial) {}

LRESULT CALLBACK InputDialog::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    InputDialog* self = nullptr;
    if (msg == WM_NCCREATE) {
        self = static_cast<InputDialog*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
        self->hwnd_ = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<InputDialog*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }
    return self ? self->handleMessage(msg, wParam, lParam) : DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT InputDialog::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        createControls();
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_OK || LOWORD(wParam) == IDOK) {
            accept();
        } else if (LOWORD(wParam) == ID_CANCEL || LOWORD(wParam) == IDCANCEL) {
            done_ = true;  // ввод отменен
        }
        return 0;
    case WM_CLOSE:
        done_ = true;
        return 0;
    }
    return DefWindowProcW(hwnd_, msg, wParam, lParam);
}

void InputDialog::createControls() {
    ui::label(hwnd_, L"День (1–31):", 16, 20, 180, 24);
    dayEdit_ = ui::edit(hwnd_, 200, 18, 150, 26);
    ui::label(hwnd_, L"Месяц (1–12):", 16, 58, 180, 24);
    monthEdit_ = ui::edit(hwnd_, 200, 56, 150, 26);
    ui::label(hwnd_, L"Год (например, 2005):", 16, 96, 180, 24);
    yearEdit_ = ui::edit(hwnd_, 200, 94, 150, 26);
    ui::button(hwnd_, L"OK", ID_OK, 170, 142, 85, 28, true);
    ui::button(hwnd_, L"Отмена", ID_CANCEL, 265, 142, 85, 28, false);

    // Восстановление последних введенных данных.
    if (initial_) {
        SetWindowTextW(dayEdit_, initial_->dayText.c_str());
        SetWindowTextW(monthEdit_, initial_->monthText.c_str());
        SetWindowTextW(yearEdit_, initial_->yearText.c_str());
    }
    SendMessageW(dayEdit_, EM_SETSEL, 0, -1);
    SetFocus(dayEdit_);
}

void InputDialog::accept() {
    result_ = InputData{ui::text(dayEdit_), ui::text(monthEdit_), ui::text(yearEdit_)};
    done_ = true;
}
