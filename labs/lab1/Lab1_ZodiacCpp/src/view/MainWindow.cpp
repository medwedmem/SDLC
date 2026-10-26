#include "MainWindow.h"

#include <cstdio>

#include "../model/ZodiacModel.h"
#include "Ui.h"

namespace {
constexpr wchar_t CLASS_NAME[] = L"ZodiacMainWindow";
constexpr int ID_INPUT = 100;

const wchar_t* MONTHS[] = {L"января", L"февраля", L"марта", L"апреля", L"мая", L"июня",
                           L"июля", L"августа", L"сентября", L"октября", L"ноября", L"декабря"};

std::wstring dayMonth(int day, int month) {
    return std::to_wstring(day) + L" " + MONTHS[month - 1];
}
}  // namespace

bool MainWindow::create(HINSTANCE instance, int showCommand) {
    WNDCLASSW wc{};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);

    hwnd_ = CreateWindowExW(0, CLASS_NAME, L"Астролог",
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                            CW_USEDEFAULT, CW_USEDEFAULT, 560, 360, nullptr, nullptr, instance,
                            this);
    if (!hwnd_) return false;
    ShowWindow(hwnd_, showCommand);
    UpdateWindow(hwnd_);
    return true;
}

LRESULT CALLBACK MainWindow::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* self = nullptr;
    if (msg == WM_NCCREATE) {
        self = static_cast<MainWindow*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
        self->hwnd_ = hwnd;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }
    return self ? self->handleMessage(msg, wParam, lParam) : DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT MainWindow::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        createControls();
        return 0;
    case WM_SETFOCUS:
        SetFocus(inputButton_);  // кнопка нажимается и с клавиатуры
        return 0;
    case WM_COMMAND:
        if ((LOWORD(wParam) == ID_INPUT || LOWORD(wParam) == IDOK) && inputAction_) {
            inputAction_();  // действие пользователя передается контроллеру
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd_, msg, wParam, lParam);
}

void MainWindow::createControls() {
    inputButton_ = ui::button(hwnd_, L"Узнать свой знак Зодиака", ID_INPUT, 16, 16, 512, 32, true);
    const wchar_t* captions[] = {L"Дата рождения:", L"Знак Зодиака:", L"Период:", L"Стихия:",
                                 L"Планета-покровитель:", L"Характеристика:"};
    HWND* values[] = {&dateLabel_, &signLabel_, &periodLabel_, &elementLabel_, &planetLabel_,
                      &descriptionLabel_};
    for (int i = 0; i < 6; ++i) {
        const int y = 66 + i * 34;
        ui::label(hwnd_, captions[i], 16, y, 170, 24);
        *values[i] = ui::label(hwnd_, i == 0 ? L"не введена" : L"—", 190, y, 338, i == 5 ? 60 : 24);
    }
    SendMessageW(signLabel_, WM_SETFONT, reinterpret_cast<WPARAM>(ui::boldFont()), TRUE);
}

void MainWindow::showError(const std::wstring& message) const {
    MessageBoxW(hwnd_, message.c_str(), L"Некорректные данные", MB_OK | MB_ICONERROR);
}

void MainWindow::modelChanged(const ZodiacModel& model) {
    if (!model.isCalculated()) return;
    wchar_t date[32];
    swprintf(date, 32, L"%02d.%02d.%04d", model.day(), model.month(), model.year());
    const ZodiacSign& s = model.sign();
    SetWindowTextW(dateLabel_, date);
    SetWindowTextW(signLabel_, s.name.c_str());
    SetWindowTextW(periodLabel_, (L"с " + dayMonth(s.startDay, s.startMonth) + L" по " +
                                  dayMonth(s.endDay, s.endMonth)).c_str());
    SetWindowTextW(elementLabel_, s.element.c_str());
    SetWindowTextW(planetLabel_, s.planet.c_str());
    SetWindowTextW(descriptionLabel_, s.description.c_str());
}
