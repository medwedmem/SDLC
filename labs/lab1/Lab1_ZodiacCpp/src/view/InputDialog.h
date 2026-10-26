#pragma once

#include <windows.h>

#include <optional>
#include <string>

// Дата рождения, введенная пользователем. Хранится как текст, чтобы
// восстановить в окне даже ошибочный ввод.
struct InputData {
    std::wstring dayText;
    std::wstring monthText;
    std::wstring yearText;
};

// Модальное окно ввода даты рождения (стандартный элемент выбора даты
// не используется: день, месяц и год вводятся в текстовые поля).
class InputDialog {
public:
    // Показывает окно и возвращает введенные данные либо пустое значение,
    // если ввод отменен.
    static std::optional<InputData> show(HWND owner, const std::optional<InputData>& initial);

private:
    InputDialog(HWND owner, const std::optional<InputData>& initial);
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void createControls();
    void accept();

    HWND owner_;
    HWND hwnd_ = nullptr;
    HWND dayEdit_ = nullptr;
    HWND monthEdit_ = nullptr;
    HWND yearEdit_ = nullptr;
    std::optional<InputData> initial_;
    std::optional<InputData> result_;
    bool done_ = false;
};
